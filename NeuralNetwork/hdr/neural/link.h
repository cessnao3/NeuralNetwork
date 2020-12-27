#ifndef __IO_NEURAL_LINK__
#define __IO_NEURAL_LINK__

#include <vector>

class NeuralLink
{
public:
    /// <summary>
    /// Constructs a new link with the provided gain
    /// </summary>
    NeuralLink(
        const size_t id,
        const size_t from_node_id,
        const size_t to_node_id,
        const double gain = 0.0);

    /// <summary>
    /// Sets the gain associated with the gain
    /// </summary>
    /// <param name="gain">the new gain to set</param>
    void set_gain(const double gain);

    /// <summary>
    /// Provides the current gain associated with the link
    /// </summary>
    /// <returns>the current gain</returns>
    double get_gain() const;

    /// <summary>
    /// Returns the from node ID
    /// </summary>
    /// <returns>the node the link is coming from</returns>
    size_t from_node_id() const;

    /// <summary>
    /// Returns the to node ID
    /// </summary>
    /// <returns>the node the link is going to</returns>
    size_t to_node_id() const;

    /// <summary>
    /// Provides the ID value of the link
    /// </summary>
    /// <returns>link ID value</returns>
    size_t get_id() const;

private:
    /// <summary>
    /// Defines the ID of the link
    /// </summary>
    size_t link_id;

    /// <summary>
    /// Provides the from node ID
    /// </summary>
    size_t _from_node_id;

    /// <summary>
    /// Provides the to node ID
    /// </summary>
    size_t _to_node_id;

    /// <summary>
    /// Defines the current gain
    /// </summary>
    double gain;
};

#endif
