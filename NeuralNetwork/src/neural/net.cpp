#include "neural/net.h"

#include <stdexcept>
#include <sstream>

#include "neural/neural_exception.h"

NeuralNetwork NeuralNetwork::from_layers(const std::vector<size_t>& layers)
{
    if (layers.size() == 0)
    {
        throw std::invalid_argument("layers vector must have at least one layer");
    }

    NeuralNetwork net;

    for (size_t i = 0; i < layers.size(); ++i)
    {
        if (layers[i] == 0)
        {
            std::ostringstream ss;
            ss << "layer " << static_cast<int>(i) << " must have at least one node";
            throw std::invalid_argument(ss.str());
        }

        if (!net.add_layer())
        {
            std::ostringstream ss;
            ss << "unable to add layer " << static_cast<int>(i);
            throw neural_exception(ss.str());
        }

        for (size_t j = 0; j < layers[i]; ++j)
        {
            if (!net.add_node())
            {
                std::ostringstream ss;
                ss << "unable to add node " << static_cast<int>(j) << " for layer " << static_cast<int>(i);
                throw  neural_exception(ss.str());
            }
        }
    }

    return net;
}

bool NeuralNetwork::step()
{
    // Return false if the layer size is less than two
    //   for no layers, or input layer is also output layer
    if (layers.size() < 2)
    {
        return false;
    }

    // Iterate over each layer, skipping the input layer
    for (size_t i = 1; i < layers.size(); ++i)
    {
        // Extract the layer
        NeuralLayer& layer = layers[i];

        // Reset each node value to zero
        for (size_t j = 0; j < layer.node_ids.size(); ++j)
        {
            nodes[layer.node_ids[j]].set_value(0.0);
        }

        // Iterate over each link in the layer
        for (size_t j = 0; j < layer.link_ids.size(); ++j)
        {
            // Extract the link
            const NeuralLink& link = links[layer.link_ids[j]];

            // Extract the to/from values
            const NeuralNode& from = nodes[link.from_node_id()];
            NeuralNode& to = nodes[link.to_node_id()];

            // Update the value
            to.set_value(to.get_value() + from.get_value() * link.get_gain());
        }
    }

    return true;
}

bool NeuralNetwork::add_layer()
{
    if (layers.size() == 0 || layers.back().node_ids.size() > 0)
    {
        layers.push_back(NeuralLayer());
        return true;
    }
    else
    {
        return false;
    }
}

bool NeuralNetwork::add_node()
{
    if (layers.size() == 0)
    {
        return false;
    }
    else
    {
        // Determine if there's a previous layer to use
        NeuralLayer* prev = nullptr;
        if (layers.size() > 1)
        {
            prev = &layers[layers.size() - 2];

            // Ensure that the previous layer has nodes
            if (prev->node_ids.size() == 0)
            {
                return false;
            }
        }

        // Define the new node
        NeuralNode n(nodes.size());
        nodes.push_back(n);

        // Add the node ID to the current layer
        layers.back().add_node(n.node_id);

        // If a previous layer, add links from each of the nodes to this new node
        if (prev != nullptr)
        {
            for (size_t i = 0; i < prev->node_ids.size(); ++i)
            {
                NeuralLink ln(links.size(), prev->node_ids[i], n.node_id);
                links.push_back(ln);
                layers.back().add_link(ln.get_id());
            }
        }

        // Return success
        return true;
    }
}

bool NeuralNetwork::set_input(const size_t index, const double value)
{
    if (layers.size() > 0 && layers.front().node_ids.size() > index)
    {
        const size_t node_idx = layers.front().node_ids[index];
        if (nodes.size() > node_idx)
        {
            nodes[node_idx].set_value(value);
            return true;
        }
        else
        {
            return false;
        }
    }
    else
    {
        return false;
    }
}

bool NeuralNetwork::get_output(const size_t index, double& output)
{
    if (layers.size() > 0 && layers.back().node_ids.size() > index)
    {
        const size_t node_idx = layers.back().node_ids[index];
        if (nodes.size() > node_idx)
        {
            output = nodes[node_idx].get_value();
            return true;
        }
        else
        {
            return false;
        }
    }
    else
    {
        return false;
    }
}

size_t NeuralNetwork::size_inputs() const
{
    if (layers.size() > 0)
    {
        return layers.front().node_ids.size();
    }
    else
    {
        return 0;
    }
}

size_t NeuralNetwork::size_outputs() const
{
    if (layers.size() > 0)
    {
        return layers.back().node_ids.size();
    }
    else
    {
        return 0;
    }
}

std::vector<NeuralLink>& NeuralNetwork::get_links()
{
    return links;
}

std::string NeuralNetwork::get_status() const
{
    std::ostringstream ss;
    ss << "Inputs:" << std::endl;

    for (size_t i = 0; i < size_inputs(); ++i)
    {
        const NeuralNode& n = nodes[layers.front().node_ids[i]];
        ss << "  " << static_cast<int>(i) << ": " << n.get_value() << std::endl;
    }

    ss << "Outputs:" << std::endl;

    for (size_t i = 0; i < size_outputs(); ++i)
    {
        const NeuralNode& n = nodes[layers.back().node_ids[i]];
        ss << "  " << static_cast<int>(i) << ": " << n.get_value() << std::endl;
    }

    return ss.str();
}
