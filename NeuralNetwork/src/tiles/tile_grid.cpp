#include "tiles/tile_grid.h"

const uint32_t RoadGrid::GridLoc::get_center_x() const
{
    return x + RoadTile::TILE_SIZE / 2;
}

const uint32_t RoadGrid::GridLoc::get_center_y() const
{
    return y + RoadTile::TILE_SIZE / 2;
}

RoadGrid::RoadGrid(size_t width, size_t height) :
    width(width),
    height(height),
    start_ind(0)
{
    for (size_t r = 0; r < height; ++r)
    {
        for (size_t c = 0; c < width; ++c)
        {
            GridLoc loc;
            loc.tile = nullptr;
            loc.x = c * RoadTile::TILE_SIZE;
            loc.y = r * RoadTile::TILE_SIZE;
            loc.type = RoadTileManager::RoadTileType::GRASS;
            tiles.push_back(loc);
        }
    }

    reset_tile_pointers();
}

void RoadGrid::reset_tile_pointers()
{
    for (size_t i = 0; i < tiles.size(); ++i)
    {
        tiles[i].tile = manager.get_tile(tiles[i].type);
    }
}

RoadGrid::GridLoc* RoadGrid::at(const size_t row, const size_t col)
{
    return this->at(rc_to_ind(row, col));
}

RoadGrid::GridLoc* RoadGrid::at(const size_t ind)
{
    if (ind < tiles.size())
    {
        return &tiles[ind];
    }
    else
    {
        return nullptr;
    }
}

bool RoadGrid::set(const size_t row, const size_t col, const RoadTileManager::RoadTileType type)
{
    return set(rc_to_ind(row, col), type);
}

bool RoadGrid::set(const size_t ind, const RoadTileManager::RoadTileType type)
{
    if (ind < tiles.size())
    {
        tiles[ind].type = type;
        tiles[ind].tile = manager.get_tile(type);
        return true;
    }
    else
    {
        return false;
    }
}

size_t RoadGrid::get_width() const
{
    return width;
}

size_t RoadGrid::get_height() const
{
    return height;
}

void RoadGrid::set_start_ind(const size_t start_ind)
{
    this->start_ind = start_ind;
}

void RoadGrid::set_start_ind(const size_t row, const size_t col)
{
    set_start_ind(rc_to_ind(row, col));
}

size_t RoadGrid::get_start_ind() const
{
    return start_ind;
}

size_t RoadGrid::rc_to_ind(const size_t row, const size_t col) const
{
    return width * row + col;
}

size_t RoadGrid::ind_to_r(const size_t ind) const
{
    return ind / width;
}

size_t RoadGrid::ind_to_c(const size_t ind) const
{
    return ind % width;
}
