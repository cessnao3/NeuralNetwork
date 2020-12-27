#include "neural/link.h"

NeuralLink::NeuralLink(
    const size_t id,
    const size_t from_node_id,
    const size_t to_node_id,
    const double gain) :
    link_id(id),
    _from_node_id(from_node_id),
    _to_node_id(to_node_id),
    gain(gain)
{
    // Empty Constructor
}

void NeuralLink::set_gain(const double gain)
{
    this->gain = gain;
}

double NeuralLink::get_gain() const
{
    return gain;
}

size_t NeuralLink::from_node_id() const
{
    return _from_node_id;
}

size_t NeuralLink::to_node_id() const
{
    return _to_node_id;
}

size_t NeuralLink::get_id() const
{
    return link_id;
}