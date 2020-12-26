#ifndef __IO_NEURAL_LAYER__
#define __IO_NEURAL_LAYER__

#include <vector>
#include <algorithm>

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
    /// <param name="node_id">Defines the layer to add</param>
    bool add_node(size_t node_id)
    {
        if (std::find(node_ids.begin(), node_ids.end(), node_id) == node_ids.end())
        {
            node_ids.push_back(node_id);
            return true;
        }
        else
        {
            return false;
        }
    }

    bool add_link(size_t link_id)
    {
        if (std::find(link_ids.begin(), link_ids.end(), link_id) == link_ids.end())
        {
            link_ids.push_back(link_id);
            return true;
        }
        else
        {
            return false;
        }
    }

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
