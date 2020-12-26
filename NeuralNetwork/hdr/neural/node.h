#ifndef __IO_NEURAL_NODE__
#define __IO_NEURAL_NODE__

#include <vector>

/// <summary>
/// Defines a given Neural network node
/// </summary>
class NeuralNode
{
    friend class NeuralNetwork;

public:
    /// <summary>
    /// Initializes the neural node parameter
    /// </summary>
    /// <param name="node_id">the node ID for the current node</param>
    NeuralNode(const size_t node_id);

    /// <summary>
    /// Adds a node link ID to the given node
    /// </summary>
    /// <param name="link_id">the link ID to add to the node</param>
    void add_link_id(const size_t link_id);

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

private:
    /// <summary>
    /// Provides the associated links for the givne link ID
    /// </summary>
    std::vector<size_t> link_ids;

    /// <summary>
    /// The current node ID
    /// </summary>
    size_t node_id;

    /// <summary>
    /// Contains the current value associated with the class
    /// </summary>
    double value;
};

#endif
