#include "tiles/road_tile_corner.h"

#include <stdexcept>
#include "allegro5/allegro_primitives.h"

#include <cmath>

RoadTileCorner::RoadTileCorner(const Direction direction) :
    direction(direction)
{
    // Provide the center point x/y values based
    // on the corner type
    if (direction == Direction::NW)
    {
        corner_x = 0;
        corner_y = 0;
    }
    else if (direction == Direction::SW)
    {
        corner_x = 0;
        corner_y = TILE_SIZE;
    }
    else if (direction == Direction::NE)
    {
        corner_x = TILE_SIZE;
        corner_y = 0;
    }
    else if (direction == Direction::SE)
    {
        corner_x = TILE_SIZE;
        corner_y = TILE_SIZE;
    }
    else
    {
        throw std::invalid_argument("unknown road tile type provided");
    }
}

bool RoadTileCorner::point_on_road(const double x, const double y) const
{
    // Define the distance from the given corner
    const double dx = x - corner_x;
    const double dy = y - corner_y;
    const double dist_sqr = dx * dx + dy * dy;

    // Return if the distance is between the square values
    return
        (dist_sqr >= std::pow(WIDTH_SMALL, 2.0)) &&
        (dist_sqr <= std::pow(WIDTH_LARGE, 2.0));
}


void RoadTileCorner::draw_bitmap()
{
    al_draw_filled_circle(
        corner_x,
        corner_y,
        WIDTH_LARGE,
        road_color);
    al_draw_filled_circle(
        corner_x,
        corner_y,
        WIDTH_SMALL,
        grass_color);
}
