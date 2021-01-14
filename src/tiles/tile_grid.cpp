#include "tiles/tile_grid.h"

#include <stdexcept>

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
            loc.x = static_cast<uint32_t>(c * RoadTile::TILE_SIZE);
            loc.y = static_cast<uint32_t>(r * RoadTile::TILE_SIZE);
            loc.type = RoadTileManager::RoadTileType::GRASS;
            tiles.push_back(loc);
        }
    }

    reset_tile_pointers();
}

void RoadGrid::init_bitmaps()
{
    manager.init_bitmaps();
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

const RoadGrid::GridLoc* RoadGrid::at(const size_t row, const size_t col) const
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

const RoadGrid::GridLoc* RoadGrid::at(const size_t ind) const
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

const RoadGrid::GridLoc* RoadGrid::get_for_xy_const(const double x, const double y) const
{
    // Determine if we have gone off the screen to the left/top
    if (x < 0 || y < 0)
    {
        return nullptr;
    }

    // Extract the row/col index
    const size_t row = static_cast<size_t>(y / RoadTile::TILE_SIZE);
    const size_t col = static_cast<size_t>(x / RoadTile::TILE_SIZE);

    // Determine if we have gone off the screen to teh right/bottom
    if (row >= get_height() || col >= get_width())
    {
        return nullptr;
    }

    // Otherwise, return the resulting value
    return at(row, col);
}

const RoadGrid::GridLoc* RoadGrid::get_for_xy(const double x, const double y) const
{
    return get_for_xy_const(x, y);
}

RoadGrid::GridLoc* RoadGrid::get_for_xy(const double x, const double y)
{
    const GridLoc* loc = get_for_xy_const(x, y);
    return const_cast<GridLoc*>(loc);
}

void RoadGrid::set(const size_t row, const size_t col, const RoadTileManager::RoadTileType type)
{
    set(rc_to_ind(row, col), type);
}

void RoadGrid::set(const size_t ind, const RoadTileManager::RoadTileType type)
{
    if (ind < tiles.size())
    {
        tiles[ind].type = type;
        tiles[ind].tile = manager.get_tile(type);
    }
    else
    {
        throw std::out_of_range("tile index out of range");
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
