#ifndef __IO_GAME_STATE__
#define __IO_GAME_STATE__

#include <cstdint>
#include <string>

#include "car/car.h"
#include "neural/net.h"
#include "states/optim_state.h"
#include "tiles/tile_grid.h"

/// <summary>
/// GameState defines and maintains the overall game state
/// </summary>
class GameState
{
public:
    /// <summary>
    /// Provides options for the game mode selections
    /// </summary>
    enum class GameMode
    {
        OPTIM = 0,
        FILE = 1,
        BEST = 2
    };

public:
    /// <summary>
    /// Initializes the game state
    /// </summary>
    GameState();

    /// <summary>
    /// Initializes and draws the bitmaps for all necessary parameters.
    /// To be called after initializing the rendering library
    /// </summary>
    void init_bitmaps();

    /// <summary>
    /// Sets a new game mode and adjusts the game state according
    /// </summary>
    /// <param name="mode">the new game mode to select</param>
    void set_game_mode(GameMode mode);

    /// <summary>
    /// Resets the car and any other necessary state parameters
    /// </summary>
    void reset_car();

    /// <summary>
    /// Performs the step for the overall game state for the given game mode
    /// </summary>
    void step_state();

    /// <summary>
    /// Provides the current game mode
    /// </summary>
    /// <returns>the current game mode</returns>
    GameMode get_current_mode() const;

    /// <summary>
    /// Provides the best distance
    /// </summary>
    /// <returns>the best optimized distance so far</returns>
    double get_best_distance() const;

    /// <summary>
    /// Provides the default output filename
    /// </summary>
    /// <returns>the default filename to use for saved output values</returns>
    std::string get_output_fname() const;

    /// <summary>
    /// Provides the current network for the given mode
    /// </summary>
    /// <returns>the current network associated with the given mdoe</returns>
    NeuralNetwork* get_selected_network();

    /// <summary>
    /// Increases the step frequency counter to a limit
    /// by a power of 10
    /// </summary>
    void increment_step_frequency();

    /// <summary>
    /// Decreases the step frequency counter to a limit
    /// by a power of 10
    /// </summary>
    void decrement_step_frequency();

    /// <summary>
    /// Returns the current requested step frequency
    /// </summary>
    /// <returns>the requested step frequency, in Hz</returns>
    uint64_t current_frequency() const;

    /// <summary>
    /// Returns the current requested step period
    /// </summary>
    /// <returns>the requested step period, in seconds</returns>
    double current_period() const;

    /// <summary>
    /// Provides the current requested car input forward
    /// </summary>
    /// <returns>forward-positive car input</returns>
    double get_input_forward() const;

    /// <summary>
    /// Provides the current requested car input right
    /// </summary>
    /// <returns>right-positive car input</returns>
    double get_input_right() const;

private:
    /// <summary>
    /// Defines the number of deliniations to use in each positive/negative
    /// direction for the forward-positive input
    /// </summary>
    static const size_t num_forward_outputs;

    /// <summary>
    /// Defines the number of deliniations to use in each positive/negative
    /// direction for the right-positive input
    /// </summary>
    static const size_t num_turn_outputs;

public:
    /// <summary>
    /// Defines the car object to use to maintain the car state
    /// </summary>
    Car car;

    /// <summary>
    /// Defines the tile grid to use for the map
    /// </summary>
    RoadGrid tile_grid;

    /// <summary>
    /// Defines the optimization state
    /// </summary>
    OptimState optim_state;

    /// <summary>
    /// Defines the car's base frequency in Hz
    /// </summary>
    static const uint64_t car_step_base_frequency;

private:
    /// <summary>
    /// Defines the frequency multiplier to use to modify the game state
    /// </summary>
    uint32_t frequency_multipler = 3;

    /// <summary>
    /// Defines the requested forward input
    /// </summary>
    double input_forward = 0.0;

    /// <summary>
    /// Defines the requested right input
    /// </summary>
    double input_right = 0.0;

private:
    /// <summary>
    /// Provides the neural network to use when loading from a file
    /// </summary>
    NeuralNetwork net_file;

    /// <summary>
    /// Defines whether the network file was loaded successfully
    /// </summary>
    bool file_net_loaded = false;

    /// <summary>
    /// Define sthe current game mode
    /// </summary>
    GameMode current_mode = GameMode::OPTIM;
};

#endif