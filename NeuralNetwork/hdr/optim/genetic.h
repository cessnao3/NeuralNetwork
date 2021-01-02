#ifndef __IO_GENETIC_OPTIM__
#define __IO_GENETIC_OPTIM__

#include <vector>
#include <random>

/// <summary>
/// GeneticOptim provides a basic genetic optimization algorithm
/// for the provided design variables to maximize the fitness of the
/// objective function
/// </summary>
class GeneticOptim
{
protected:
    /// <summary>
    /// OptimStatus is a structure to contain information regarding the
    /// current fitness and design variables for a given design
    /// </summary>
    struct OptimStatus
    {
        /// <summary>
        /// Initializes the OptimStatus
        /// </summary>
        /// <param name="num_vars">the number of design variables to use</param>
        OptimStatus(const size_t num_vars);

        /// <summary>
        /// Resets the fitness score of the design
        /// </summary>
        void reset();

        /// <summary>
        /// Provides the design variable values
        /// </summary>
        std::vector<double> design_variables;

        /// <summary>
        /// Provides the fitness score of the design
        /// </summary>
        double fitness;
    };

public:
    /// <summary>
    /// Constructs the Genetic Optimization algorithm
    /// </summary>
    /// <param name="num_population">the size of the population to use</param>
    /// <param name="num_des_var">the number of design variables to have</param>
    GeneticOptim(
        const size_t num_designs,
        const size_t num_des_var);

    /// <summary>
    /// Obtain the design variables for a given design
    /// </summary>
    /// <param name="i">the design index</param>
    /// <returns>the design variables</returns>
    const std::vector<double>& get_design(const size_t i);

    /// <summary>
    /// Provides the count of the current designs
    /// </summary>
    /// <returns>the number of designs in the optimization algorithm</returns>
    const size_t design_count() const;

    /// <summary>
    /// Sets the fitness score for a given design
    /// </summary>
    /// <param name="ind">the design index to check</param>
    /// <param name="val">the fitness value to assign</param>
    void set_design_fitness(const size_t ind, const double val);

    /// <summary>
    /// Updates the designs and steps the genetic algorithm once all new fitness
    /// values are assigned, resetting the fitness scores to zero for each new design
    /// </summary>
    void update_design();

    /// <summary>
    /// Provides the count of the design variables
    /// </summary>
    /// <returns>the number of design variables in the optimization</returns>
    size_t design_variable_count() const;

    /// <summary>
    /// Provides the current generation count for the optimization steps
    /// </summary>
    /// <returns>the number of times update_population has been called</returns>
    size_t get_generation() const;

    /// <summary>
    /// Initializes the population to random values within the upper and lower bound values
    /// </summary>
    void init_population();

    /// <summary>
    /// Innitializes the population to random values roughtly surrounding
    /// the given design varaibles
    /// </summary>
    /// <param name="other">the design variables to target around</param>
    void init_population(const std::vector<double>& other);

protected:
    /// <summary>
    /// Obtains a random number to assign to a design variable between the
    /// provided lower and upper bounds
    /// </summary>
    /// <returns>uniform random number between the upper and lower bounds</returns>
    double get_random();

    /// <summary>
    /// Constrains the input value between the upper and lower bound
    /// </summary>
    /// <param name="val">value to constrain</param>
    /// <returns>constrained value</returns>
    double constrain_value(const double val) const;

protected:
    /// <summary>
    /// A vector to store the optimization status for each design
    /// </summary>
    std::vector<OptimStatus> designs;

    /// <summary>
    /// The random number generator
    /// </summary>
    std::default_random_engine generator;

    /// <summary>
    /// The real distribution between 0 and 1 to use for generating random numbers
    /// </summary>
    std::uniform_real_distribution<double> distribution;

    /// <summary>
    /// The integer distribution to use to find a random index value
    /// </summary>
    std::uniform_int_distribution<size_t> index_distribution;

    /// <summary>
    /// Storage for the number of design variables in a solution
    /// </summary>
    size_t num_des_var;

    /// <summary>
    /// Storage for the current generation value for the solution
    /// </summary>
    size_t current_generation;

    /// <summary>
    /// The lower-bound of the design space
    /// </summary>
    static const double lower_bound;

    /// <summary>
    /// The upper-bound of the design space
    /// </summary>
    static const double upper_bound;
};

#endif
