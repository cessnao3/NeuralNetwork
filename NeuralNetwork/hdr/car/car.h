#ifndef __IO_CAR__
#define __IO_CAR__

#include <allegro5/allegro.h>

#include <cmath>

class Car
{
protected:
    const double PI = 3.14159265358979323846;

public:
    Car() :
        rotation(0.0),
        x(0.0),
        y(0.0)
    {
        // Empty Constructor
    }

    void init_bitmap()
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

    ALLEGRO_BITMAP* get_bitmap()
    {
        return bitmap;
    }

    void step(const double forward, const double turn)
    {
        const double fwd_val = constrain_input(forward);
        const double trn_val = constrain_input(turn);
        rotation = std::fmod(rotation + trn_val * 0.03 * fwd_val, 2 * PI);
        x += std::cos(rotation) * fwd_val;
        y += std::sin(rotation) * fwd_val;
    }

    double get_rotation() const
    {
        return rotation;
    }

    double get_x() const
    {
        return x;
    }

    double get_y() const
    {
        return y;
    }

    double get_width() const
    {
        return 60.0;
    }

    double get_height() const
    {
        return 30.0;
    }

    ~Car()
    {
        if (bitmap != nullptr)
        {
            al_destroy_bitmap(bitmap);
            bitmap = nullptr;
        }
    }

protected:
    double constrain_input(const double val)
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

protected:
    double rotation;
    double x;
    double y;
    ALLEGRO_BITMAP* bitmap;
};

#endif
