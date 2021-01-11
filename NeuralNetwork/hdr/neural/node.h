#ifndef __IO_NEURAL_NODE__
#define __IO_NEURAL_NODE__

#include <vector>

/// <summary>
/// Defines a given Neural network node
/// </summary>
class NeuralNode
{
public:
    /// <summary>
    /// Initializes the neural node parameter
    /// </summary>
    /// <param name="node_id">the node ID for the current node</param>
    /// <param name="bias">the input bias for the given node value</param>
    NeuralNode(const size_t node_id, const double bias);

    /// <summary>
    /// Provides the current value associated with the node
    /// </summary>
    /// <returns>the current value</returns>
    double get_value() const;

    /// <summary>
    /// Updates the value associated with the ndoe
    /// </summary>
    /// <param name="value">the new value to set</param>
    void set_value(const double value);

    /// <summary>
    /// Provides the node ID value
    /// </summary>
    /// <returns>the node ID value</returns>
    size_t get_node_id() const;

    /// <summary>
    /// Provides the current bias for the node
    /// </summary>
    /// <returns>bias value</returns>
    double get_bias() const;

    /// <summary>
    /// Sets the new bias value for the node between 0.0 and 1.0
    /// </summary>
    /// <param name="bias">the new bias value</param>
    bool set_bias(const double bias);

private:
    /// <summary>
    /// The current node ID
    /// </summary>
    size_t node_id;

    /// <summary>
    /// Contains the current value associated with the class
    /// </summary>
    double value;

    /// <summary>
    /// Contains the bias value used to determine activation of the given node
    /// </summary>
    double bias;
};

#endif
