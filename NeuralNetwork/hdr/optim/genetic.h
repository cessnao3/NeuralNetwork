#ifndef __IO_GENETIC_OPTIM__
#define __IO_GENETIC_OPTIM__

#include <vector>
#include <random>
#include <stdexcept>

class GeneticOptim
{
protected:
    struct OptimStatus
    {
        OptimStatus(const size_t num_vars) :
            design_variables(num_vars, 0.0),
            fitness(0.0)
        {
            // Empty Constructor
        }

        void reset()
        {
            fitness = 0.0;
        }

        std::vector<double> design_variables;
        double fitness;
    };

public:
    GeneticOptim(const size_t num_population, const size_t num_des_var) :
        population(num_population, OptimStatus(num_des_var)),
        distribution(0.0, 1.0),
        index_distribution(0, num_population - 1),
        generator(0),
        num_des_var(num_des_var),
        generation(0)
    {
        if (num_population == 0 || num_des_var == 0)
        {
            throw std::invalid_argument("population and design variables must be positive");
        }
    }

    void init_population()
    {
        for (size_t i = 0; i < population.size(); ++i)
        {
            for (size_t j = 0; j < population[i].design_variables.size(); ++j)
            {
                population[i].design_variables[j] = get_random();
            }
        }
    }

    const std::vector<double>& get_population(const size_t i)
    {
        return population[i].design_variables;
    }

    const size_t population_size() const
    {
        return population.size();
    }

    void set_result(const size_t ind, const double val)
    {
        population[ind].fitness = val;
    }

    void update_population()
    {
        std::vector<size_t> combination_group(population.size() * 2, 0);
        std::vector<OptimStatus> new_population(population.size(), OptimStatus(num_des_var));

        double min_val = 0.0;
        double max_val = 0.0;
        size_t max_i = 0;

        for (size_t i = 0; i < population.size(); ++i)
        {
            const double fv = population[i].fitness;
            if (i == 0)
            {
                min_val = fv;
                max_val = fv;
                max_i = i;
            }
            else
            {
                if (fv > max_val)
                {
                    max_i = i;
                }

                min_val = std::min(fv, min_val);
                max_val = std::max(fv, max_val);
            }
        }

        // Determine the indices to add to the combination group
        std::uniform_int_distribution<size_t> index_dist;

        for (size_t i = 0; i < population.size(); ++i)
        {
            for (size_t j = 0; j < 2; ++j)
            {
                size_t i2 = i;

                while (i == i2)
                {
                    i2 = index_distribution(generator);
                }

                combination_group[2 * i + j] = (population[i].fitness > population[i2].fitness) ? i : i2;
            }
        }

        // Perform the sample combinations
        for (size_t i = 0; i < population.size(); ++i)
        {
            const double w1 = 0.75;
            const double w2 = 0.25;

            const double ind1 = combination_group[2 * i];
            const double ind2 = combination_group[2 * i + 1];

            const OptimStatus& o1 = population[ind1];
            const OptimStatus& o2 = population[ind2];

            const OptimStatus& val_max = (o1.fitness > o2.fitness) ? o1 : o2;
            const OptimStatus& val_min = (o1.fitness > o2.fitness) ? o2 : o1;

            OptimStatus& child = new_population[i];

            for (size_t j = 0; j < population[i].design_variables.size(); ++j)
            {
                double& desvar = child.design_variables[j];
                desvar = w1 * val_max.design_variables[j] + w2 * val_min.design_variables[j];
                desvar += get_random() * 0.1;
                desvar = std::min(upper_bound, std::max(lower_bound, desvar));
            }
        }

        // Reset the status values
        population = new_population;
        generation += 1;
    }

    size_t design_variable_size() const
    {
        return num_des_var;
    }

    size_t get_generation() const
    {
        return generation;
    }

protected:
    double get_random()
    {
        return lower_bound + distribution(generator) * (upper_bound - lower_bound);
    }

protected:
    std::vector<OptimStatus> population;

    std::default_random_engine generator;
    std::uniform_real_distribution<double> distribution;
    std::uniform_int_distribution<size_t> index_distribution;

    const size_t num_des_var;

    const double lower_bound = -1;
    const double upper_bound = 1;

    size_t generation;
};

#endif
