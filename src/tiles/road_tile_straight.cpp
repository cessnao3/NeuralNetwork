#include "tiles/road_tile_straight.h"

#include <stdexcept>
#include <allegro5/allegro_primitives.h>

RoadTileStraight::RoadTileStraight(const Direction dir) :
    direction(dir)
{
    if (dir != Direction::EW &&
        dir != Direction::NS)
    {
        throw std::invalid_argument("unknown direction provided");
    }
}

bool RoadTileStraight::point_on_road(const double x, const double y) const
{
    switch (direction)
    {
    case Direction::EW:
        return y >= ROAD_WIDTH_SMALL && y <= ROAD_WIDTH_LARGE;
    case Direction::NS:
        return x >= ROAD_WIDTH_SMALL && x <= ROAD_WIDTH_LARGE;
    default:
        return false;
    }
}

void RoadTileStraight::draw_bitmap()
{
    ALLEGRO_BITMAP* prev_target = al_get_target_bitmap();
    al_set_target_bitmap(bitmap);
    if (direction == Direction::NS)
    {
        al_draw_filled_rectangle(
            ROAD_WIDTH_SMALL,
            0,
            ROAD_WIDTH_LARGE,
            TILE_SIZE,
            get_road_color());
    }
    else
    {
        al_draw_filled_rectangle(
            0,
            ROAD_WIDTH_SMALL,
            TILE_SIZE,
            ROAD_WIDTH_LARGE,
            get_road_color());
    }
    al_set_target_bitmap(prev_target);
}
