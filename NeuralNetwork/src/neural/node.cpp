#include "neural/node.h"

#include <algorithm>

NeuralNode::NeuralNode(const size_t node_id) :
    node_id(node_id),
    value(0.0)
{
    // Empty Constructor
}

void NeuralNode::add_link_id(const size_t link_id)
{
    if (std::find(link_ids.begin(), link_ids.end(), link_id) != link_ids.end())
    {
        link_ids.push_back(link_id);
    }
}

double NeuralNode::get_value() const
{
    return value;
}

void NeuralNode::set_value(const double value)
{
    this->value = value;
}
