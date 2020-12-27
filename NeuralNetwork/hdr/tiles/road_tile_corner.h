#ifndef __IO_ROAD_TILE_CORNER__
#define __IO_ROAD_TILE_CORNER__

#include "tiles/road_tile.h"

class RoadTileCorner : public RoadTile
{
public:
    enum class Direction
    {
        NW = 0,
        NE,
        SW,
        SE
    };

public:
    RoadTileCorner(const Direction direction);

    virtual bool point_on_road(const double x, const double y) const override;

protected:
    virtual void draw_bitmap() override;

protected:
    Direction direction;

    uint32_t corner_x;
    uint32_t corner_y;
};

#endif
