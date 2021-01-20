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

private:
    /// <summary>
    /// Performs the inner-step to perform one iteration of the step
    /// </summary>
    void step_state_inner();

    /// <summary>
    /// Obtains an input value from the given network
    /// </summary>
    /// <param name="selected_net">the neural network to obtain outputs from</param>
    /// <param name="init_index">the initial output index</param>
    /// <param name="input_size">the number of contributing network indices</param>
    /// <returns>the resulting input value, between [-1.0, 1.0], inclusive</returns>
    double get_input_value(
        const NeuralNetwork* selected_net,
        const size_t init_index,
        const size_t input_size) const;

public:
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
    /// Provides the temporary output fname for the current best updated count
    /// </summary>
    /// <returns></returns>
    std::string get_temp_fname() const;

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
    /// Provides the base period of the game state
    /// </summary>
    /// <returns>base period, in seconds</returns>
    double base_period() const;

    /// <summary>
    /// Provides the current frequency multiplier
    /// </summary>
    /// <returns>the frequency multiplier</returns>
    uint32_t get_frequency_multiplier() const;

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

    /// <summary>
    /// Provide the given screen width
    /// </summary>
    /// <returns>the screen width</returns>
    uint32_t get_screen_width() const;

    /// <summary>
    /// Provide the given screen height
    /// </summary>
    /// <returns>the screen height</returns>
    uint32_t get_screen_height() const;

    /// <summary>
    /// Provides the currently-selected tile grid
    /// </summary>
    /// <returns>a pointer to the currently-selected instance</returns>
    const RoadGrid* get_tile_grid() const;

    /// <summary>
    /// Sets the tile grid instance value
    /// </summary>
    /// <param name="ind">the index to set</param>
    void set_tile_grid_index(const size_t ind);

    /// <summary>
    /// Returns the total number of tile grid instances
    /// </summary>
    /// <returns>the total number of tile grid instances</returns>
    size_t get_tile_grid_count() const;

    /// <summary>
    /// Returns the current index of the selected tile grid
    /// </summary>
    /// <returns>current selected index</returns>
    size_t get_tile_grid_index() const;

    /// <summary>
    /// Toggles whether to save the best network values
    /// </summary>
    void toggle_save_best_networks();

    /// <summary>
    /// Returns whether best networks should be saved
    /// </summary>
    /// <returns>True if networks should be saved</returns>
    bool get_save_bets_networks_flag() const;

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
    std::vector<RoadGrid> tile_grids;

    /// <summary>
    /// Provides the currently-selected tile grid instance
    /// </summary>
    size_t tile_grid_index = 0;

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
    /// Determines whether to save the next best value
    /// </summary>
    bool save_optim_network_flag = true;

    /// <summary>
    /// Define sthe current game mode
    /// </summary>
    GameMode current_mode = GameMode::OPTIM;

    /// <summary>
    /// Defines the width of the tile grids
    /// </summary>
    const static size_t tile_grid_width;

    /// <summary>
    /// Defines the height of the tile grids
    /// </summary>
    const static size_t tile_grid_height;
};

#endif
