#ifndef __IO_NEURAL_LAYER__
#define __IO_NEURAL_LAYER__

#include <vector>
#include <string>

/// <summary>
/// Defines a Neural Network layer
/// </summary>
class NeuralLayer
{
    friend class NeuralNetwork;

public:
    /// <summary>
    /// Adds a node to the layer
    /// </summary>
    /// <param name="node_id">the node ID to add</param>
    /// <returns>true if successful</returns>
    bool add_node(size_t node_id);

    /// <summary>
    /// Adds a link to the layer
    /// </summary>
    /// <param name="link_id">the link ID to add</param>
    /// <returns>true if successful</returns>
    bool add_link(size_t link_id);

private:
    /// <summary>
    /// The list of node IDs present within the layer
    /// </summary>
    std::vector<size_t> node_ids;

    /// <summary>
    /// The list of link IDs associated going into nodes for the current vector
    /// </summary>
    std::vector<size_t> link_ids;
};

#endif
