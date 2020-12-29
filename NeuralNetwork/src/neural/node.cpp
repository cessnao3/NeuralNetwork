#include "neural/node.h"

#include <algorithm>

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
    this->value = value;
}
