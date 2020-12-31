#ifndef __IO_CAR__
#define __IO_CAR__

#include <allegro5/allegro.h>
#include <tiles/tile_grid.h>

#include <array>

/// <summary>
/// Defines the car state object
/// </summary>
class Car
{
public:
    /// <summary>
    /// Defines the PI constant to use for rotation parameters
    /// </summary>
    static const double PI;

    /// <summary>
    /// Provides a state response for sensor parameters
    /// </summary>
    struct SensorResult
    {
        /// <summary>
        /// Defines the starting x location for the search
        /// </summary>
        double start_x = 0.0;

        /// <summary>
        /// Defines the starting y location for the search
        /// </summary>
        double start_y = 0.0;

        /// <summary>
        /// Defines the impact x location for the search
        /// </summary>
        double impact_x = 0.0;

        /// <summary>
        /// Defines the impact y location for the search
        /// </summary>
        double impact_y = 0.0;

        /// <summary>
        /// Defines the distance between the start and the impact locations
        /// </summary>
        double dist = 0.0;
    };

public:
    /// <summary>
    /// Initializes the car object
    /// </summary>
    Car();

    /// <summary>
    /// Initializes the bitmap images. To be called after rendering library
    /// initialization has completed
    /// </summary>
    void init_bitmap();

    /// <summary>
    /// Provides the base bitmap for the car
    /// </summary>
    /// <returns>the bitmap for the car, or nullptr if not initialized</returns>
    ALLEGRO_BITMAP* get_bitmap() const;

    /// <summary>
    /// Sets the current and starting rotation value for the car
    /// </summary>
    /// <param name="rot_val"></param>
    void set_start_rotation(const double rot_val);

    /// <summary>
    /// Sets teh current and starting x/y positions for the car
    /// </summary>
    /// <param name="x">the x location to set</param>
    /// <param name="y">the y location to set</param>
    void set_pos(const double x, const double y);

    /// <summary>
    /// Steps the car state to the next timestep
    /// </summary>
    /// <param name="grid">the road grid to check for collision against</param>
    /// <param name="forward">the forward input to use, between -1 and 1</param>
    /// <param name="turn">the right input to use, between -1 and 1</param>
    void step_movement(const RoadGrid& grid, const double forward, const double turn);

    /// <summary>
    /// Gets the current car rotation
    /// </summary>
    /// <returns>the car rotation, in radians</returns>
    double get_rotation() const;

    /// <summary>
    /// Provides the current car X location
    /// </summary>
    /// <returns>the car x location</returns>
    double get_car_x() const;

    /// <summary>
    /// Provides the current car Y location
    /// </summary>
    /// <returns>the car y location</returns>
    double get_car_y() const;

    /// <summary>
    /// Resets the car state to the starting position values and any other
    /// associated flags to state parameters
    /// </summary>
    void reset();

    /// <summary>
    /// Gets the car width
    /// </summary>
    /// <returns>the car width, in pixels</returns>
    double get_length() const;

    /// <summary>
    /// Gets the car height
    /// </summary>
    /// <returns>the car height, in pixels</returns>
    double get_width() const;

    /// <summary>
    /// Determines if the car has collided with something
    /// </summary>
    /// <returns>true if the car has collided with something</returns>
    bool has_collided() const;

    /// <summary>
    /// Obtains the distance the car has traveled since the last reset
    /// </summary>
    /// <returns>the distance the car has traveled since the last reset</returns>
    double get_distance() const;

    /// <summary>
    /// Obtains the sensor result value for the given sensor number and road grid
    /// </summary>
    /// <param name="sensor_num">the sensor number of check</param>
    /// <returns>the associated sensor result value</returns>
    const SensorResult& get_sensor(const size_t sensor_num) const;

    /// <summary>
    /// Determines the number of sensors to use in checking
    /// </summary>
    /// <returns></returns>
    size_t sensor_count() const;

    /// <summary>
    /// Returns the current forward input setting for the car
    /// </summary>
    /// <returns>the forward input</returns>
    double get_forward_input() const;

    /// <summary>
    /// Returns the average speed since reset
    /// </summary>
    /// <returns>the average speed since the last reset</returns>
    double get_average_speed() const;

    /// <summary>
    /// Returns the step count since reset
    /// </summary>
    /// <returns>the step count since the last reset</returns>
    uint64_t get_step_count() const;

public:
    ~Car();

protected:
    /// <summary>
    /// Defines parameters used for sensor calculations
    /// </summary>
    struct Sensor
    {
        /// <summary>
        /// Defines the longitudinal delta for the sensor slope
        /// </summary>
        double delta_lon = 0.0;

        /// <summary>
        /// Defines the lateral delta for the sensor slope
        /// </summary>
        double delta_lat = 0.0;

        /// <summary>
        /// Defines whether the sensor is at the front or back of the car
        /// </summary>
        bool is_front = true;

        /// <summary>
        /// Defines the sensor result for the sensor
        /// </summary>
        SensorResult result;
    };

    /// <summary>
    /// Checks if the car has collided with something on the given road grid
    /// for it's current location
    /// </summary>
    /// <param name="grid">the road grid to check for collision against</param>
    void check_collision(const RoadGrid& grid);

    /// <summary>
    /// Updates all sensor values for the car with the current grid
    /// </summary>
    /// <param name="tile_grid">the tile grid to check for distance</param>
    void update_all_sensors(const RoadGrid& tile_grid);

    /// <summary>
    /// Constrains any input values to be within -1 and 1
    /// </summary>
    /// <param name="val">the value to constrain</param>
    /// <returns>the constrained value</returns>
    double constrain_input(const double val);

    /// <summary>
    /// Steps a simple rate-limiting filter with an input, the previous value,
    /// and a step rate
    /// </summary>
    /// <param name="in">the input value</param>
    /// <param name="prev">the previous value</param>
    /// <param name="rate">the maximum rate to move at</param>
    /// <returns>the filtered result</returns>
    double step_filter(const double in, const double prev, const double rate);

    /// <summary>
    /// Provides the longitudinal/x component of a rotation vector
    /// for the current car rotation for the longitudinal and lateral
    /// delta positions in car-coordinates from the car's center
    /// </summary>
    /// <param name="lon">the longitudinal delta from car center, forward-positive</param>
    /// <param name="lat">the lateral delta from car center, right-positive</param>
    /// <returns>the rotation vector x component</returns>
    double rot_vec_lon(const double lon, const double lat) const;

    /// <summary>
    /// Provides the lateral/y component of a rotation vector
    /// for the current car rotation for the longitudinal and lateral
    /// delta positions in car-coordinates from the car's center
    /// </summary>
    /// <param name="lon">the longitudinal delta from car center, forward-positive</param>
    /// <param name="lat">the lateral delta from car center, right-positive</param>
    /// <returns>the rotation vector y component</returns>
    double rot_vec_lat(const double lon, const double lat) const;

protected:
    /// <summary>
    /// Defines the current Y location for the car
    /// </summary>
    double x = 0.0;

    /// <summary>
    /// Defines the current X location for the car
    /// </summary>
    double y = 0.0;

    /// <summary>
    /// Defines the current rotation angle for the car, in radians
    /// </summary>
    double rotation = 0.0;

    /// <summary>
    /// Defines the starting X location for the car
    /// </summary>
    double start_x = 0.0;

    /// <summary>
    /// Defines the starting Y location for the car
    /// </summary>
    double start_y = 0.0;

    /// <summary>
    /// Defines the starting rotation angle for the car, in radians
    /// </summary>
    double start_rot = 0.0;

    /// <summary>
    /// Defines the distance the car has traveled since it was last reset
    /// </summary>
    double distance = 0.0;

    /// <summary>
    /// Defines the previous input used to track forward movement
    /// </summary>
    double input_forward_prev = 0.0;

    /// <summary>
    /// Defines the previous input used to track turning
    /// </summary>
    double input_turn_prev = 0.0;

    /// <summary>
    /// Tracks the number of times the car has stepped since it was last
    /// reset
    /// </summary>
    uint64_t car_step_count = 0;

    /// <summary>
    /// Stores an overall average of the car's speed
    /// </summary>
    double average_speed = 0.0;

    /// <summary>
    /// Flag to track if the car has collided with something or not
    /// </summary>
    bool collided = false;

    /// <summary>
    /// Provides the overall bitmap for the car
    /// </summary>
    ALLEGRO_BITMAP* bitmap = nullptr;

    /// <summary>
    /// Defines the number of sensors provided
    /// </summary>
    const static size_t num_sensors = 7;

    /// <summary>
    /// Defines the sensor values associated with the car
    /// </summary>
    std::array<Sensor, num_sensors> sensors;
};

#endif
