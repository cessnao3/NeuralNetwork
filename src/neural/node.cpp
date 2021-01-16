#include "neural/node.h"

#include <algorithm>
#include <cassert>

NeuralNode::NeuralNode(const size_t node_id) :
    node_id(node_id),
    value(0.0)
{
    // Empty Constructor
}

double NeuralNode::get_value() const
{
    return value;
}

void NeuralNode::set_value(const double value)
{
    this->value = std::min(std::max(value, -10.0), 10.0);
    //this->value = value;
}

size_t NeuralNode::get_node_id() const
{
    return node_id;
}
