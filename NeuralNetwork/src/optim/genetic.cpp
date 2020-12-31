#include "optim/genetic.h"

#include <stdexcept>

const double GeneticOptim::lower_bound = -1.0;
const double GeneticOptim::upper_bound = 1.0;

GeneticOptim::OptimStatus::OptimStatus(const size_t num_vars) :
    design_variables(num_vars, 0.0),
    fitness(0.0)
{
    // Empty Constructor
}

void GeneticOptim::OptimStatus::reset()
{
    fitness = 0.0;
}

GeneticOptim::GeneticOptim(const size_t num_designs, const size_t num_des_var) :
    designs(num_designs, OptimStatus(num_des_var)),
    distribution(0.0, 1.0),
    index_distribution(0, num_designs - 1),
    generator(0),
    num_des_var(num_des_var),
    current_generation(0)
{
    if (num_designs == 0 || num_des_var == 0)
    {
        throw std::invalid_argument("population and design variables must be positive");
    }

    for (size_t i = 0; i < designs.size(); ++i)
    {
        for (size_t j = 0; j < designs[i].design_variables.size(); ++j)
        {
            designs[i].design_variables[j] = get_random();
        }
    }
}

const std::vector<double>& GeneticOptim::get_design(const size_t i)
{
    if (i >= designs.size())
    {
        throw std::out_of_range("design index out of range for getting variables");
    }
    else
    {
        return designs[i].design_variables;
    }
}

const size_t GeneticOptim::design_count() const
{
    return designs.size();
}

void GeneticOptim::set_design_fitness(const size_t ind, const double val)
{
    if (ind >= designs.size())
    {
        throw std::out_of_range("design index out of range for setting fitness");
    }
    else
    {
        designs[ind].fitness = val;
    }
}

void GeneticOptim::update_design()
{
    std::vector<size_t> combination_group(designs.size() * 2, 0);
    std::vector<OptimStatus> new_population(designs.size(), OptimStatus(num_des_var));

    double min_val = 0.0;
    double max_val = 0.0;
    size_t max_i = 0;

    for (size_t i = 0; i < designs.size(); ++i)
    {
        const double fv = designs[i].fitness;
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

    for (size_t i = 0; i < designs.size(); ++i)
    {
        for (size_t j = 0; j < 2; ++j)
        {
            size_t i2 = i;

            while (i == i2)
            {
                i2 = index_distribution(generator);
            }

            combination_group[2 * i + j] = (designs[i].fitness > designs[i2].fitness) ? i : i2;
        }
    }

    // Perform the sample combinations
    for (size_t i = 0; i < designs.size(); ++i)
    {
        const double w1 = 0.75;
        const double w2 = 1.0 - w1;

        const size_t ind1 = combination_group[2 * i];
        const size_t ind2 = combination_group[2 * i + 1];

        const OptimStatus& o1 = designs[ind1];
        const OptimStatus& o2 = designs[ind2];

        const OptimStatus& val_max = (o1.fitness > o2.fitness) ? o1 : o2;
        const OptimStatus& val_min = (o1.fitness > o2.fitness) ? o2 : o1;

        OptimStatus& child = new_population[i];

        for (size_t j = 0; j < designs[i].design_variables.size(); ++j)
        {
            double& desvar = child.design_variables[j];
            desvar = w1 * val_max.design_variables[j] + w2 * val_min.design_variables[j];
            desvar += get_random() * 0.1;
            desvar = std::min(upper_bound, std::max(lower_bound, desvar));
        }
    }

    // Reset the status values
    designs = new_population;
    current_generation += 1;
}

size_t GeneticOptim::design_variable_count() const
{
    return num_des_var;
}

size_t GeneticOptim::get_generation() const
{
    return current_generation;
}

double GeneticOptim::get_random()
{
    return lower_bound + distribution(generator) * (upper_bound - lower_bound);
}
