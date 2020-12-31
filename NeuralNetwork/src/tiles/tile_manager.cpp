#include "tiles/tile_manager.h"

#include <iostream>

RoadTileManager::RoadTileManager()
{
    // Intiialize Tile Types
    tiles_corner.push_back(RoadTileCorner(RoadTileCorner::Direction::NE));
    tiles_corner.push_back(RoadTileCorner(RoadTileCorner::Direction::NW));
    tiles_corner.push_back(RoadTileCorner(RoadTileCorner::Direction::SE));
    tiles_corner.push_back(RoadTileCorner(RoadTileCorner::Direction::SW));

    // Initialize Straight Types
    tiles_straight.push_back(RoadTileStraight(RoadTileStraight::Direction::VERTICAL));
    tiles_straight.push_back(RoadTileStraight(RoadTileStraight::Direction::HORIZONTAL));

    // Add resulting tiles
    tiles[RoadTileType::CORNER_NE] = &tiles_corner[0];
    tiles[RoadTileType::CORNER_NW] = &tiles_corner[1];
    tiles[RoadTileType::CORNER_SE] = &tiles_corner[2];
    tiles[RoadTileType::CORNER_SW] = &tiles_corner[3];
    tiles[RoadTileType::STRAIGHT_V] = &tiles_straight[0];
    tiles[RoadTileType::STRAIGHT_H] = &tiles_straight[1];
    tiles[RoadTileType::GRASS] = &tile_grass;
}

void RoadTileManager::init_bitmaps()
{
    // Iterate over key-value pairs
    for (auto kv = tiles.begin(); kv != tiles.end(); ++kv)
    {
        kv->second->init_bitmap();
    }
}

RoadTile* RoadTileManager::get_tile(RoadTileType type)
{
    return tiles[type];
}

size_t RoadTileManager::num_tiles() const
{
    return tiles.size();
}
