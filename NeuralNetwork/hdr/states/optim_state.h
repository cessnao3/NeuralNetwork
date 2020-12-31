#ifndef __IO_OPTIM_STATE__
#define __IO_OPTIM_STATE

#include "neural/net.h"
#include "optim/genetic.h"

class OptimState
{
    //friend class GameState;

public:
    OptimState(const size_t sensor_count, const size_t num_outputs) :
        net_optim(NeuralNetwork::from_layers({ sensor_count, 3 * sensor_count, num_outputs })),
        net_best(net_optim),
        optim(num_population, net_optim.get_links().size())
    {
        optim.init_population();
    }

    bool step()
    {
        bool reset_car = false;

        if (update_population)
        {
            // Set the gain values to the current population design variables
            const std::vector<double>& current_desvars = optim.get_population(current_population_index);
            for (size_t i = 0; i < optim.design_variable_size(); ++i)
            {
                net_optim.get_links()[i].set_gain(current_desvars[i]);
            }

            // Reset State
            update_population = false;
            reset_car = true;
        }

        return reset_car;
    }

    bool update_best(const double distance, const double average_speed)
    {
        optim.set_result(current_population_index, distance * average_speed);

        if (distance > distance_best)
        {
            distance_best = distance;
            net_best = net_optim;
            num_best_update_count += 1;
            return true;
        }
        else
        {
            return false;
        }
    }

    void next_network()
    {
        current_population_index += 1;
        if (current_population_index >= num_population)
        {
            optim.update_population();
            current_generation += 1;
            current_population_index = 0;
        }
        set_update_population();
    }

    double get_best_distance() const
    {
        return distance_best;
    }

    size_t get_best_generation() const
    {
        return generation_best;
    }

    NeuralNetwork* get_best_network()
    {
        return &net_best;
    }

    NeuralNetwork* get_optim_network()
    {
        return &net_optim;
    }

    size_t get_current_generation() const
    {
        return current_generation;
    }

    size_t get_current_index() const
    {
        return current_population_index;
    }

    void set_update_population()
    {
        update_population = true;
    }

    size_t get_num_best_update_counts() const
    {
        return num_best_update_count;
    }

private:
    // Make Static
    const size_t num_population = 50;

private:
    NeuralNetwork net_optim;
    NeuralNetwork net_best;

    GeneticOptim optim;

    double distance_best = 0.0;
    size_t generation_best = 0;

    size_t current_generation = 0;
    size_t current_population_index = 0;

    size_t num_best_update_count = 0;

    bool update_population = true;
};

#endif
