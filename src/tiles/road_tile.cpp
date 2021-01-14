#include "tiles/road_tile.h"

#include <allegro5/allegro_primitives.h>

#include <stdexcept>

const uint32_t RoadTile::TILE_SIZE = 120;
const uint32_t RoadTile::ROAD_WIDTH = 90;

const uint32_t RoadTile::ROAD_WIDTH_LARGE = (RoadTile::TILE_SIZE + RoadTile::ROAD_WIDTH) / 2;
const uint32_t RoadTile::ROAD_WIDTH_SMALL = (RoadTile::TILE_SIZE - RoadTile::ROAD_WIDTH) / 2;

RoadTile::RoadTile()
{
    // Empty Constructor
}

RoadTile::RoadTile(const RoadTile& other) :
    bitmap(nullptr)
{
    if (other.bitmap != nullptr)
    {
        prepare_bitmap();
        ALLEGRO_BITMAP* prev_target = al_get_target_bitmap();
        al_draw_bitmap(other.bitmap, 0, 0, 0);
        al_set_target_bitmap(prev_target);
    }
}

RoadTile& RoadTile::operator=(const RoadTile& other)
{
    if (this == &other)
    {
        return *this;
    }
    ALLEGRO_BITMAP* prev_target = al_get_target_bitmap();
    al_set_target_bitmap(bitmap);
    al_draw_bitmap(other.bitmap, 0, 0, 0);
    al_set_target_bitmap(prev_target);

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

    ALLEGRO_BITMAP* prev_target = al_get_target_bitmap();

    al_set_target_bitmap(bitmap);
    al_clear_to_color(get_grass_color());

    al_set_target_bitmap(prev_target);
}

void RoadTile::finish_bitmap()
{
    ALLEGRO_BITMAP* prev_target = al_get_target_bitmap();
    al_set_target_bitmap(bitmap);
    al_draw_rectangle(
        1,
        1,
        TILE_SIZE,
        TILE_SIZE,
        al_map_rgb(48, 48, 48),
        0);
    al_set_target_bitmap(prev_target);
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
