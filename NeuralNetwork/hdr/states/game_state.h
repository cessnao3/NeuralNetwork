#ifndef __IO_GAME_STATE__
#define __IO_GAME_STATE__

#include <cstdint>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>

// TODO - Remove This
#include <iostream>

#include "car/car.h"
#include "neural/net.h"
#include "states/optim_state.h"
#include "tiles/tile_grid.h"


class GameState
{
public:
    enum class GameMode
    {
        OPTIM = 0,
        FILE = 1,
        BEST = 2
    };

public:
    GameState() :
        tile_grid(6, 4),
        optim_state(car.sensor_count(), num_forward_outputs + num_turn_outputs)
    {
        // Read the file result
        file_net_loaded = false;

        try
        {
            std::ifstream input_vals(get_output_fname());
            std::ostringstream config;
            std::string l;
            while (std::getline(input_vals, l))
            {
                config << l << std::endl;
            }

            net_file = NeuralNetwork::from_config(config.str());
            file_net_loaded = true;
        }
        catch (const std::invalid_argument&)
        {
            file_net_loaded = false;
        }

        // Define the road grid
        tile_grid.set(0, 5, RoadTileManager::RoadTileType::CORNER_SW);
        tile_grid.set(0, 4, RoadTileManager::RoadTileType::STRAIGHT_H);
        tile_grid.set(0, 3, RoadTileManager::RoadTileType::STRAIGHT_H);
        tile_grid.set(0, 2, RoadTileManager::RoadTileType::CORNER_SE);
        tile_grid.set(1, 2, RoadTileManager::RoadTileType::CORNER_NW);
        tile_grid.set(1, 1, RoadTileManager::RoadTileType::STRAIGHT_H);
        tile_grid.set(1, 0, RoadTileManager::RoadTileType::CORNER_SE);
        tile_grid.set(2, 0, RoadTileManager::RoadTileType::STRAIGHT_V);
        tile_grid.set(3, 0, RoadTileManager::RoadTileType::CORNER_NE);
        tile_grid.set(3, 1, RoadTileManager::RoadTileType::STRAIGHT_H);
        tile_grid.set(3, 2, RoadTileManager::RoadTileType::STRAIGHT_H);
        tile_grid.set(3, 3, RoadTileManager::RoadTileType::CORNER_NW);
        tile_grid.set(2, 3, RoadTileManager::RoadTileType::CORNER_SE);
        tile_grid.set(2, 4, RoadTileManager::RoadTileType::CORNER_SW);
        tile_grid.set(3, 4, RoadTileManager::RoadTileType::CORNER_NE);
        tile_grid.set(3, 5, RoadTileManager::RoadTileType::CORNER_NW);
        tile_grid.set(2, 5, RoadTileManager::RoadTileType::STRAIGHT_V);
        tile_grid.set(1, 5, RoadTileManager::RoadTileType::STRAIGHT_V);

        tile_grid.set_start_ind(2, 0);

        // Initialize the starting position
        RoadGrid::GridLoc* start_pos = tile_grid.at(tile_grid.get_start_ind());
        car.set_pos(start_pos->get_center_x(), start_pos->get_center_y());
        car.set_rotation(3 * car.PI / 2);

        // Reset the car
        reset_car();
    }

    void init_bitmaps()
    {
        car.init_bitmap();
        tile_grid.init_bitmaps();
    }

    void set_game_mode(GameMode mode)
    {
        if (mode == current_mode)
        {
            return;
        }

        if (mode == GameMode::FILE)
        {
            if (file_net_loaded)
            {
                current_mode = mode;
            }
        }
        else
        {
            current_mode = mode;
        }

        if (mode == current_mode)
        {
            car.reset();
        }
    }

    void reset_car()
    {
        car.reset();
        current_car_step = 0;
        average_car_speed = 0.0;

        if (current_mode != GameMode::OPTIM)
        {
            optim_state.set_update_population();
        }
    }

    void step_state()
    {
        // Update the optimization step
        if (optim_state.step())
        {
            reset_car();
        }

        // Extract the current network
        NeuralNetwork* selected_net = get_selected_network();

        // Set the input sizes
        for (size_t i = 0; i < car.sensor_count(); ++i)
        {
            selected_net->set_input(i, car.get_sensor(tile_grid, i).dist);
        }

        // Step the network
        if (!selected_net->step())
        {
            throw std::exception("unable to step network");
        }

        // Set the outputs
        input_forward = 0.0;
        input_right = 0.0;

        for (size_t i = 0; i < num_forward_outputs; ++i)
        {
            double val;
            const double add_val = 1.0 / num_forward_outputs;
            const double thresh = 0.5;
            if (selected_net->get_output(0 + i, val) && val > 0.5)
            {
                if (val > thresh)
                {
                    input_forward += add_val;
                }
                else if (val < -thresh)
                {
                    input_forward -= add_val;
                }
            }
        }

        for (size_t i = 0; i < num_turn_outputs; ++i)
        {
            double val;
            const double add_val = 1.0 / num_turn_outputs;
            const double thresh = 0.5;
            if (selected_net->get_output(num_forward_outputs + i, val))
            {
                if (val > thresh)
                {
                    input_right += add_val;
                }
                else if (val < thresh)
                {
                    input_right -= add_val;
                }
            }
        }

        // Step the car
        car.step(input_forward, input_right);
        car.check_collision(tile_grid);

        const bool is_stuck = std::abs(input_forward) < 1e-6 && std::abs(car.get_forward_vel()) < 1e-6;

        // TODO - Move this to the car?
        average_car_speed = (current_car_step * average_car_speed + car.get_forward_vel()) / (current_car_step + 1.0);
        current_car_step += 1;

        if (current_mode == GameMode::OPTIM)
        {
            if (car.has_collided() || current_car_step > 100 * car_step_base_frequency || is_stuck)
            {
                // Check if the provided distance is better than the previous value
                if (optim_state.update_best(car.get_distance(), average_car_speed))
                {
                    // Write Outputs
                    std::cout << "D: " << car.get_distance() << std::endl;

                    // Save the best-so-far network

                    std::ostringstream fname;
                    fname << get_output_fname() << "." << optim_state.get_num_best_update_counts();

                    std::ofstream output(fname.str());
                    if (output.is_open())
                    {
                        output << optim_state.get_best_network()->get_config();
                        output.close();
                    }
                    else
                    {
                        std::ostringstream error_str;
                        error_str << "Error opening file " << fname.str() << " for writing" << std::endl;
                        throw std::exception(error_str.str().c_str());
                    }
                }

                // Move to the next value
                optim_state.next_network();

                // Reset the car
                reset_car();
            }
        }
    }

    GameMode get_current_mode() const
    {
        return current_mode;
    }

    double get_best_distance() const
    {
        return optim_state.get_best_distance();
    }

    std::string get_output_fname() const
    {
        return "default.txt";
    }

    NeuralNetwork* get_selected_network()
    {
        switch (current_mode)
        {
        case GameMode::OPTIM:
            return optim_state.get_optim_network();
        case GameMode::FILE:
            return &net_file;
        case GameMode::BEST:
            return optim_state.get_best_network();
        default:
            return nullptr;
        }
    }

    void increment_step_frequency()
    {
        if (frequency_multipler < 5)
        {
            frequency_multipler += 1;
        }
    }

    void decrement_sstep_frequency()
    {
        if (frequency_multipler > 0)
        {
            frequency_multipler -= 1;
        }
    }

    uint64_t current_frequency() const
    {
        return car_step_base_frequency * std::pow(10, frequency_multipler);
    }

    double current_period() const
    {
        return 1.0 / current_frequency();
    }

    double get_input_forward() const
    {
        return input_forward;
    }

    double get_input_right() const
    {
        return input_right;
    }

private:
    const size_t num_forward_outputs = 10;
    const size_t num_turn_outputs = 10;

public:
    Car car;
    RoadGrid tile_grid;

    OptimState optim_state;

    const uint64_t car_step_base_frequency = 100;

private:
    size_t current_car_step = 0;
    double average_car_speed = 0.0;

    uint32_t frequency_multipler = 0;

    double input_forward = 0.0;
    double input_right = 0.0;

private:
    NeuralNetwork net_file;
    GameMode current_mode = GameMode::OPTIM;
    bool file_net_loaded = false;
};

#endif
