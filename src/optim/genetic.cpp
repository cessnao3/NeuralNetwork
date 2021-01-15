#include "optim/genetic.h"

#include <stdexcept>

const double GeneticOptim::lower_bound = 0.0;
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

GeneticOptim::GeneticOptim(
    const size_t num_designs,
    const size_t num_des_var) :
    designs(num_designs, OptimStatus(num_des_var)),
    generator(0),
    distribution(lower_bound, upper_bound),
    mutation_distribution(-1.0, 1.0),
    index_distribution(0, num_designs - 1),
    num_des_var(num_des_var),
    current_generation(0)
{
    // Check for valid inputs
    if (num_designs == 0 || num_des_var == 0)
    {
        throw std::invalid_argument("population and design variables must be positive");
    }

    // Define the random initial population values
    init_population();
}

void GeneticOptim::init_population()
{
    // Loop through each design
    for (size_t i = 0; i < designs.size(); ++i)
    {
        // Define a new random variable for each design variable
        for (size_t j = 0; j < designs[i].design_variables.size(); ++j)
        {
            designs[i].design_variables[j] = get_random();
        }
    }
}

void GeneticOptim::init_population(const std::vector<double>& other)
{
    if (other.size() != design_variable_count())
    {
        throw std::invalid_argument("input design variable size does not match between init vector and optimizer");
    }

    // Loop through each design
    for (size_t i = 0; i < designs.size(); ++i)
    {
        // Define a new design variable within 25% of the previous value, limiting the results
        for (size_t j = 0; j < designs[i].design_variables.size(); ++j)
        {
            designs[i].design_variables[j] = constrain_value(other[j] + 0.25 * get_mutation_random());
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

void GeneticOptim::update_designs()
{
    std::vector<size_t> combination_group(designs.size() * 2, 0);
    std::vector<OptimStatus> new_population(designs.size(), OptimStatus(num_des_var));

    double min_val = 0.0;
    double max_val = 0.0;

    for (size_t i = 0; i < designs.size(); ++i)
    {
        const double fv = designs[i].fitness;
        if (i == 0)
        {
            min_val = fv;
            max_val = fv;
        }
        else
        {
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
        // Define weights
        const double w1 = 0.5;
        const double w2 = 1.0 - w1;

        // Define the two value indices
        const size_t ind1 = combination_group[2 * i];
        const size_t ind2 = combination_group[2 * i + 1];

        // Extract the two designs
        const OptimStatus& o1 = designs[ind1];
        const OptimStatus& o2 = designs[ind2];

        // Determine the min and max value
        const OptimStatus& val_max = (o1.fitness > o2.fitness) ? o1 : o2;
        const OptimStatus& val_min = (o1.fitness > o2.fitness) ? o2 : o1;

        // Obtain the child to set into the new population result
        OptimStatus& child = new_population[i];

        // Update each design variable
        for (size_t j = 0; j < designs[i].design_variables.size(); ++j)
        {
            // Extract and update the design variable
            double& desvar = child.design_variables[j];
            desvar = w1 * val_max.design_variables[j] + w2 * val_min.design_variables[j];

            // Provide some mutation into the design variable
            const double mutation = get_mutation_random() * 0.05 * (upper_bound - lower_bound) / 2.0;
            desvar += mutation;

            // Limit the design variable to the upper and lower bounds
            desvar = constrain_value(desvar);
        }
    }

    // Reset the status values
    designs = new_population;
    current_generation += 1;
}

double GeneticOptim::constrain_value(const double val) const
{
    return std::min(upper_bound, std::max(lower_bound, val));
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
    return distribution(generator);
}

double GeneticOptim::get_mutation_random()
{
    return mutation_distribution(generator);
}
