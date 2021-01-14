#ifndef __IO_ROAD_TILE_STRAIGHT__
#define __IO_ROAD_TILE_STRAIGHT__

#include "tiles/road_tile.h"

/// <summary>
/// Defines the road tile associated with straight roads
/// </summary>
class RoadTileStraight : public RoadTile
{
public:
    /// <summary>
    /// Defines the NS/EW direction to use for the road direction
    /// </summary>
    enum class Direction
    {
        EW = 0,
        NS = 1
    };

public:
    /// <summary>
    /// Defines the road tile with a provided direction
    /// </summary>
    /// <param name="dir">the direction to use for the road tile</param>
    RoadTileStraight(const Direction dir);

    /// <summary>
    /// Checks to see if the point requested, in tile-relative coordinates,
    /// is within the tile
    /// </summary>
    /// <param name="x">x location</param>
    /// <param name="y">y location</param>
    /// <returns>true if the point is on the road</returns>
    virtual bool point_on_road(const double x, const double y) const override;

protected:
    /// <summary>
    /// Performs any additional drawing required for the road tile
    /// </summary>
    virtual void draw_bitmap() override;

protected:
    /// <summary>
    /// Defines the direction of the straight tile
    /// </summary>
    Direction direction;
};

#endif
