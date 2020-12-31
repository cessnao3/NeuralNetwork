#ifndef __IO_ROAD_TILE_MANAGER
#define __IO_ROAD_TILE_MANAGER

#include <vector>
#include <unordered_map>

#include "tiles/road_tile.h"
#include "tiles/road_tile_grass.h"
#include "tiles/road_tile_corner.h"
#include "tiles/road_tile_straight.h"

/// <summary>
/// Manages road tiles and pointer types to easily get a given tile type
/// for a variety of supported tiles
/// </summary>
class RoadTileManager
{
public:
    /// <summary>
    /// Enumeration to define a selection of tile types to use
    /// </summary>
    enum class RoadTileType
    {
        GRASS = 0,
        STRAIGHT_V,
        STRAIGHT_H,
        CORNER_NW,
        CORNER_SW,
        CORNER_NE,
        CORNER_SE
    };

public:
    /// <summary>
    /// Initializes the road tile manager
    /// </summary>
    RoadTileManager();

    /// <summary>
    /// Gets a tile type for the selected type
    /// </summary>
    /// <param name="type">selected tile type to get</param>
    /// <returns>pointer to a road tile of the given type</returns>
    RoadTile* get_tile(RoadTileType type);

    /// <summary>
    /// Initializes all bitmaps within the tile
    /// </summary>
    void init_bitmaps();

private:
    /// <summary>
    /// Stores all tile types for easy retrieval
    /// </summary>
    std::unordered_map<RoadTileType, RoadTile*> tiles;

    /// <summary>
    /// Defines the tile grass parameter
    /// </summary>
    RoadTileGrass tile_grass;

    /// <summary>
    /// Defines the straight tile parameters
    /// </summary>
    std::vector<RoadTileStraight> tiles_straight;

    /// <summary>
    /// Defines the corner tile parameters
    /// </summary>
    std::vector<RoadTileCorner> tiles_corner;
};

#endif
