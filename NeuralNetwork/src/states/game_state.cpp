#include "states/game_state.h"

#include <fstream>
#include <sstream>

#include <stdexcept>

#include <cmath>

#include <cassert>

const size_t GameState::num_forward_outputs = 10;
const size_t GameState::num_turn_outputs = 10;

const uint64_t GameState::car_step_base_frequency = 100;

const size_t GameState::tile_grid_width = 16;
const size_t GameState::tile_grid_height = 9;

static const bool include_inverse = false;

GameState::GameState() :
    optim_state(car.sensor_count() * (include_inverse ? 2 : 1), num_forward_outputs + num_turn_outputs)
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
    {
        const size_t row_offset = 0;
        const size_t col_offset = 1;

        RoadGrid tile_grid(16, 9);

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

        tile_grids.push_back(tile_grid);
    }

    {
        RoadGrid tile_grid(16, 9);

        const size_t row_offset = 0;
        const size_t col_offset = 0;

        tile_grid.set(row_offset + 1, col_offset + 1, RoadTileManager::RoadTileType::CORNER_SE);
        tile_grid.set(row_offset + 1, col_offset + 2, RoadTileManager::RoadTileType::STRAIGHT_H);
        tile_grid.set(row_offset + 1, col_offset + 3, RoadTileManager::RoadTileType::CORNER_NW);
        tile_grid.set(row_offset + 0, col_offset + 3, RoadTileManager::RoadTileType::CORNER_SE);
        tile_grid.set(row_offset + 0, col_offset + 4, RoadTileManager::RoadTileType::STRAIGHT_H);
        tile_grid.set(row_offset + 0, col_offset + 5, RoadTileManager::RoadTileType::STRAIGHT_H);
        tile_grid.set(row_offset + 0, col_offset + 6, RoadTileManager::RoadTileType::STRAIGHT_H);
        tile_grid.set(row_offset + 0, col_offset + 7, RoadTileManager::RoadTileType::STRAIGHT_H);
        tile_grid.set(row_offset + 0, col_offset + 8, RoadTileManager::RoadTileType::STRAIGHT_H);
        tile_grid.set(row_offset + 0, col_offset + 9, RoadTileManager::RoadTileType::STRAIGHT_H);
        tile_grid.set(row_offset + 0, col_offset + 10, RoadTileManager::RoadTileType::STRAIGHT_H);
        tile_grid.set(row_offset + 0, col_offset + 11, RoadTileManager::RoadTileType::STRAIGHT_H);
        tile_grid.set(row_offset + 0, col_offset + 12, RoadTileManager::RoadTileType::STRAIGHT_H);
        tile_grid.set(row_offset + 0, col_offset + 13, RoadTileManager::RoadTileType::STRAIGHT_H);
        tile_grid.set(row_offset + 0, col_offset + 14, RoadTileManager::RoadTileType::STRAIGHT_H);
        tile_grid.set(row_offset + 0, col_offset + 15, RoadTileManager::RoadTileType::CORNER_SW);

        tile_grid.set(row_offset + 1, col_offset + 15, RoadTileManager::RoadTileType::CORNER_NW);
        tile_grid.set(row_offset + 1, col_offset + 14, RoadTileManager::RoadTileType::STRAIGHT_H);
        tile_grid.set(row_offset + 1, col_offset + 13, RoadTileManager::RoadTileType::STRAIGHT_H);
        tile_grid.set(row_offset + 1, col_offset + 12, RoadTileManager::RoadTileType::CORNER_SE);
        tile_grid.set(row_offset + 2, col_offset + 12, RoadTileManager::RoadTileType::CORNER_NW);
        tile_grid.set(row_offset + 2, col_offset + 11, RoadTileManager::RoadTileType::CORNER_NE);
        tile_grid.set(row_offset + 1, col_offset + 11, RoadTileManager::RoadTileType::CORNER_SW);
        tile_grid.set(row_offset + 1, col_offset + 10, RoadTileManager::RoadTileType::STRAIGHT_H);
        tile_grid.set(row_offset + 1, col_offset + 9, RoadTileManager::RoadTileType::CORNER_SE);
        tile_grid.set(row_offset + 2, col_offset + 9, RoadTileManager::RoadTileType::CORNER_NW);
        tile_grid.set(row_offset + 2, col_offset + 8, RoadTileManager::RoadTileType::CORNER_SE);
        tile_grid.set(row_offset + 3, col_offset + 8, RoadTileManager::RoadTileType::CORNER_NW);
        tile_grid.set(row_offset + 3, col_offset + 7, RoadTileManager::RoadTileType::CORNER_SE);
        tile_grid.set(row_offset + 4, col_offset + 7, RoadTileManager::RoadTileType::CORNER_NW);
        tile_grid.set(row_offset + 4, col_offset + 6, RoadTileManager::RoadTileType::CORNER_SE);
        tile_grid.set(row_offset + 5, col_offset + 6, RoadTileManager::RoadTileType::CORNER_NE);
        tile_grid.set(row_offset + 5, col_offset + 7, RoadTileManager::RoadTileType::STRAIGHT_H);
        tile_grid.set(row_offset + 5, col_offset + 8, RoadTileManager::RoadTileType::CORNER_NW);
        tile_grid.set(row_offset + 4, col_offset + 8, RoadTileManager::RoadTileType::CORNER_SE);
        tile_grid.set(row_offset + 4, col_offset + 9, RoadTileManager::RoadTileType::CORNER_NW);
        tile_grid.set(row_offset + 3, col_offset + 9, RoadTileManager::RoadTileType::CORNER_SE);
        tile_grid.set(row_offset + 3, col_offset + 10, RoadTileManager::RoadTileType::STRAIGHT_H);
        tile_grid.set(row_offset + 3, col_offset + 11, RoadTileManager::RoadTileType::STRAIGHT_H);
        tile_grid.set(row_offset + 3, col_offset + 12, RoadTileManager::RoadTileType::STRAIGHT_H);
        tile_grid.set(row_offset + 3, col_offset + 13, RoadTileManager::RoadTileType::CORNER_NW);
        tile_grid.set(row_offset + 2, col_offset + 13, RoadTileManager::RoadTileType::CORNER_SE);
        tile_grid.set(row_offset + 2, col_offset + 14, RoadTileManager::RoadTileType::STRAIGHT_H);
        tile_grid.set(row_offset + 2, col_offset + 15, RoadTileManager::RoadTileType::CORNER_SW);
        tile_grid.set(row_offset + 3, col_offset + 15, RoadTileManager::RoadTileType::CORNER_NW);
        tile_grid.set(row_offset + 3, col_offset + 14, RoadTileManager::RoadTileType::CORNER_SE);
        tile_grid.set(row_offset + 4, col_offset + 14, RoadTileManager::RoadTileType::CORNER_NE);
        tile_grid.set(row_offset + 4, col_offset + 15, RoadTileManager::RoadTileType::CORNER_SW);
        tile_grid.set(row_offset + 5, col_offset + 15, RoadTileManager::RoadTileType::CORNER_NW);
        tile_grid.set(row_offset + 5, col_offset + 14, RoadTileManager::RoadTileType::STRAIGHT_H);
        tile_grid.set(row_offset + 5, col_offset + 13, RoadTileManager::RoadTileType::CORNER_NE);
        tile_grid.set(row_offset + 4, col_offset + 13, RoadTileManager::RoadTileType::CORNER_SW);
        tile_grid.set(row_offset + 4, col_offset + 12, RoadTileManager::RoadTileType::STRAIGHT_H);
        tile_grid.set(row_offset + 4, col_offset + 11, RoadTileManager::RoadTileType::STRAIGHT_H);

        tile_grid.set(row_offset + 4, col_offset + 10, RoadTileManager::RoadTileType::CORNER_SE);
        tile_grid.set(row_offset + 5, col_offset + 10, RoadTileManager::RoadTileType::STRAIGHT_V);
        tile_grid.set(row_offset + 6, col_offset + 10, RoadTileManager::RoadTileType::CORNER_NE);
        tile_grid.set(row_offset + 6, col_offset + 11, RoadTileManager::RoadTileType::STRAIGHT_H);
        tile_grid.set(row_offset + 6, col_offset + 12, RoadTileManager::RoadTileType::STRAIGHT_H);
        tile_grid.set(row_offset + 6, col_offset + 13, RoadTileManager::RoadTileType::STRAIGHT_H);

        tile_grid.set(row_offset + 6, col_offset + 14, RoadTileManager::RoadTileType::CORNER_SW);
        tile_grid.set(row_offset + 7, col_offset + 14, RoadTileManager::RoadTileType::CORNER_NE);
        tile_grid.set(row_offset + 7, col_offset + 15, RoadTileManager::RoadTileType::CORNER_SW);
        tile_grid.set(row_offset + 8, col_offset + 15, RoadTileManager::RoadTileType::CORNER_NW);
        tile_grid.set(row_offset + 8, col_offset + 14, RoadTileManager::RoadTileType::STRAIGHT_H);
        tile_grid.set(row_offset + 8, col_offset + 13, RoadTileManager::RoadTileType::CORNER_NE);
        tile_grid.set(row_offset + 7, col_offset + 13, RoadTileManager::RoadTileType::CORNER_SW);

        tile_grid.set(row_offset + 7, col_offset + 12, RoadTileManager::RoadTileType::STRAIGHT_H);
        tile_grid.set(row_offset + 7, col_offset + 11, RoadTileManager::RoadTileType::STRAIGHT_H);
        tile_grid.set(row_offset + 7, col_offset + 10, RoadTileManager::RoadTileType::STRAIGHT_H);
        tile_grid.set(row_offset + 7, col_offset + 9, RoadTileManager::RoadTileType::STRAIGHT_H);
        tile_grid.set(row_offset + 7, col_offset + 8, RoadTileManager::RoadTileType::STRAIGHT_H);
        tile_grid.set(row_offset + 7, col_offset + 7, RoadTileManager::RoadTileType::STRAIGHT_H);
        tile_grid.set(row_offset + 7, col_offset + 6, RoadTileManager::RoadTileType::STRAIGHT_H);
        tile_grid.set(row_offset + 7, col_offset + 5, RoadTileManager::RoadTileType::CORNER_NE);

        tile_grid.set(row_offset + 6, col_offset + 5, RoadTileManager::RoadTileType::STRAIGHT_V);
        tile_grid.set(row_offset + 5, col_offset + 5, RoadTileManager::RoadTileType::STRAIGHT_V);
        tile_grid.set(row_offset + 4, col_offset + 5, RoadTileManager::RoadTileType::STRAIGHT_V);
        tile_grid.set(row_offset + 3, col_offset + 5, RoadTileManager::RoadTileType::STRAIGHT_V);
        tile_grid.set(row_offset + 2, col_offset + 5, RoadTileManager::RoadTileType::CORNER_SE);
        tile_grid.set(row_offset + 2, col_offset + 6, RoadTileManager::RoadTileType::CORNER_NW);
        tile_grid.set(row_offset + 1, col_offset + 6, RoadTileManager::RoadTileType::CORNER_SW);
        tile_grid.set(row_offset + 1, col_offset + 5, RoadTileManager::RoadTileType::STRAIGHT_H);
        tile_grid.set(row_offset + 1, col_offset + 4, RoadTileManager::RoadTileType::CORNER_SE);

        tile_grid.set(row_offset + 2, col_offset + 4, RoadTileManager::RoadTileType::STRAIGHT_V);
        tile_grid.set(row_offset + 3, col_offset + 4, RoadTileManager::RoadTileType::STRAIGHT_V);
        tile_grid.set(row_offset + 4, col_offset + 4, RoadTileManager::RoadTileType::STRAIGHT_V);
        tile_grid.set(row_offset + 5, col_offset + 4, RoadTileManager::RoadTileType::STRAIGHT_V);
        tile_grid.set(row_offset + 6, col_offset + 4, RoadTileManager::RoadTileType::STRAIGHT_V);
        tile_grid.set(row_offset + 7, col_offset + 4, RoadTileManager::RoadTileType::CORNER_NW);

        tile_grid.set(row_offset + 7, col_offset + 3, RoadTileManager::RoadTileType::CORNER_NE);
        tile_grid.set(row_offset + 6, col_offset + 3, RoadTileManager::RoadTileType::CORNER_SW);
        tile_grid.set(row_offset + 6, col_offset + 2, RoadTileManager::RoadTileType::CORNER_NE);
        tile_grid.set(row_offset + 5, col_offset + 2, RoadTileManager::RoadTileType::CORNER_SW);
        tile_grid.set(row_offset + 5, col_offset + 1, RoadTileManager::RoadTileType::CORNER_NE);
        tile_grid.set(row_offset + 4, col_offset + 1, RoadTileManager::RoadTileType::CORNER_SW);

        tile_grid.set(row_offset + 4, col_offset + 0, RoadTileManager::RoadTileType::CORNER_NE);
        tile_grid.set(row_offset + 3, col_offset + 0, RoadTileManager::RoadTileType::CORNER_SE);
        tile_grid.set(row_offset + 3, col_offset + 1, RoadTileManager::RoadTileType::CORNER_NW);
        tile_grid.set(row_offset + 2, col_offset + 1, RoadTileManager::RoadTileType::STRAIGHT_V);

        tile_grid.set_start_ind(row_offset + 2, col_offset + 1);

        tile_grids.push_back(tile_grid);
    }

    // Initialize the tile grid values to the first grid
    tile_grid_index = 0;

    // Initialize the starting position
    const RoadGrid::GridLoc* start_pos = get_tile_grid()->at(get_tile_grid()->get_start_ind());
    car.set_pos(start_pos->get_center_x(), start_pos->get_center_y());
    car.set_start_rotation(1 * car.PI / 2);

    // Reset the car
    reset_car();
}

void GameState::init_bitmaps()
{
    car.init_bitmap();
    for (size_t i = 0; i < tile_grids.size(); ++i)
    {
        tile_grids[i].init_bitmaps();
    }
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
    for (uint64_t i = 0; i < std::pow(10, frequency_multipler); ++i)
    {
        step_state_inner();
    }
}

void GameState::step_state_inner()
{
    // Update the optimization step
    if (optim_state.update_network_design())
    {
        reset_car();
    }

    // Extract the current network
    NeuralNetwork* selected_net = get_selected_network();

    // Set the input sensor and inverse sensor inputs
    for (size_t i = 0; i < car.sensor_count(); ++i)
    {
        const double dist_val = car.get_sensor(i).dist;
        if (!selected_net->set_input(i, dist_val))
        {
            assert(false);
        }

        if (include_inverse)
        {
            if (!selected_net->set_input(car.sensor_count() + i, 1.0 - dist_val))
            {
                assert(false);
            }
        }
    }

    // Step the network
    if (!selected_net->step_network())
    {
        throw std::runtime_error("unable to step network");
    }

    // Set the outputs
    input_forward = 0.0;
    input_right = 0.0;

    for (size_t i = 0; i < num_forward_outputs; ++i)
    {
        const bool is_neg = i % 2 == 0;

        double val = 0.0;
        if (selected_net->get_output(i, val))
        {
            if (is_neg)
            {
                val = -val;
            }

            input_forward += val / static_cast<double>(num_forward_outputs);
        }
    }

    for (size_t i = 0; i < num_turn_outputs; ++i)
    {
        const bool is_neg = i % 2 == 0;

        double val = 0.0;
        if (selected_net->get_output(num_forward_outputs + i, val))
        {
            if (is_neg)
            {
                val = -val;
            }

            input_right += val / static_cast<double>(num_turn_outputs);
        }
    }

    // Step the car
    car.step_movement(*get_tile_grid(), input_forward, input_right);

    // Determine if the car is stuck
    const bool is_stuck = std::abs(car.get_forward_input()) < 1e-3 || car.get_distance() < -10.0;
        //(std::abs(input_forward) < 1e-6 && std::abs(car.get_forward_input()) < 1e-6);// ||
        //(std::abs(car.get_delta_distance() < 0.05) && car.get_step_count() * car.step_period() > 3.0);

    // Perform special consideration values for
    if (current_mode == GameMode::OPTIM)
    {
        if (car.has_collided() || car.get_step_count() > 300 * car_step_base_frequency || is_stuck)
        {
            // Check if the provided distance is better than the previous value
            if (optim_state.check_update_best_design(car) && save_optim_network_flag)
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
                    throw std::runtime_error(error_str.str().c_str());
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
    if (frequency_multipler < 4)
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

double GameState::base_period() const
{
    return 1.0 / static_cast<double>(car_step_base_frequency);
}

uint32_t GameState::get_frequency_multiplier() const
{
    return frequency_multipler;
}

double GameState::get_input_forward() const
{
    if (std::abs(input_forward) < 0.001)
    {
        return 0.0;
    }
    else
    {
        return input_forward;
    }
}

double GameState::get_input_right() const
{
    if (std::abs(input_right) < 0.001)
    {
        return 0.0;
    }
    else
    {
        return input_right;
    }
}

uint32_t GameState::get_screen_width() const
{
    return static_cast<uint32_t>(get_tile_grid()->get_width() * RoadTile::TILE_SIZE);
}

uint32_t GameState::get_screen_height() const
{
    return static_cast<uint32_t>(get_tile_grid()->get_height() * RoadTile::TILE_SIZE);
}

const RoadGrid* GameState::get_tile_grid() const
{
    if (tile_grid_index >= tile_grids.size())
    {
        throw std::out_of_range("selected tile grid index is beyond the valid range");
    }
    else
    {
        return &tile_grids[tile_grid_index];
    }
}

void GameState::set_tile_grid_index(const size_t ind)
{
    if (ind >= tile_grids.size())
    {
        throw std::out_of_range("cannot set tile grid beyond current range");
    }
    else
    {
        tile_grid_index = ind;
    }
}

size_t GameState::get_tile_grid_count() const
{
    return tile_grids.size();
}

size_t GameState::get_tile_grid_index() const
{
    return tile_grid_index;
}

void GameState::toggle_save_best_networks()
{
    save_optim_network_flag = !save_optim_network_flag;
}

bool GameState::get_save_bets_networks_flag() const
{
    return save_optim_network_flag;
}
