#include "car/car.h"

#include <allegro5/allegro_primitives.h>

#include <stdexcept>
#include <cmath>

const double Car::PI = 3.14159265358979323846;

Car::Car()
{
    // Initialize sensor values
    const std::array<double, num_sensors> delta_lons = {
        1, 1, 1, 0, 0, 2, 2
    };

    const std::array<double, num_sensors> delta_lats = {
        0, 1, -1, 1, -1, 1, -1
    };

    const std::array<bool, num_sensors> fronts = {
        true, true, true, true, true, true, true
    };

    for (size_t i = 0; i < delta_lons.size(); ++i)
    {
        Sensor s;
        s.delta_lat = delta_lats[i];
        s.delta_lon = delta_lons[i];
        s.is_front = fronts[i];
        sensors[i] = s;
    }
}

void Car::init_bitmap()
{
    // Destroy the bitmap if it already exists
    if (bitmap != nullptr)
    {
        al_destroy_bitmap(bitmap);
        bitmap = nullptr;
    }

    // Define the bitmap target
    ALLEGRO_BITMAP* prev_target = al_get_target_bitmap();

    // Create a new bitmap and target to draw
    bitmap = al_create_bitmap(get_width(), get_height());
    al_set_target_bitmap(bitmap);

    // Clear the bitmap
    al_clear_to_color(al_map_rgb(0, 0, 255));

    // Add lights
    const uint32_t light_depth = 5;
    const uint32_t light_width = 10;

    const ALLEGRO_COLOR headlight_color = al_map_rgb(255, 255, 255);
    const ALLEGRO_COLOR taillight_color = al_map_rgb(255, 0, 0);

    // Draw Headlights
    al_draw_filled_rectangle(
        get_width() - light_depth,
        0,
        get_width(),
        light_width,
        headlight_color);

    al_draw_filled_rectangle(
        get_width() - light_depth,
        get_height() - light_width,
        get_width(),
        get_height(),
        headlight_color);

    // Draw Taillights
    al_draw_filled_rectangle(
        0,
        0,
        light_depth,
        light_width,
        taillight_color);
    al_draw_filled_rectangle(
        0,
        get_height() - light_width,
        light_depth,
        get_height(),
        taillight_color);

    // Reset the bitmap target
    al_set_target_bitmap(prev_target);
}

ALLEGRO_BITMAP* Car::get_bitmap() const
{
    return bitmap;
}

void Car::set_start_rotation(const double rot_val)
{
    this->start_rot = std::fmod(rot_val, 2 * PI);
    this->reset();
}

void Car::set_pos(const double x, const double y)
{
    this->start_x = x;
    this->start_y = y;
    this->reset();
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

void Car::step_movement(const RoadGrid& grid, const double forward, const double turn)
{
    // Move only if we haven't collided with anything
    if (!collided)
    {
        // Calculate the breaking force for when the input and previous
        // values differ
        bool input_same_sign =
            (forward >= 0 && input_forward_prev >= 0) ||
            (forward <= 0 && input_forward_prev <= 0);

        // Set the forward and turn increments
        const double fwd_incr = 0.005 * ((input_same_sign) ? 1 : 10);
        const double trn_incr = 0.075;

        // Filter the forward values
        const double fwd_val = step_filter(
            constrain_input(forward),
            input_forward_prev,
            fwd_incr);
        input_forward_prev = fwd_val;

        // Filter the turn values
        const double trn_val = step_filter(
            constrain_input(turn),
            input_turn_prev,
            trn_incr);
        input_turn_prev = trn_val;

        // Determine the modified turn to allow full turning capability
        // at 50% and higher speed
        const double mod_trn = constrain_input(fwd_val * 2) * trn_val;

        // Set the turn rates and forward speed rates
        const double turn_rate = 0.02 * mod_trn;
        const double forward_speed = 1.5 * fwd_val;

        // Increment the rotation
        rotation = std::fmod(rotation + turn_rate, 2 * PI);

        // Increment the position
        x += std::cos(rotation) * forward_speed;
        y += std::sin(rotation) * forward_speed;

        // Increment the distance value
        distance += forward_speed;

        // Update the average speed and step count
        average_speed = (average_speed * car_step_count + forward_speed) / (car_step_count + 1.0);
        car_step_count += 1;

        // Check for collision
        check_collision(grid);

        // Update sensor values
        update_all_sensors(grid);
    }
}

double Car::get_rotation() const
{
    return rotation;
}

double Car::get_car_x() const
{
    return x;
}

double Car::get_car_y() const
{
    return y;
}

void Car::check_collision(const RoadGrid& tile_grid)
{
    // Define the number of corners
    const size_t num_corners = 4;

    // Define the corner positions
    const double corner_lon[num_corners] =
    {
        get_width() / 2.0,
        get_width() / 2.0,
        -get_width() / 2.0,
        -get_width() / 2.0
    };

    const double corner_lat[num_corners] =
    {
        get_height() / 2.0,
        -get_height() / 2.0,
        get_height() / 2.0,
        -get_height() / 2.0
    };

    // Loop over each corner
    for (size_t i = 0; i < num_corners; ++i)
    {
        // Extract the current corner position by a rotation DCM
        const double corner_x = x + rot_vec_lon(corner_lon[i], corner_lat[i]);
        const double corner_y = y + rot_vec_lat(corner_lon[i], corner_lat[i]);

        // Extract the current tile, and check that it is valid
        const RoadGrid::GridLoc* t = tile_grid.get_for_xy(corner_x, corner_y);

        if (t == nullptr)
        {
            collided = true;
            break;
        }

        // Check if the point is on the road tile
        if (!t->tile->point_on_road(corner_x - t->x, corner_y - t->y))
        {
            collided = true;
            break;
        }
    }
}

void Car::reset()
{
    // Reset collision state and distance
    collided = false;
    distance = 0.0;

    // Reset rotation and x/y state
    rotation = std::fmod(start_rot, 2 * PI);
    x = start_x;
    y = start_y;

    // Reset velocity state
    input_forward_prev = 0.0;
    input_turn_prev = 0.0;

    // Reset the velocity state
    car_step_count = 0;
    average_speed = 0.0;
}

double Car::get_width() const
{
    return 60.0;
}

double Car::get_height() const
{
    return 30.0;
}

bool Car::has_collided() const
{
    return collided;
}

double Car::get_distance() const
{
    return distance;
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

double Car::rot_vec_lon(const double lon, const double lat) const
{
    return lon * std::cos(rotation) - lat * std::sin(rotation);
}

double Car::rot_vec_lat(const double lon, const double lat) const
{
    return lon * std::sin(rotation) + lat * std::cos(rotation);
}

const Car::SensorResult& Car::get_sensor(const size_t sensor_num) const
{
    if (sensor_num < sensor_count())
    {
        return sensors[sensor_num].result;
    }
    else
    {
        throw std::out_of_range("sensor index is out of range");
    }
}

size_t Car::sensor_count() const
{
    return sensors.size();
}

double Car::get_forward_input() const
{
    return input_forward_prev;
}

double Car::get_average_speed() const
{
    return average_speed;
}

uint64_t Car::get_step_count() const
{
    return car_step_count;
}

void Car::update_all_sensors(const RoadGrid& tile_grid)
{
    for (size_t i = 0; i < sensors.size(); ++i)
    {
        // Extract the sensor
        Sensor& sensor = sensors[i];

        // Find the front of the car
        const double front_sign = sensor.is_front ? 1.0 : -1.0;
        const double origin_x = x + rot_vec_lon(get_width() / 2.0 * front_sign, 0);
        const double origin_y = y + rot_vec_lat(get_width() / 2.0 * front_sign, 0);

        // Find the vector/slope to find the point out the front of the car
        const double dmag = std::sqrt(std::pow(sensor.delta_lon, 2.0) + std::pow(sensor.delta_lat, 2.0));
        const double dlon_rot = rot_vec_lon(sensor.delta_lon, sensor.delta_lat) / dmag;
        const double dlat_rot = rot_vec_lat(sensor.delta_lon, sensor.delta_lat) / dmag;

        // Determine the increment to use in searching
        const double incr = 2;

        double xval = origin_x;
        double yval = origin_y;

        // Define the sensor max range
        const double sensor_max = 50.0;

        // Iterate up to the sensor max range
        double current_dist = 0.0;
        while (current_dist < sensor_max)
        {
            xval = origin_x + dlon_rot * current_dist;
            yval = origin_y + dlat_rot * current_dist;

            const RoadGrid::GridLoc* loc = tile_grid.get_for_xy(xval, yval);
            if (loc == nullptr)
            {
                break;
            }
            else if (!loc->tile->point_on_road(xval - loc->x, yval - loc->y))
            {
                break;
            }

            current_dist += incr;
        }

        // Define the sensor result values
        sensor.result.start_x = origin_x;
        sensor.result.start_y = origin_y;
        sensor.result.impact_x = xval;
        sensor.result.impact_y = yval;
        sensor.result.dist = current_dist;
    }
}
