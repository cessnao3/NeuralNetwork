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
    // Empty Constructor
}

RoadTile::RoadTile(const RoadTile& other) :
    bitmap(nullptr)
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
    al_clear_to_color(get_grass_color());
}

void RoadTile::finish_bitmap()
{
    al_draw_rectangle(
        1,
        1,
        TILE_SIZE,
        TILE_SIZE,
        al_map_rgb(48, 48, 48),
        0);
}

RoadTile::~RoadTile()
{
    if (bitmap != nullptr)
    {
        al_destroy_bitmap(bitmap);
    }
}

ALLEGRO_COLOR RoadTile::get_road_color()
{
    return al_map_rgb(71, 71, 71);
}

ALLEGRO_COLOR RoadTile::get_grass_color()
{
    return al_map_rgb(62, 117, 59);
}
