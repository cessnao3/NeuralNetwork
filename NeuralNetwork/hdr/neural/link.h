#ifndef __IO_NEURAL_LINK__
#define __IO_NEURAL_LINK__

#include <vector>

class NeuralLink
{
public:
    /// <summary>
    /// Constructs a new link with the provided gain
    /// </summary>
    NeuralLink(const size_t id, const size_t from_node_id, const size_t to_node_id, const double gain=0.0) :
        link_id(id),
        _from_node_id(from_node_id),
        _to_node_id(to_node_id),
        gain(gain)
    {
        // Empty Constructor
    }

    /// <summary>
    /// Sets the gain associated with the gain
    /// </summary>
    /// <param name="gain">the new gain to set</param>
    void set_gain(const double gain)
    {
        this->gain = gain;
    }

    /// <summary>
    /// Provides the current gain associated with the link
    /// </summary>
    /// <returns>the current gain</returns>
    double get_gain() const
    {
        return gain;
    }

    /// <summary>
    /// Returns the from node ID
    /// </summary>
    /// <returns>the node the link is coming from</returns>
    size_t from_node_id() const
    {
        return _from_node_id;
    }

    /// <summary>
    /// Returns the to node ID
    /// </summary>
    /// <returns>the node the link is going to</returns>
    size_t to_node_id() const
    {
        return _to_node_id;
    }

    /// <summary>
    /// Provides the ID value of the link
    /// </summary>
    /// <returns>link ID value</returns>
    size_t get_id() const
    {
        return link_id;
    }

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
