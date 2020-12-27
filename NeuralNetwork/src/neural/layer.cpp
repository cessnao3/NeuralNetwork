#include "neural/layer.h"

#include <algorithm>

static bool add_if_contains(std::vector<size_t>& vec, const size_t val)
{
    if (std::find(vec.begin(), vec.end(), val) == vec.end())
    {
        vec.push_back(val);
        return true;
    }
    else
    {
        return false;
    }
}

bool NeuralLayer::add_node(size_t node_id)
{
    return add_if_contains(node_ids, node_id);
}

bool NeuralLayer::add_link(size_t link_id)
{
    return add_if_contains(link_ids, link_id);
}
