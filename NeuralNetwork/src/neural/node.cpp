#include "neural/node.h"

#include <algorithm>
#include <cassert>

NeuralNode::NeuralNode(const size_t node_id, const double bias) :
    node_id(node_id),
    value(0.0),
    bias(bias)
{
    // Empty Constructor
}

double NeuralNode::get_value() const
{
    const bool is_neg = value < 0.0;
    const double value_out = std::max(0.0, (std::abs(value) - bias) / (1.0 - bias)) * (is_neg ? -1.0 : 1.0);

    assert(std::abs(value_out) <= 1.0);

    return value_out;
}

void NeuralNode::set_value(const double value)
{
    if (value < -1.0 || value > 1.0)
    {
        //assert(false);
    }
    //this->value = std::min(std::max(value, -1.0), 1.0);
    this->value = value;
}

size_t NeuralNode::get_node_id() const
{
    return node_id;
}

double NeuralNode::get_bias() const
{
    return bias;
}

bool NeuralNode::set_bias(const double bias)
{
    if (bias >= 0.0 && bias <= 1.0)
    {
        this->bias = bias;
        return true;
    }
    else
    {
        return false;
    }
}
