#include <iostream>

#include "neural/net.h"

#include "states/game_state.h"

#include "tiles/road_tile.h"
#include "tiles/tile_manager.h"
#include "tiles/tile_grid.h"

#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>

#include "car/car.h"

#include "optim/genetic.h"

#include <sstream>
#include <string>

#include <fstream>

#include <iomanip>

void draw_background_bitmap_for_state(const GameState& state, ALLEGRO_BITMAP* background)
{
    ALLEGRO_BITMAP* prev_target = al_get_target_bitmap();

    al_set_target_bitmap(background);
    al_clear_to_color(al_map_rgb(0, 0, 0));

    const RoadGrid* grid = state.get_tile_grid();

    for (size_t w = 0; w < grid->get_width(); ++w)
    {
        for (size_t h = 0; h < grid->get_height(); ++h)
        {
            const RoadGrid::GridLoc* loc = grid->at(h, w);
            ALLEGRO_BITMAP* bmp = loc->tile->get_bitmap();
            al_draw_bitmap(
                bmp,
                loc->x,
                loc->y,
                0);
        }
    }

    const RoadGrid::GridLoc* start_loc = grid->at(grid->get_start_ind());

    al_draw_filled_circle(
        start_loc->get_center_x(),
        start_loc->get_center_y(),
        RoadTile::ROAD_WIDTH / 4.0,
        al_map_rgb(5, 104, 57));

    al_set_target_bitmap(prev_target);
}

ALLEGRO_DISPLAY* create_display(const GameState& state, bool fullscreen, ALLEGRO_TRANSFORM* transform)
{
    // Define the default scale factor
    double scale_factor = 0.0;

    // Apply options and adjust parameters for fullscreen or not
    if (fullscreen)
    {
        al_set_new_display_flags(ALLEGRO_FULLSCREEN);
        scale_factor = 1.0;
    }
    else
    {
        al_set_new_display_flags(ALLEGRO_WINDOWED);
        scale_factor = 0.5;
    }

    // Create the display
    ALLEGRO_DISPLAY* display = al_create_display(
        static_cast<int>(state.get_screen_width() * scale_factor),
        static_cast<int>(state.get_screen_height() * scale_factor));
    al_set_window_title(display, "Neural Network");

    // Update the transformation
    al_identity_transform(transform);
    al_scale_transform(transform, scale_factor, scale_factor);
    al_use_transform(transform);

    // Return the display
    return display;
}

int main()
{
    // Initialize Allegro
    if (!al_init() ||
        !al_install_keyboard() ||
        !al_init_primitives_addon() ||
        !al_init_font_addon() ||
        !al_init_ttf_addon())
    {
        std::cerr << "Unable to initialize Allegro" << std::endl;
    }

    // Define the game state
    GameState state;
    state.init_bitmaps();
    //state.set_tile_grid_index(0);

    // Initialize the display
    bool is_fullscreen = false;
    ALLEGRO_TRANSFORM display_transform;
    ALLEGRO_DISPLAY* display = create_display(
        state,
        is_fullscreen,
        &display_transform);

    // Define the background display
    ALLEGRO_BITMAP* background_bitmap = al_create_bitmap(
        static_cast<int>(state.get_screen_width()),
        static_cast<int>(state.get_screen_height()));

    // Draw the background display
    draw_background_bitmap_for_state(state, background_bitmap);

    // Initialize the event queue and add events to the main queue
    ALLEGRO_EVENT_QUEUE* queue = al_create_event_queue();
    al_register_event_source(queue, al_get_keyboard_event_source());
    al_register_event_source(queue, al_get_display_event_source(display));

    // Define a timer event queue
    ALLEGRO_EVENT_QUEUE* timer_queue = al_create_event_queue();

    // Define an event timer for loop
    ALLEGRO_TIMER* main_timer = al_create_timer(1.0 / 60.0);
    al_register_event_source(timer_queue, al_get_timer_event_source(main_timer));
    al_start_timer(main_timer);

    // Define an event timer for the physics step
    ALLEGRO_TIMER* car_step_timer = al_create_timer(state.base_period());
    al_register_event_source(timer_queue, al_get_timer_event_source(car_step_timer));
    al_start_timer(car_step_timer);

    // Load the font
    ALLEGRO_FONT* font = al_load_ttf_font("font/DejaVuSansMono.ttf", 20, 0);
    if (font == nullptr)
    {
        std::cerr << "Unable to load font!" << std::endl;
        return 1;
    }

    // Initialize the game bitmaps
    state.init_bitmaps();

    // Define a loop for running
    bool running = true;
    while (running)
    {
        // Define the event
        ALLEGRO_EVENT event;

        // Check if there is an input event ready
        if (!al_is_event_queue_empty(queue))
        {
            // Obtain the event
            al_wait_for_event(queue, &event);

            // Switch the event type to perform the action
            switch (event.type)
            {
            case ALLEGRO_EVENT_DISPLAY_CLOSE:
                running = false;
                break;
            case ALLEGRO_EVENT_KEY_DOWN:
                switch (event.keyboard.keycode)
                {
                case ALLEGRO_KEY_ESCAPE:
                    running = false;
                    break;
                case ALLEGRO_KEY_UP:
                    state.increment_step_frequency();
                    break;
                case ALLEGRO_KEY_DOWN:
                    state.decrement_step_frequency();
                    break;
                case ALLEGRO_KEY_HOME:
                    if (state.get_current_mode() == GameState::GameMode::BEST || state.get_current_mode() == GameState::GameMode::FILE)
                    {
                        state.reset_car();
                    }
                    break;
                case ALLEGRO_KEY_1:
                    state.set_game_mode(GameState::GameMode::OPTIM);
                    break;
                case ALLEGRO_KEY_2:
                    state.set_game_mode(GameState::GameMode::BEST);
                    break;
                case ALLEGRO_KEY_3:
                    state.set_game_mode(GameState::GameMode::FILE);
                    break;
                case ALLEGRO_KEY_S:
                    state.toggle_save_best_networks();
                    break;
                case ALLEGRO_KEY_N:
                    state.set_tile_grid_index((state.get_tile_grid_index() + 1) % state.get_tile_grid_count());
                    draw_background_bitmap_for_state(state, background_bitmap);
                    state.reset_car();
                    break;
                case ALLEGRO_KEY_P:
                    if (al_get_timer_started(car_step_timer))
                    {
                        al_stop_timer(car_step_timer);
                    }
                    else
                    {
                        al_start_timer(car_step_timer);
                    }
                    break;
                case ALLEGRO_KEY_F:
                    al_destroy_display(display);
                    is_fullscreen = !is_fullscreen;
                    display = create_display(state, is_fullscreen, &display_transform);
                }
                break;
            case ALLEGRO_EVENT_TIMER:
                /*
                if (event.timer.source == end_timer)
                {
                    running = false;
                }
                */
                break;
            }

            // Break if we have set the program to stop
            if (!running)
            {
                break;
            }
        }

        // Get the main timer event queue event
        al_wait_for_event(timer_queue, &event);

        // Switch cast
        switch (event.type)
        {
        case ALLEGRO_EVENT_TIMER:
            if (event.timer.source == main_timer)
            {
                // Draw the background
                al_draw_bitmap(background_bitmap, 0, 0, 0);

                // Extract the car
                const Car& car = state.car;

                // Draw the car sensor lines
                for (size_t i = 0; i < car.sensor_count(); ++i)
                {
                    const Car::SensorResult& s = car.get_sensor(i);
                    al_draw_line(
                        s.start_x,
                        s.start_y,
                        s.impact_x,
                        s.impact_y,
                        al_map_rgb(255, 255, 255),
                        0);
                }

                // Draw the Car
                al_draw_rotated_bitmap(
                    car.get_bitmap(),
                    car.get_length() / 2,
                    car.get_width() / 2,
                    car.get_car_x(),
                    car.get_car_y(),
                    car.get_rotation(),
                    0);

                // Define the output string
                for (size_t i = 0; i < 5; ++i)
                {
                    std::ostringstream status_str;

                    switch (i)
                    {
                    case 0:
                        switch (state.get_current_mode())
                        {
                        case GameState::GameMode::BEST:
                            status_str << "Best Network So Far";
                            break;
                        case GameState::GameMode::FILE:
                            status_str << "File Network";
                            break;
                        case GameState::GameMode::OPTIM:
                            status_str << "Optimizing Network";
                            break;
                        default:
                            status_str << "Unknown Mode";
                            break;
                        }
                        break;
                    case 1:
                        status_str << "Forward: " << std::setw(4) << state.get_input_forward();
                        break;
                    case 2:
                        status_str << "Right:   " << std::setw(4) << state.get_input_right();
                        break;
                    case 3:
                        if (state.get_current_mode() == GameState::GameMode::OPTIM)
                        {
                            status_str << "Generation " << state.optim_state.get_current_generation() << ", Test " << state.optim_state.get_current_design_index();
                        }
                        else
                        {
                            status_str << "Distance: " << state.car.get_distance();
                        }
                        break;
                    case 4:
                        if (state.get_current_mode() == GameState::GameMode::OPTIM)
                        {
                            status_str << "Best Distance: " << state.get_best_distance();
                        }
                        else if (state.get_current_mode() == GameState::GameMode::BEST)
                        {
                            status_str << "Generation: " << state.optim_state.get_best_generation();
                        }
                        break;
                    }

                    al_draw_text(
                        font,
                        al_map_rgb(0, 0, 0),
                        10,
                        20 * i,
                        0,
                        status_str.str().c_str());

                    {
                        std::ostringstream speed_text;
                        speed_text << "Average Speed: " << std::setw(5) << std::setprecision(2) << state.car.get_average_speed();
                        al_draw_text(
                            font,
                            al_map_rgb(0, 0, 0),
                            state.get_screen_width() - 10,
                            0,
                            ALLEGRO_ALIGN_RIGHT,
                            speed_text.str().c_str());
                    }

                    {
                        std::ostringstream output;
                        output << "Time Multiplier: " << state.get_frequency_multiplier();

                        al_draw_text(
                            font,
                            al_map_rgb(0, 0, 0),
                            state.get_screen_width() - 10,
                            20,
                            ALLEGRO_ALIGN_RIGHT,
                            output.str().c_str());
                    }

                    if (state.get_current_mode() == GameState::GameMode::OPTIM)
                    {
                        std::string save_text = "Save Networks ";
                        if (state.get_save_bets_networks_flag())
                        {
                            save_text += "On";
                        }
                        else
                        {
                            save_text += "Off";
                        }
                        al_draw_text(
                            font,
                            al_map_rgb(0, 0, 0),
                            state.get_screen_width() - 10,
                            40,
                            ALLEGRO_ALIGN_RIGHT,
                            save_text.c_str());
                    }

                    if (state.get_current_mode() == GameState::GameMode::OPTIM)
                    {
                        std::ostringstream output;
                        output << "Delta Distance: " << car.get_delta_distance();

                        al_draw_text(
                            font,
                            al_map_rgb(0, 0, 0),
                            state.get_screen_width() - 10,
                            60,
                            ALLEGRO_ALIGN_RIGHT,
                            output.str().c_str());
                    }
                }

                // Flip the Display Buffer
                al_flip_display();
            }
            else if (event.timer.source == car_step_timer)
            {
                state.step_state();
            }
            break;
        }
    }

    // Destroy Windows and Queue
    al_destroy_display(display);
    al_destroy_bitmap(background_bitmap);
    al_destroy_event_queue(queue);
    al_destroy_event_queue(timer_queue);
    al_destroy_timer(main_timer);
    al_destroy_timer(car_step_timer);
    al_destroy_font(font);

    // Reset Pointers
    display = nullptr;
    queue = nullptr;
    main_timer = nullptr;
    car_step_timer = nullptr;
    font = nullptr;

    // Return Success
    return 0;
}
