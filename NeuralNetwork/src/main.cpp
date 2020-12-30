#include <iostream>

#include "neural/net.h"
#include "neural/neural_exception.h"

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

int main()
{
    // Define the Neural Network
    NeuralNetwork net;

    // Define the car
    Car car;
    const size_t num_forward_outputs = 10;
    const size_t num_turn_outputs = 10;

    // Define the layer sizes
    std::vector<size_t> layer_sizes = { car.sensor_count(), 3 * car.sensor_count(), num_forward_outputs + num_turn_outputs };

    // Create the layer
    try
    {
        net = NeuralNetwork::from_layers(layer_sizes);
    }
    catch (neural_exception& e)
    {
        std::cerr << "Neural Exception: " << e.what() << std::endl;
        return 1;
    }

    // Define the genetic algorithm
    GeneticOptim optim(50, net.get_links().size());
    optim.init_population();

    // Initialize Allegro
    if (!al_init() ||
        !al_install_keyboard() ||
        !al_init_primitives_addon() ||
        !al_init_font_addon() ||
        !al_init_ttf_addon())
    {
        std::cerr << "Unable to initialize Allegro" << std::endl;
    }

    // Initialize the road grid
    RoadGrid grid(6, 4);

    grid.set(0, 5, RoadTileManager::RoadTileType::CORNER_SW);
    grid.set(0, 4, RoadTileManager::RoadTileType::STRAIGHT_H);
    grid.set(0, 3, RoadTileManager::RoadTileType::STRAIGHT_H);
    grid.set(0, 2, RoadTileManager::RoadTileType::CORNER_SE);
    grid.set(1, 2, RoadTileManager::RoadTileType::CORNER_NW);
    grid.set(1, 1, RoadTileManager::RoadTileType::STRAIGHT_H);
    grid.set(1, 0, RoadTileManager::RoadTileType::CORNER_SE);
    grid.set(2, 0, RoadTileManager::RoadTileType::STRAIGHT_V);
    grid.set(3, 0, RoadTileManager::RoadTileType::CORNER_NE);
    grid.set(3, 1, RoadTileManager::RoadTileType::STRAIGHT_H);
    grid.set(3, 2, RoadTileManager::RoadTileType::STRAIGHT_H);
    grid.set(3, 3, RoadTileManager::RoadTileType::CORNER_NW);
    grid.set(2, 3, RoadTileManager::RoadTileType::CORNER_SE);
    grid.set(2, 4, RoadTileManager::RoadTileType::CORNER_SW);
    grid.set(3, 4, RoadTileManager::RoadTileType::CORNER_NE);
    grid.set(3, 5, RoadTileManager::RoadTileType::CORNER_NW);
    grid.set(2, 5, RoadTileManager::RoadTileType::STRAIGHT_V);
    grid.set(1, 5, RoadTileManager::RoadTileType::STRAIGHT_V);

    grid.set_start_ind(2, 0);

    // Define the car
    car.init_bitmap();

    RoadGrid::GridLoc* start_pos = grid.at(grid.get_start_ind());
    car.set_pos(start_pos->get_center_x(), start_pos->get_center_y());
    car.set_rotation(3 * car.PI / 2);

    // Initialize the display
    ALLEGRO_DISPLAY* display = al_create_display(
        RoadTile::TILE_SIZE * grid.get_width(),
        RoadTile::TILE_SIZE * grid.get_height());
    al_set_window_title(display, "Neural Network");

    // Initialize the event queue and add events to the main queue
    ALLEGRO_EVENT_QUEUE* queue = al_create_event_queue();
    al_register_event_source(queue, al_get_keyboard_event_source());
    al_register_event_source(queue, al_get_display_event_source(display));

    // Define an event timer for loop
    ALLEGRO_TIMER* main_timer = al_create_timer(1.0 / 60.0);
    al_register_event_source(queue, al_get_timer_event_source(main_timer));
    al_start_timer(main_timer);

    // Define an event timer for the physics step
    double timer_divide_val = 100.0;
    ALLEGRO_TIMER* car_step_timer = al_create_timer(1.0 / timer_divide_val);
    al_register_event_source(queue, al_get_timer_event_source(car_step_timer));
    al_start_timer(car_step_timer);

    // Load the font
    ALLEGRO_FONT* font = al_load_ttf_font("DejaVuSans.ttf", 20, 0);
    if (font == nullptr)
    {
        std::cerr << "Unable to load font!" << std::endl;
        return 1;
    }

    // Define the Roadtile
    RoadTileManager manager;
    
    // Reset the display buffer
    al_set_target_bitmap(al_get_backbuffer(display));

    // Current Population Index
    size_t population_index = 0;
    size_t current_step = 0;
    bool update_population = true;

    // Define parameter values
    double best_distance_so_far = 0.0;
    NeuralNetwork best_so_far = net;
    bool best_selected = false;
    size_t best_count = 0;

    // Define the file net
    NeuralNetwork file_net;
    bool file_net_read = false;
    bool use_file_value = false;
    const std::string output_fname = "default.txt";

    try
    {
        std::ifstream input_vals(output_fname);
        std::ostringstream config;
        std::string l;
        while (std::getline(input_vals, l))
        {
            config << l << std::endl;
        }

        file_net = NeuralNetwork::from_config(config.str());

        file_net_read = true;
    }
    catch (const std::invalid_argument& err)
    {
        file_net_read = false;
        std::cerr << "Unable to read input file: " << err.what() << std::endl;
    }

    if (file_net_read)
    {
        use_file_value = true;
        best_selected = true;
    }

    // Define Status Values
    double input_forward = 0.0;
    double input_right = 0.0;

    // Define a loop for running
    bool running = true;
    size_t loop_val = 0;
    while (running)
    {
        // Define the event
        ALLEGRO_EVENT event;
        al_wait_for_event(queue, &event);

        loop_val = (loop_val + 1) % 1000;

        if (loop_val == 0)
        {
            al_flush_event_queue(queue);
        }

        // Switch cast
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
                if (timer_divide_val < 100000.0)
                {
                    timer_divide_val *= 10.0;
                    al_set_timer_speed(car_step_timer, 1.0 / timer_divide_val);
                }
                break;
            case ALLEGRO_KEY_DOWN:
                if (timer_divide_val > 100.0)
                {
                    timer_divide_val /= 10.0;
                    al_set_timer_speed(car_step_timer, 1.0 / timer_divide_val);
                }
                break;
            case ALLEGRO_KEY_HOME:
                if (best_selected)
                {
                    car.reset();
                }
                break;
            case ALLEGRO_KEY_ENTER:
                best_selected = !best_selected;
                car.reset();
                update_population = true;
                break;
            case ALLEGRO_KEY_F:
                if (best_selected && file_net_read)
                {
                    use_file_value = !use_file_value;
                    car.reset();
                    update_population = true;
                }
            }
            break;
        case ALLEGRO_EVENT_TIMER:
            if (event.timer.source == main_timer)
            {
                for (size_t w = 0; w < grid.get_width(); ++w)
                {
                    for (size_t h = 0; h < grid.get_height(); ++h)
                    {
                        RoadGrid::GridLoc* loc = grid.at(h, w);
                        al_draw_bitmap(
                            loc->tile->get_bitmap(),
                            loc->x,
                            loc->y,
                            0);
                    }
                }

                // Draw the car sensor lines
                for (size_t i = 0; i < car.sensor_count(); ++i)
                {
                    Car::SensorResult s = car.get_sensor(grid, i);
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
                    car.get_width() / 2,
                    car.get_height() / 2,
                    car.get_car_x(),
                    car.get_car_y(),
                    car.get_rotation(),
                    0);

                // Define the output string
                std::ostringstream status_str;
                status_str << "Generation " << optim.get_generation() << ", Test " << population_index;

                // Draw Text
                al_draw_text(
                    font,
                    al_map_rgb(0, 0, 0),
                    10,
                    0,
                    0,
                    status_str.str().c_str());

                status_str.str("");
                status_str.clear();
                status_str << "Forward: " << std::setprecision(3) << input_forward;

                al_draw_text(
                    font,
                    al_map_rgb(0, 0, 0),
                    10,
                    20,
                    0,
                    status_str.str().c_str());

                status_str.str("");
                status_str.clear();
                status_str << "Right: " << std::setprecision(3) << input_right;

                al_draw_text(
                    font,
                    al_map_rgb(0, 0, 0),
                    10,
                    40,
                    0,
                    status_str.str().c_str());

                std::string optim_str = (best_selected) ? (use_file_value ? "File" : "Best Selected") : "Optimizing";
                al_draw_text(
                    font,
                    al_map_rgb(0, 0, 0),
                    10,
                    60,
                    0,
                    optim_str.c_str());

                status_str.str("");
                status_str.clear();
                status_str << "Distance: ";

                if (best_selected)
                {
                    status_str << std::setprecision(3) << car.get_distance();
                }
                else
                {
                    status_str << std::setprecision(3) << best_distance_so_far;
                }

                al_draw_text(
                    font,
                    al_map_rgb(0, 0, 0),
                    10,
                    80,
                    0,
                    status_str.str().c_str());

                // Flip the Display Buffer
                al_flip_display();
            }
            else if (event.timer.source == car_step_timer)
            {
                // Update the population
                NeuralNetwork* selected_net;
                if (best_selected)
                {
                    if (use_file_value)
                    {
                        selected_net = &file_net;
                    }
                    else
                    {
                        selected_net = &best_so_far;
                    }
                }
                else
                {
                    selected_net = &net;

                    if (update_population)
                    {
                        // Perform the optimization step if necessary
                        if (population_index >= optim.population_size())
                        {
                            optim.update_population();
                            population_index = 0;
                        }

                        // Set the gain values to the current population design variables
                        const std::vector<double>& current_desvars = optim.get_population(population_index);
                        for (size_t i = 0; i < optim.design_variable_size(); ++i)
                        {
                            net.get_links()[i].set_gain(current_desvars[i]);
                        }

                        // Reset State
                        update_population = false;
                        current_step = 0;
                        car.reset();
                    }
                }

                // Set the event values
                for (size_t i = 0; i < car.sensor_count(); ++i)
                {
                    selected_net->set_input(i, car.get_sensor(grid, i).dist);
                }

                // Step the net
                if (!selected_net->step())
                {
                    throw std::exception("unable to step network");
                }

                // Set the outputs
                input_forward = 0.0;
                input_right = 0.0;

                for (size_t i = 0; i < num_forward_outputs; ++i)
                {
                    double val;
                    const double add_val = 1.0 / num_forward_outputs;
                    const double thresh = 0.5;
                    if (selected_net->get_output(0 + i, val) && val > 0.5)
                    {
                        if (val > thresh)
                        {
                            input_forward += add_val;
                        }
                        else if (val < -thresh)
                        {
                            input_forward -= add_val;
                        }
                    }
                }

                for (size_t i = 0; i < num_turn_outputs; ++i)
                {
                    double val;
                    const double add_val = 1.0 / num_turn_outputs;
                    const double thresh = 0.5;
                    if (selected_net->get_output(num_forward_outputs + i, val))
                    {
                        if (val > thresh)
                        {
                            input_right += add_val;
                        }
                        else if (val < thresh)
                        {
                            input_right -= add_val;
                        }
                    }
                }

                // Step the car
                car.step(input_forward, input_right);
                car.check_collision(grid);

                const bool is_stuck = std::abs(input_forward) < 1e-6 && std::abs(car.get_forward_vel()) < 1e-6;

                current_step += 1;

                if (best_selected)
                {
                    if (car.has_collided())
                    {
                        //car.reset();
                    }
                }
                else
                {
                    if (car.has_collided() || current_step > 300 * 100 || is_stuck)
                    {
                        if (car.get_distance() > best_distance_so_far)
                        {
                            std::cout << "D: " << car.get_distance() << std::endl;
                            best_distance_so_far = car.get_distance();
                            best_so_far = net;

                            // Save the best-so-far network

                            std::ostringstream fname;
                            fname << output_fname << "." << best_count;

                            std::ofstream output(fname.str());
                            if (output.is_open())
                            {
                                std::cout << "  Wrote network config " << fname.str() << std::endl;
                                output << best_so_far.get_config();
                                output.close();
                            }
                            else
                            {

                                std::cerr << "Error opening file " << fname.str() << " for writing" << std::endl;
                            }

                            best_count += 1;

                        }
                        optim.set_result(population_index, car.get_distance());
                        population_index += 1;
                        update_population = true;
                    }
                }
            }
            break;
        }
    }

    // Destroy Windows and Queue
    al_destroy_display(display);
    al_destroy_event_queue(queue);
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
