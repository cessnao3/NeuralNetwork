#ifndef __IO_ROAD_TILE_GRASS__
#define __IO_ROAD_TILE_GRASS__

#include "tiles/road_tile.h"

class RoadTileGrass : public RoadTile
{
public:
    virtual bool point_on_road(const double x, const double y) const;

protected:
    virtual void draw_bitmap() override;
};

#endif
