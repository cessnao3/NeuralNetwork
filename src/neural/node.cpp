#include "neural/node.h"

#include <algorithm>
#include <cassert>

NeuralNode::NeuralNode(const size_t node_id, const bool bias_node) :
    node_id(node_id),
    value(0.0),
    bias_node(bias_node)
{
    if (bias_node)
    {
        value = 1.0;
    }
}

double NeuralNode::get_value() const
{
    return value;
}

void NeuralNode::set_value(const double value)
{
    if (!bias_node)
    {
        this->value = value;
    }
}

size_t NeuralNode::get_node_id() const
{
    return node_id;
}

bool NeuralNode::is_bias_node() const
{
    return bias_node;
}
