#ifndef __IO_ROAD_TILE_CORNER__
#define __IO_ROAD_TILE_CORNER__

#include "tiles/road_tile.h"

/// <summary>
/// Defines the road tile associated with a basic corner
/// </summary>
class RoadTileCorner : public RoadTile
{
public:
    /// <summary>
    /// Defines the direction by the two sides of the tile that
    /// the road connects to
    /// </summary>
    enum class Direction
    {
        NW = 0,
        NE = 1,
        SW = 2,
        SE = 3
    };

public:
    /// <summary>
    /// Constructs the corner tile with a direction
    /// </summary>
    /// <param name="direction">the direction to set the tile to use</param>
    RoadTileCorner(const Direction direction);

    /// <summary>
    /// Checks to see if the point requested, in tile-relative coordinates,
    /// is within the tile
    /// </summary>
    /// <param name="x">x location</param>
    /// <param name="y">y location</param>
    /// <returns>true if the point is on the road</returns>
    virtual bool point_on_road(
        const double x,
        const double y) const override;

protected:
    /// <summary>
    /// Draws the road on the bitmap
    /// </summary>
    virtual void draw_bitmap() override;

protected:
    /// <summary>
    /// Defines the direction
    /// </summary>
    Direction direction;

    /// <summary>
    /// Defines the X location to use for the corner
    /// </summary>
    uint32_t corner_x;

    /// <summary>
    /// Defines the Y location to use for the corner
    /// </summary>
    uint32_t corner_y;
};

#endif
