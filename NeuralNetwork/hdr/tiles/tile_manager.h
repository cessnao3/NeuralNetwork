#ifndef __IO_ROAD_TILE_MANAGER
#define __IO_ROAD_TILE_MANAGER

#include <vector>
#include <unordered_map>

#include "tiles/road_tile.h"
#include "tiles/road_tile_grass.h"
#include "tiles/road_tile_corner.h"
#include "tiles/road_tile_straight.h"

class RoadTileManager
{
public:
    RoadTileManager();

    RoadTile* get_tile(RoadTileType type);

    size_t num_tiles() const;

private:
    std::unordered_map<RoadTileType, RoadTile*> tiles;

    RoadTileGrass tile_grass;
    std::vector<RoadTileStraight> tiles_straight;
    std::vector<RoadTileCorner> tiles_corner;
};

#endif
