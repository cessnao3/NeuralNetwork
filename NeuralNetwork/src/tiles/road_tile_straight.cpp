#include "tiles/road_tile_straight.h"

#include <stdexcept>
#include <allegro5/allegro_primitives.h>

RoadTileStraight::RoadTileStraight(const Direction dir) :
    direction(dir)
{
    if (dir != Direction::HORIZONTAL &&
        dir != Direction::VERTICAL)
    {
        throw std::invalid_argument("unknown direction provided");
    }
}

bool RoadTileStraight::point_on_road(const double x, const double y) const
{
    switch (direction)
    {
    case Direction::HORIZONTAL:
        return y >= WIDTH_SMALL && y <= WIDTH_LARGE;
    case Direction::VERTICAL:
        return x >= WIDTH_SMALL && x <= WIDTH_LARGE;
    default:
        return false;
    }
}

void RoadTileStraight::draw_bitmap()
{
    if (direction == Direction::VERTICAL)
    {
        al_draw_filled_rectangle(
            WIDTH_SMALL,
            0,
            WIDTH_LARGE,
            TILE_SIZE,
            road_color);
    }
    else
    {
        al_draw_filled_rectangle(
            0,
            WIDTH_SMALL,
            TILE_SIZE,
            WIDTH_LARGE,
            road_color);
    }
}