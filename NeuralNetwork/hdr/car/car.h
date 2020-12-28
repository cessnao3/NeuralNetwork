#ifndef __IO_CAR__
#define __IO_CAR__

#include <allegro5/allegro.h>

#include <tiles/tile_grid.h>

#include <cmath>

class Car
{
public:
    static const double PI;

    struct SensorResult
    {
        SensorResult() :
            start_x(0),
            start_y(0),
            impact_x(0),
            impact_y(0),
            dist(0)
        {
            // Empty Constructor
        }

        double start_x;
        double start_y;
        double impact_x;
        double impact_y;
        double dist;
    };

public:
    Car();

    void init_bitmap();

    ALLEGRO_BITMAP* get_bitmap();

    void set_rotation(const double rot_val);

    void set_pos(const double x, const double y);

    void step(const double forward, const double turn);

    double get_rotation() const;

    double get_car_x() const;

    double get_car_y() const;

    void check_collision(const RoadGrid& grid);

    void reset();

    double get_width() const;

    double get_height() const;

    bool has_collided() const;

    double get_distance() const;

    SensorResult get_sensor(const RoadGrid& grid, const size_t sensor_num) const;

    ~Car();

protected:
    SensorResult calc_sensor_dist(const RoadGrid& grid, const double dlon, const double dlat) const;

    double constrain_input(const double val);

    double step_filter(const double in, const double prev, const double rate);

    double rot_vec_lon(const double lon, const double lat) const;
    double rot_vec_lat(const double lon, const double lat) const;

protected:
    double rotation;
    double x;
    double y;

    double start_x;
    double start_y;
    double start_rot;

    double distance;

    double input_forward_prev;
    double input_turn_prev;

    bool collided;

    ALLEGRO_BITMAP* bitmap;
};

#endif
