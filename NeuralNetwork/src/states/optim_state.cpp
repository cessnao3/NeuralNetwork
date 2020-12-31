#include "states/optim_state.h"

const size_t OptimState::num_designs = 50;

OptimState::OptimState(
    const size_t num_inputs,
    const size_t num_outputs)
    :
    net_optim(NeuralNetwork::from_layers({ num_inputs, 3 * num_inputs, num_outputs })),
    net_best(net_optim),
    optim(num_designs, net_optim.get_links().size())
{
    // Empty Constructor
}

bool OptimState::update_network_design()
{
    // Boolean to track if the car should be reset
    bool reset_car = false;

    // Check if we need to update the design
    if (update_design)
    {
        // Set the gain values to the current network's design variables
        const std::vector<double>& current_desvars = optim.get_design(current_design_index);
        for (size_t i = 0; i < optim.design_variable_count(); ++i)
        {
            net_optim.get_links()[i].set_gain(current_desvars[i]);
        }

        // Reset State
        update_design = false;
        reset_car = true;
    }

    // Return the reset car flag
    return reset_car;
}

bool OptimState::check_update_best_design(const double distance)
{
    // Set the fitness score
    optim.set_design_fitness(current_design_index, distance);

    // Check if we shoudl update the values
    if (distance > distance_best)
    {
        // Update value and save if requested
        distance_best = distance;
        net_best = net_optim;
        num_best_update_count += 1;

        // Return true
        return true;
    }
    else
    {
        return false;
    }
}

void OptimState::step_to_next_design()
{
    // Increment the population index
    current_design_index += 1;

    // Check if we need to roll-over to the next generation
    if (current_design_index >= num_designs)
    {
        // If so, update the design variables and set parameters
        optim.update_design();
        current_generation += 1;
        current_design_index = 0;
    }

    // Set the flag to update the design variables on next update call
    set_update_design_flag();
}

double OptimState::get_best_distance() const
{
    return distance_best;
}

NeuralNetwork* OptimState::get_optim_network()
{
    return &net_optim;
}

NeuralNetwork* OptimState::get_best_network()
{
    return &net_best;
}

size_t OptimState::get_current_generation() const
{
    return current_generation;
}

size_t OptimState::get_best_generation() const
{
    return generation_best;
}

size_t OptimState::get_current_design_index() const
{
    return current_design_index;
}

void OptimState::set_update_design_flag()
{
    update_design = true;
}

size_t OptimState::get_num_best_update_counts() const
{
    return num_best_update_count;
}
