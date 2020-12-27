#ifndef __IO_TILE_GRID__
#define __IO_TILE_GRID__

#include "tiles/tile_manager.h"

#include <vector>

#include <cstdint>

class RoadGrid
{
public:
    struct GridLoc
    {
    public:
        RoadTile* tile;
        uint32_t x;
        uint32_t y;
        RoadTileManager::RoadTileType type;
    };

public:
    RoadGrid(size_t width, size_t height) :
        width(width),
        height(height)
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

    void reset_tile_pointers()
    {
        for (size_t i = 0; i < tiles.size(); ++i)
        {
            tiles[i].tile = manager.get_tile(tiles[i].type);
        }
    }

    GridLoc* at(const size_t row, const size_t col)
    {
        return this->at(rc_to_ind(row, col));
    }

    GridLoc* at(const size_t ind)
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

    bool set(const size_t row, const size_t col, const RoadTileManager::RoadTileType type)
    {
        return set(rc_to_ind(row, col), type);
    }

    bool set(const size_t ind, const RoadTileManager::RoadTileType type)
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

    size_t get_width() const
    {
        return width;
    }

    size_t get_height() const
    {
        return height;
    }

protected:
    size_t rc_to_ind(const size_t row, const size_t col) const
    {
        return width * row + col;
    }

    size_t ind_to_r(const size_t ind) const
    {
        return ind / width;
    }

    size_t ind_to_c(const size_t ind) const
    {
        return ind % width;
    }

protected:
    size_t width;
    size_t height;

    std::vector<GridLoc> tiles;
    RoadTileManager manager;
};

#endif
