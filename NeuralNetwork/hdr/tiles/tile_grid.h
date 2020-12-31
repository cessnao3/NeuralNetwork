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
        GridLoc() :
            tile(nullptr),
            x(0),
            y(0),
            type(RoadTileManager::RoadTileType::GRASS)
        {
            // Empty Constructor
        }

        RoadTile* tile;
        uint32_t x;
        uint32_t y;
        RoadTileManager::RoadTileType type;

        const uint32_t get_center_x() const;

        const uint32_t get_center_y() const;
    };

public:
    RoadGrid(size_t width, size_t height);

    void init_bitmaps();

    void reset_tile_pointers();

    GridLoc* at(const size_t row, const size_t col);
    GridLoc* at(const size_t ind);

    const GridLoc* get_for_xy_const(const double x, const double y) const;
    const GridLoc* get_for_xy(const double x, const double y) const;
    GridLoc* get_for_xy(const double x, const double y);

    const GridLoc* at(const size_t row, const size_t col) const;
    const GridLoc* at(const size_t ind) const;

    bool set(const size_t row, const size_t col, const RoadTileManager::RoadTileType type);

    bool set(const size_t ind, const RoadTileManager::RoadTileType type);

    size_t get_width() const;

    size_t get_height() const;

    void set_start_ind(const size_t start_ind);

    void set_start_ind(const size_t row, const size_t col);

    size_t get_start_ind() const;

protected:
    size_t rc_to_ind(const size_t row, const size_t col) const;

    size_t ind_to_r(const size_t ind) const;

    size_t ind_to_c(const size_t ind) const;

protected:
    size_t width;
    size_t height;

    size_t start_ind;

    std::vector<GridLoc> tiles;
    RoadTileManager manager;
};

#endif
