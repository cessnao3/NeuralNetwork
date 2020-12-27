#ifndef __IO_ROAD_TILE_STRAIGHT__
#define __IO_ROAD_TILE_STRAIGHT__

#include "tiles/road_tile.h"

class RoadTileStraight : public RoadTile
{
public:
    enum class Direction
    {
        HORIZONTAL = 0,
        VERTICAL
    };

public:
    RoadTileStraight(const Direction dir);

    virtual bool point_on_road(const double x, const double y) const override;

protected:
    virtual void draw_bitmap() override;

protected:
    Direction direction;
};

#endif
