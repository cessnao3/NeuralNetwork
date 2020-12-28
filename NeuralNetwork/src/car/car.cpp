#include "car/car.h"

const double Car::PI = 3.14159265358979323846;

Car::Car() :
    rotation(0.0),
    x(0.0),
    y(0.0),
    input_forward_prev(0.0),
    input_turn_prev(0.0),
    bitmap(nullptr)
{
    // Empty Constructor
}

void Car::init_bitmap()
{
    if (bitmap != nullptr)
    {
        al_destroy_bitmap(bitmap);
        bitmap = nullptr;
    }

    bitmap = al_create_bitmap(get_width(), get_height());
    al_set_target_bitmap(bitmap);

    al_clear_to_color(al_map_rgb(0, 0, 255));
}

ALLEGRO_BITMAP* Car::get_bitmap()
{
    return bitmap;
}

void Car::set_rotation(const double rot_val)
{
    this->rotation = std::fmod(rot_val, 2 * PI);
}

void Car::set_pos(const double x, const double y)
{
    this->x = x;
    this->y = y;
}

double Car::step_filter(const double in, const double prev, const double rate)
{
    if (in > prev + rate)
    {
        return prev + rate;
    }
    else if (in < prev - rate)
    {
        return prev - rate;
    }
    else
    {
        return in;
    }
}

void Car::step(const double forward, const double turn)
{
    const double fwd_incr = 0.005;
    const double trn_incr = 0.05;

    const double fwd_val = step_filter(
        constrain_input(forward),
        input_forward_prev,
        fwd_incr);
    input_forward_prev = fwd_val;

    const double trn_val = step_filter(
        constrain_input(turn),
        input_turn_prev,
        trn_incr);
    input_turn_prev = trn_val;

    const double mod_trn = constrain_input(fwd_val * 2) * trn_val;

    const double turn_rate = 0.02;
    const double forward_speed = 1.5;

    rotation = std::fmod(rotation + turn_rate * mod_trn, 2 * PI);
    x += std::cos(rotation) * fwd_val * forward_speed;
    y += std::sin(rotation) * fwd_val * forward_speed;
}

double Car::get_rotation() const
{
    return rotation;
}

double Car::get_x() const
{
    return x;
}

double Car::get_y() const
{
    return y;
}

double Car::get_width() const
{
    return 60.0;
}

double Car::get_height() const
{
    return 30.0;
}

Car::~Car()
{
    if (bitmap != nullptr)
    {
        al_destroy_bitmap(bitmap);
        bitmap = nullptr;
    }
}

double Car::constrain_input(const double val)
{
    if (val > 1.0)
    {
        return 1.0;
    }
    else if (val < -1.0)
    {
        return -1.0;
    }
    else
    {
        return val;
    }
}
