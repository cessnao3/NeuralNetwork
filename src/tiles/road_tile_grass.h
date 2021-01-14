#ifndef __IO_ROAD_TILE_GRASS__
#define __IO_ROAD_TILE_GRASS__

#include "tiles/road_tile.h"

/// <summary>
/// Defines the road tile associated with grass
/// </summary>
class RoadTileGrass : public RoadTile
{
public:
    /// <summary>
    /// Checks to see if the point requested, in tile-relative coordinates,
    /// is within the tile
    /// </summary>
    /// <param name="x">x location</param>
    /// <param name="y">y location</param>
    /// <returns>true if the point is on the road</returns>
    virtual bool point_on_road(const double x, const double y) const;

protected:
    /// <summary>
    /// Performs any additional drawing required for the road tile
    /// </summary>
    virtual void draw_bitmap() override;
};

#endif
