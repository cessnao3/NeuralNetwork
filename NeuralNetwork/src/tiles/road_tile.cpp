#include "tiles/road_tile.h"

#include <allegro5/allegro_primitives.h>

#include <stdexcept>

const uint32_t RoadTile::TILE_SIZE = 128;
const uint32_t RoadTile::ROAD_WIDTH = 80;

const uint32_t RoadTile::WIDTH_LARGE = (RoadTile::TILE_SIZE + RoadTile::ROAD_WIDTH) / 2;
const uint32_t RoadTile::WIDTH_SMALL = (RoadTile::TILE_SIZE - RoadTile::ROAD_WIDTH) / 2;

RoadTile::RoadTile() :
    bitmap(nullptr)
{
    // Initialize Colors
    road_color = al_map_rgb(128, 128, 128);
    grass_color = al_map_rgb(84, 171, 71);
}

RoadTile::RoadTile(const RoadTile& other) :
    bitmap(nullptr),
    grass_color(grass_color),
    road_color(road_color)
{
    if (other.bitmap != nullptr)
    {
        prepare_bitmap();
        al_draw_bitmap(other.bitmap, 0, 0, 0);
    }
}

RoadTile& RoadTile::operator=(const RoadTile& other)
{
    if (this == &other)
    {
        return *this;
    }

    grass_color = other.grass_color;
    road_color = other.road_color;

    al_set_target_bitmap(bitmap);
    al_draw_bitmap(other.bitmap, 0, 0, 0);
    return *this;
}

ALLEGRO_BITMAP* RoadTile::get_bitmap() const
{
    return bitmap;
}

void RoadTile::init_bitmap()
{
    this->prepare_bitmap();
    this->draw_bitmap();
    this->finish_bitmap();
}

void RoadTile::prepare_bitmap()
{
    if (bitmap != nullptr)
    {
        al_destroy_bitmap(bitmap);
        bitmap = nullptr;
    }

    bitmap = al_create_bitmap(TILE_SIZE, TILE_SIZE);

    al_set_target_bitmap(bitmap);
    al_clear_to_color(grass_color);
}

void RoadTile::finish_bitmap()
{
    al_draw_rectangle(
        1,
        1,
        TILE_SIZE,
        TILE_SIZE,
        al_map_rgb(0, 0, 0),
        0);
}

RoadTile::~RoadTile()
{
    if (bitmap != nullptr)
    {
        al_destroy_bitmap(bitmap);
    }
}
