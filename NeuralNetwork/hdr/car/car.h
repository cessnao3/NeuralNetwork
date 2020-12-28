#ifndef __IO_CAR__
#define __IO_CAR__

#include <allegro5/allegro.h>

#include <cmath>

class Car
{
public:
    static const double PI;

public:
    Car();

    void init_bitmap();

    ALLEGRO_BITMAP* get_bitmap();

    void set_rotation(const double rot_val);

    void set_pos(const double x, const double y);

    void step(const double forward, const double turn);

    double get_rotation() const;

    double get_x() const;

    double get_y() const;

    double get_width() const;

    double get_height() const;

    ~Car();

protected:
    double constrain_input(const double val);

    double step_filter(const double in, const double prev, const double rate);

protected:
    double rotation;
    double x;
    double y;

    double input_forward_prev;
    double input_turn_prev;

    ALLEGRO_BITMAP* bitmap;
};

#endif
