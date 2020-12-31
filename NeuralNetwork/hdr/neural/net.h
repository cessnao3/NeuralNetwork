#ifndef __IO_NEURAL_NET__
#define __IO_NEURAL_NET__

#include <vector>

#include <string>

#include "neural/layer.h"
#include "neural/link.h"
#include "neural/node.h"

/// <summary>
/// The overall neural network
/// </summary>
class NeuralNetwork
{
public:
    /// <summary>
    /// Creates a NeuralNetwork based on the number of layers provided
    /// Will through either an std::invalid_argument for invalid (empty
    /// or zero layer size) or a neural_exception (layer/node error)
    /// on error
    /// </summary>
    /// <param name="layers">a vector of layers, with each size_t containing the
    /// number of nodes to put in that layer. The first layer is the input,
    /// and the end layer is the output</param>
    /// <returns>NeuralNetwork</returns>
    static NeuralNetwork from_layers(const std::vector<size_t>& layers);

public:
    /// <summary>
    /// Steps the neural network to calculate the new outputs from the given inputs
    /// </summary>
    /// <returns>true if successful</returns>
    bool update_network_design();

    /// <summary>
    /// Adds a new layer to the network. Will not allow a new layer
    /// to be added if the previous layer is empty
    /// </summary>
    /// <returns>true if successful</returns>
    bool add_layer();

    /// <summary>
    /// Adds a new node to the last layer, and creates links with all
    /// nodes in the previous layer.
    /// </summary>
    /// <returns>true if successful</returns>
    bool add_node();

    /// <summary>
    /// Sets the given input to a provided value
    /// </summary>
    /// <param name="index">the input index to set</param>
    /// <param name="value">the value to set</param>
    /// <returns>true if successful</returns>
    bool set_input(
        const size_t index,
        const double value);

    /// <summary>
    /// Obtains the given output for a value
    /// </summary>
    /// <param name="index">the output index to get</param>
    /// <param name="output">the output parameter to use</param>
    /// <returns>true if successful</returns>
    bool get_output(
        const size_t index,
        double& output);

    /// <summary>
    /// Provides the number of inputs
    /// </summary>
    /// <returns>the number of inputs</returns>
    size_t size_inputs() const;

    /// <summary>
    /// Provides the number of outputs
    /// </summary>
    /// <returns>the number of outputs</returns>
    size_t size_outputs() const;

    /// <summary>
    /// Obtains a vector of all links to be used in setting
    /// network parameters
    /// </summary>
    /// <returns>a mutable vector of links</returns>
    std::vector<NeuralLink>& get_links();

    /// <summary>
    /// Provides some simple text output of the current status
    /// </summary>
    /// <returns>a status string for the inputs and outputs</returns>
    std::string get_status() const;

    /// <summary>
    /// Provides the string representation of the current network
    /// </summary>
    /// <returns>string configuration representation</returns>
    std::string get_config() const;

    /// <summary>
    /// Provides a neural network from the given neural network configuration string
    /// Will throw an exception if the network is unable to be created
    /// </summary>
    /// <param name="config">the configuration string</param>
    /// <returns>the network associated with the given configuration</returns>
    static NeuralNetwork from_config(const std::string& config);

private:
    /// <summary>
    /// The neural network layers, to be evaluated from
    /// index 0 (input) to the ending index (output)
    /// </summary>
    std::vector<NeuralLayer> layers;

    /// <summary>
    /// The neural network nodes
    /// </summary>
    std::vector<NeuralNode> nodes;

    /// <summary>
    /// The neural network links
    /// </summary>
    std::vector<NeuralLink> links;
};

#endif
