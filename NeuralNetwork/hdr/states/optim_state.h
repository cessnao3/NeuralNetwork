#ifndef __IO_OPTIM_STATE__
#define __IO_OPTIM_STATE

#include "neural/net.h"
#include "optim/genetic.h"

/// <summary>
/// OptimState provides the overall state of the optimization
/// </summary>
class OptimState
{
    //friend class GameState;

public:
    /// <summary>
    /// Constructs an object to track the overall optimization state
    /// </summary>
    /// <param name="sensor_count">the number of inputs to set</param>
    /// <param name="num_outputs">the number of outputs to set</param>
    OptimState(const size_t num_inputs, const size_t num_outputs);

    /// <summary>
    /// Steps the initial setup for the optimization to update the design
    /// currently contained in the neural network
    /// </summary>
    /// <returns>true if the car should be reset</returns>
    bool update_network_design();

    /// <summary>
    /// Determines whether the best design should be updated based on the input
    /// trial result parameters
    /// </summary>
    /// <param name="distance">the distance traveled</param>
    /// <returns>true if the best design has been updated</returns>
    bool check_update_best_design(const double distance);

    /// <summary>
    /// Steps to the next network, updating the optimization design variables as necessary
    /// if all designs have been set and accounted for
    /// </summary>
    void step_to_next_design();

    /// <summary>
    /// Provides the best distance so far
    /// </summary>
    /// <returns>the distance with the best fitness</returns>
    double get_best_distance() const;

    /// <summary>
    /// Provides the neural network currently used in optimization
    /// </summary>
    /// <returns>the neural network used in optimization for the current generation/design</returns>
    NeuralNetwork* get_optim_network();

    /// <summary>
    /// Provides the neural network with the best fitness
    /// </summary>
    /// <returns>the neural network with the best fitness</returns>
    NeuralNetwork* get_best_network();

    /// <summary>
    /// Provides the current generation
    /// </summary>
    /// <returns>the current generation count</returns>
    size_t get_current_generation() const;

    /// <summary>
    /// Provides the generation where the best value so far has been found
    /// </summary>
    /// <returns>the generation with the best fitness so far</returns>
    size_t get_best_generation() const;

    /// <summary>
    /// Provides the current design index within a generation
    /// </summary>
    /// <returns>the current design index</returns>
    size_t get_current_design_index() const;

    /// <summary>
    /// Sets the flag to indicate that the design will need to be updated on next init call
    /// </summary>
    void set_update_design_flag();

    /// <summary>
    /// Provides the number of times that the best network was updated
    /// </summary>
    /// <returns>the number of calls to check_update_best_design that return true</returns>
    size_t get_num_best_update_counts() const;

private:
    /// <summary>
    /// The overall number of designs to use in the optimization within
    /// each generation
    /// </summary>
    static const size_t num_designs;

private:
    /// <summary>
    /// The neural network used in optimization
    /// </summary>
    NeuralNetwork net_optim;

    /// <summary>
    /// The neural network with the best found so far fitness
    /// </summary>
    NeuralNetwork net_best;

    /// <summary>
    /// The optimization algorithm
    /// </summary>
    GeneticOptim optim;

    /// <summary>
    /// The best distance so far
    /// </summary>
    double distance_best = 0.0;

    /// <summary>
    /// The generation with the best distance so far
    /// </summary>
    size_t generation_best = 0;

    /// <summary>
    /// The current generation for optimization
    /// </summary>
    size_t current_generation = 0;

    /// <summary>
    /// The current design index within a generation
    /// </summary>
    size_t current_design_index = 0;

    /// <summary>
    /// The number of updates to the best index
    /// </summary>
    size_t num_best_update_count = 0;

    /// <summary>
    /// Flag to indicate that the design should be updated
    /// </summary>
    bool update_design = true;
};

#endif
