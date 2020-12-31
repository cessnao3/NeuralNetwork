#include "states/game_state.h"

#include <fstream>
#include <sstream>

const size_t GameState::num_forward_outputs = 10;
const size_t GameState::num_turn_outputs = 10;

const uint64_t GameState::car_step_base_frequency = 100;

GameState::GameState() :
    tile_grid(10, 6),
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
    const size_t row_offset = 1;
    const size_t col_offset = 2;
    tile_grid.set(row_offset + 0, col_offset + 5, RoadTileManager::RoadTileType::CORNER_SW);
    tile_grid.set(row_offset + 0, col_offset + 4, RoadTileManager::RoadTileType::STRAIGHT_H);
    tile_grid.set(row_offset + 0, col_offset + 3, RoadTileManager::RoadTileType::STRAIGHT_H);
    tile_grid.set(row_offset + 0, col_offset + 2, RoadTileManager::RoadTileType::CORNER_SE);
    tile_grid.set(row_offset + 1, col_offset + 2, RoadTileManager::RoadTileType::CORNER_NW);
    tile_grid.set(row_offset + 1, col_offset + 1, RoadTileManager::RoadTileType::STRAIGHT_H);
    tile_grid.set(row_offset + 1, col_offset + 0, RoadTileManager::RoadTileType::CORNER_SE);
    tile_grid.set(row_offset + 2, col_offset + 0, RoadTileManager::RoadTileType::STRAIGHT_V);
    tile_grid.set(row_offset + 3, col_offset + 0, RoadTileManager::RoadTileType::CORNER_NE);

    //tile_grid.set(row_offset + 3, col_offset + 1, RoadTileManager::RoadTileType::CORNER_NW);
    //tile_grid.set(row_offset + 2, col_offset + 1, RoadTileManager::RoadTileType::CORNER_SE);
    //tile_grid.set(row_offset + 2, col_offset + 2, RoadTileManager::RoadTileType::CORNER_SW);
    //tile_grid.set(row_offset + 3, col_offset + 2, RoadTileManager::RoadTileType::CORNER_NE);

    tile_grid.set(row_offset + 3, col_offset + 1, RoadTileManager::RoadTileType::STRAIGHT_H);
    tile_grid.set(row_offset + 3, col_offset + 2, RoadTileManager::RoadTileType::STRAIGHT_H);

    tile_grid.set(row_offset + 3, col_offset + 3, RoadTileManager::RoadTileType::CORNER_NW);
    tile_grid.set(row_offset + 2, col_offset + 3, RoadTileManager::RoadTileType::CORNER_SE);
    tile_grid.set(row_offset + 2, col_offset + 4, RoadTileManager::RoadTileType::CORNER_SW);
    tile_grid.set(row_offset + 3, col_offset + 4, RoadTileManager::RoadTileType::CORNER_NE);
    tile_grid.set(row_offset + 3, col_offset + 5, RoadTileManager::RoadTileType::CORNER_NW);
    tile_grid.set(row_offset + 2, col_offset + 5, RoadTileManager::RoadTileType::STRAIGHT_V);
    tile_grid.set(row_offset + 1, col_offset + 5, RoadTileManager::RoadTileType::STRAIGHT_V);

    tile_grid.set_start_ind(row_offset + 2, col_offset + 0);

    // Initialize the starting position
    RoadGrid::GridLoc* start_pos = tile_grid.at(tile_grid.get_start_ind());
    car.set_pos(start_pos->get_center_x(), start_pos->get_center_y());
    car.set_start_rotation(3 * car.PI / 2);

    // Reset the car
    reset_car();
}

void GameState::init_bitmaps()
{
    car.init_bitmap();
    tile_grid.init_bitmaps();
}

void GameState::set_game_mode(GameMode mode)
{
    // Skip if the mode is already the current mode
    if (mode == current_mode)
    {
        return;
    }

    // Special consideration for file to ensure that the 
    // neural network was successfully loaded from a file before
    // setting the file mode
    if (mode == GameMode::FILE && !file_net_loaded)
    {
        return;
    }

    // Set the new mode
    current_mode = mode;

    // Reset the car and set the optimization state flag to reset
    car.reset();
    optim_state.set_update_design_flag();
}

void GameState::reset_car()
{
    car.reset();
}

void GameState::step_state()
{
    // Update the optimization step
    if (optim_state.update_network_design())
    {
        reset_car();
    }

    // Extract the current network
    NeuralNetwork* selected_net = get_selected_network();

    // Set the input sizes
    for (size_t i = 0; i < car.sensor_count(); ++i)
    {
        selected_net->set_input(i, car.get_sensor(i).dist);
    }

    // Step the network
    if (!selected_net->update_network_design())
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
    car.step_movement(tile_grid, input_forward, input_right);

    // Determine if the car is stuck
    const bool is_stuck = std::abs(input_forward) < 1e-6 && std::abs(car.get_forward_input()) < 1e-6;

    // Perform special consideration values for 
    if (current_mode == GameMode::OPTIM)
    {
        if (car.has_collided() || car.get_step_count() > 100 * car_step_base_frequency || is_stuck)
        {
            // Check if the provided distance is better than the previous value
            if (optim_state.check_update_best_design(car.get_distance(), car.get_average_speed()))
            {
                // Save the resulting network
                const std::string fname = get_temp_fname();
                std::ofstream output(fname);
                if (output.is_open())
                {
                    output << optim_state.get_best_network()->get_config();
                    output.close();
                }
                else
                {
                    std::ostringstream error_str;
                    error_str << "Error opening file " << fname << " for writing" << std::endl;
                    throw std::exception(error_str.str().c_str());
                }
            }

            // Move to the next value
            optim_state.step_to_next_design();

            // Reset the car
            reset_car();
        }
    }
}

GameState::GameMode GameState::get_current_mode() const
{
    return current_mode;
}

double GameState::get_best_distance() const
{
    return optim_state.get_best_distance();
}

std::string GameState::get_output_fname() const
{
    return "default.txt";
}

std::string GameState::get_temp_fname() const
{
    std::ostringstream fname;
    fname << "temp_" << optim_state.get_num_best_update_counts() << ".txt";
    return fname.str();
}

NeuralNetwork* GameState::get_selected_network()
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

void GameState::increment_step_frequency()
{
    if (frequency_multipler < 5)
    {
        frequency_multipler += 1;
    }
}

void GameState::decrement_step_frequency()
{
    if (frequency_multipler > 0)
    {
        frequency_multipler -= 1;
    }
}

uint64_t GameState::current_frequency() const
{
    return car_step_base_frequency * std::pow(9, frequency_multipler);
}

double GameState::current_period() const
{
    return 1.0 / current_frequency();
}

double GameState::get_input_forward() const
{
    return input_forward;
}

double GameState::get_input_right() const
{
    return input_right;
}
