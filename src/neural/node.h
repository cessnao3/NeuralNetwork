#ifndef __IO_NEURAL_NODE__
#define __IO_NEURAL_NODE__

#include <vector>
#include <cstddef>

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
    NeuralNode(const size_t node_id, const bool bias_node);

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
    /// Returns whether the current node is a bias node
    /// </summary>
    /// <returns>true if the node is a bias node</returns>
    bool is_bias_node() const;

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
    /// Defines whether the current node is a bias node
    /// </summary>
    bool bias_node;
};

#endif
