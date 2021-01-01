#include "neural/net.h"

#include <stdexcept>
#include <sstream>

#include "neural/neural_exception.h"

NeuralNetwork NeuralNetwork::from_layers(const std::vector<size_t>& layers)
{
    // Ensure that layers were added
    if (layers.size() == 0)
    {
        throw std::invalid_argument("layers vector must have at least one layer");
    }

    // Create the net
    NeuralNetwork net;

    // Iterate layers
    for (size_t i = 0; i < layers.size(); ++i)
    {
        // Check that size values are provided
        if (layers[i] == 0)
        {
            std::ostringstream ss;
            ss << "layer " << static_cast<int>(i) << " must have at least one node";
            throw std::invalid_argument(ss.str());
        }

        // Try to add the layer
        if (!net.add_layer())
        {
            std::ostringstream ss;
            ss << "unable to add layer " << static_cast<int>(i);
            throw neural_exception(ss.str());
        }

        // Add each node value
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

    // Return the net
    return net;
}

bool NeuralNetwork::update_network_design()
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

        // Update to normalize between 0 and 1
        for (size_t j = 0; j < layer.node_ids.size(); ++j)
        {
            NeuralNode& n = nodes[layer.node_ids[j]];
            n.set_value(n.get_value() / layers[i - 1].node_ids.size());
        }
    }

    // Return true if success
    return true;
}

bool NeuralNetwork::add_layer()
{
    // Attempt to add the layer
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
    // Ensure that layers are provided
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
            // Extract the previous layer
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
    // Ensure that the size is consistent
    if (layers.size() > 0 && layers.front().node_ids.size() > index)
    {
        // Define the node index
        const size_t node_idx = layers.front().node_ids[index];

        // Set the value and return success if the node index is within the nodes
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
    // Ensure that the size is consistent
    if (layers.size() > 0 && layers.back().node_ids.size() > index)
    {
        // Define the node index
        const size_t node_idx = layers.back().node_ids[index];

        // Set the value and return success if the node index is within the nodes
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
    // Return the input value size, or zero on failure
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
    // Return the output value size, or zero on failure
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
    // Define the string stream
    std::ostringstream ss;

    // Provide input values
    ss << "Inputs:" << std::endl;

    for (size_t i = 0; i < size_inputs(); ++i)
    {
        const NeuralNode& n = nodes[layers.front().node_ids[i]];
        ss << "  " << static_cast<int>(i) << ": " << n.get_value() << std::endl;
    }

    // Provide output values
    ss << "Outputs:" << std::endl;

    for (size_t i = 0; i < size_outputs(); ++i)
    {
        const NeuralNode& n = nodes[layers.back().node_ids[i]];
        ss << "  " << static_cast<int>(i) << ": " << n.get_value() << std::endl;
    }

    // Return the resulting string
    return ss.str();
}

std::string NeuralNetwork::get_config() const
{
    // Define the output string
    std::ostringstream output;

    // Write the number of nodes
    output << nodes.size() << std::endl;

    // Write the number of links, and then the status value for each link
    output << links.size() << std::endl;
    for (size_t i = 0; i < links.size(); ++i)
    {
        const NeuralLink& l = links[i];
        output << l.from_node_id() << ">" << l.to_node_id() << "=" << l.get_gain() << std::endl;
    }

    // Write the number of layers
    output << layers.size() << std::endl;
    for (size_t i = 0; i < layers.size(); ++i)
    {
        const NeuralLayer& l = layers[i];
        output << l.node_ids.size() << std::endl;
        for (size_t j = 0; j < l.node_ids.size(); ++j)
        {
            output << l.node_ids[j] << std::endl;
        }
        output << l.link_ids.size() << std::endl;
        for (size_t j = 0; j < l.link_ids.size(); ++j)
        {
            output << l.link_ids[j] << std::endl;
        }
    }

    output << 8080 << std::endl;

    // Return combined values
    return output.str();
}

#include <iostream>

NeuralNetwork NeuralNetwork::from_config(const std::string& config)
{
    // Define the input reader
    std::istringstream input(config);
    NeuralNetwork net;

    // Number of nodes
    size_t num_nodes;
    input >> num_nodes;

    if (!input || num_nodes == 0)
    {
        throw std::invalid_argument("cannot provide zero nodes as input to network");
    }

    for (size_t i = 0; i < num_nodes; ++i)
    {
        net.nodes.push_back(NeuralNode(i));
    }

    // Read in the links
    size_t num_links;
    input >> num_links;

    if (!input || num_links == 0)
    {
        throw std::invalid_argument("cannot provide zero links as input to network");
    }

    for (size_t i = 0; i < num_links; ++i)
    {
        std::string link_str;
        input >> link_str;

        const size_t c1 = link_str.find('>');
        const size_t c2 = link_str.find('=');

        if (c1 == std::string::npos || c2 == std::string::npos)
        {
            throw std::invalid_argument("unable to find either > or = delimiter in link string " + link_str);
        }

        const std::string from_id_str = link_str.substr(0, c1);
        const std::string to_id_str = link_str.substr(c1 + 1, c2 - c1 - 1);
        const std::string gain_str = link_str.substr(c2 + 1);

        const size_t from_id = std::stol(from_id_str);
        const size_t to_id = std::stol(to_id_str);
        const double gain = std::stod(gain_str);

        net.links.push_back(NeuralLink(net.links.size(), from_id, to_id, gain));
    }

    // Read in the layers
    size_t num_layers;
    input >> num_layers;

    if (!input || num_layers == 0)
    {
        throw std::invalid_argument("cannot provide zero layers as input to network");
    }

    for (size_t i = 0; i < num_layers; ++i)
    {
        NeuralLayer layer;

        {
            size_t num_node_vals;
            input >> num_node_vals;

            if (!input || num_node_vals == 0)
            {
                throw std::invalid_argument("network configuration layer must have valid node size");
            }

            for (size_t j = 0; j < num_node_vals; ++j)
            {
                size_t nid;
                input >> nid;

                const bool add_result = layer.add_node(nid);

                if (!input || !add_result)
                {
                    throw std::invalid_argument("unable to read network configuration layer node ID value");
                }
            }
        }

        {
            size_t num_link_vals = 0;
            input >> num_link_vals;

            if (!input || (net.layers.size() > 0 && num_link_vals == 0) || (net.layers.size() == 0 && num_link_vals != 0))
            {
                throw std::invalid_argument("network congiraution layer must have valid link size");
            }

            for (size_t j = 0; j < num_link_vals; ++j)
            {
                size_t lid;
                input >> lid;

                if (!input || !layer.add_link(lid))
                {
                    throw std::invalid_argument("unable to read network configuration layer link ID value");
                }
            }
        }

        net.layers.push_back(layer);
    }

    // Check that the ending value is 0
    size_t end_val = 10;
    input >> end_val;

    if (!input || end_val != 8080)
    {
        throw std::invalid_argument("configuration end value is incorrect");
    }

    // Return the network
    return net;
}
