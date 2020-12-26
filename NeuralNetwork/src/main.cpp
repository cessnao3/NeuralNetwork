#include <iostream>

#include "neuralnet.h"

int main()
{
    // Define the Neural Network
    NeuralNetwork net;

    // Define the layer sizes
    std::vector<size_t> layer_sizes = { 5, 7, 4 };

    // Create the layer
    try
    {
        net = NeuralNetwork::from_layers(layer_sizes);
    }
    catch (neural_exception& e)
    {
        std::cerr << "Neural Exception: " << e.what() << std::endl;
        return 1;
    }

    // Set Links to have unity gain
    std::vector<NeuralLink>& links = net.get_links();
    for (size_t i = 0; i < links.size(); ++i)
    {
        links[i].set_gain(1.0);
    }

    // Set an initial input
    net.set_input(0, 1.0);

    // Define Success
    std::cout << "Neural Network Successfully Created" << std::endl;

    // Define an initial output
    std::cout << "Step 0" << std::endl << net.get_status() << std::endl;

    // Step
    net.step();

    // Print Output
    std::cout << "Step 1" << std::endl << net.get_status() << std::endl;

    // Return Success
    return 0;
}
