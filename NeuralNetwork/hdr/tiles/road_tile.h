#ifndef __IO_ROAD_TILE__
#define __IO_ROAD_TILE__

#include <allegro5/allegro.h>
#include <cstdint>

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


class RoadTile
{
public:
    RoadTile();

    RoadTile(const RoadTile& other);

    RoadTile& operator=(const RoadTile& other);

    ALLEGRO_BITMAP* get_bitmap() const;

    virtual bool point_on_road(const double x, const double y) const = 0;

    virtual void init_bitmap();

    ~RoadTile();

protected:
    virtual void draw_bitmap() = 0;

    virtual void prepare_bitmap();

    virtual void finish_bitmap();

protected:
    ALLEGRO_BITMAP* bitmap;

public:
    static const uint32_t TILE_SIZE;
    static const uint32_t ROAD_WIDTH;

protected:
    ALLEGRO_COLOR road_color;
    ALLEGRO_COLOR grass_color;

    static const uint32_t WIDTH_LARGE;
    static const uint32_t WIDTH_SMALL;
};

#endif
