#include <iostream>

#include "neural/net.h"
#include "neural/neural_exception.h"

#include "tiles/road_tile.h"
#include "tiles/tile_manager.h"
#include "tiles/tile_grid.h"

#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>

#include "car/car.h"

int main()
{
    // Define the Neural Network
    NeuralNetwork net;

    // Define the layer sizes
    std::vector<size_t> layer_sizes = { 5, 7, 4 };

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

    // Set Links to have unity gain
    std::vector<NeuralLink>& links = net.get_links();
    for (size_t i = 0; i < links.size(); ++i)
    {
        links[i].set_gain(1.0);
    }

    // Set an initial input
    net.set_input(0, 1.0);

    // Define Success
    std::cout << "Neural Network Successfully Created" << std::endl;

    // Define an initial output
    std::cout << "Step 0" << std::endl << net.get_status() << std::endl;

    // Step
    net.step();

    // Print Output
    std::cout << "Step 1" << std::endl << net.get_status() << std::endl;

    // Initialize Allegro
    if (!al_init() ||
        !al_install_keyboard() ||
        !al_init_primitives_addon())
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
    Car car;
    car.init_bitmap();

    RoadGrid::GridLoc* start_pos = grid.at(grid.get_start_ind());
    car.set_pos(start_pos->get_center_x(), start_pos->get_center_y());
    car.set_rotation(3 * car.PI / 2);

    // Initialize the display
    ALLEGRO_DISPLAY* display = al_create_display(
        RoadTile::TILE_SIZE * grid.get_width(),
        RoadTile::TILE_SIZE * grid.get_height());

    // Initialize the event queue and add events to the main queue
    ALLEGRO_EVENT_QUEUE* queue = al_create_event_queue();
    al_register_event_source(queue, al_get_keyboard_event_source());
    al_register_event_source(queue, al_get_display_event_source(display));

    // Define an event timer for loop
    ALLEGRO_TIMER* main_timer = al_create_timer(1.0 / 30.0);
    al_register_event_source(queue, al_get_timer_event_source(main_timer));
    al_start_timer(main_timer);

    // Define an event timer for the physics step
    ALLEGRO_TIMER* car_step_timer = al_create_timer(1.0 / 100.0);
    al_register_event_source(queue, al_get_timer_event_source(car_step_timer));
    al_start_timer(car_step_timer);

    // Define the Roadtile
    RoadTileManager manager;
    
    // Reset the display buffer
    al_set_target_bitmap(al_get_backbuffer(display));

    // Add go factor
    double go_factor = 0.0;
    double turn_val = 0.0;

    // Define a loop for running
    bool running = true;
    while (running)
    {
        // Define the event
        ALLEGRO_EVENT event;
        al_wait_for_event(queue, &event);

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
                go_factor = 1.0;
                break;
            case ALLEGRO_KEY_DOWN:
                go_factor = -1.0;
                break;
            case ALLEGRO_KEY_LEFT:
                turn_val = -1.0;
                break;
            case ALLEGRO_KEY_RIGHT:
                turn_val = 1.0;
                break;
            }
            break;
        case ALLEGRO_EVENT_KEY_UP:
            switch (event.keyboard.keycode)
            {
            case ALLEGRO_KEY_UP:
            case ALLEGRO_KEY_DOWN:
                go_factor = 0.0;
                break;
            case ALLEGRO_KEY_LEFT:
            case ALLEGRO_KEY_RIGHT:
                turn_val = 0.0;
                break;
            }
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

                al_draw_rotated_bitmap(
                    car.get_bitmap(),
                    car.get_width() / 2,
                    car.get_height() / 2,
                    car.get_x(),
                    car.get_y(),
                    car.get_rotation(),
                    0);
                al_flip_display();
            }
            else if (event.timer.source == car_step_timer)
            {
                car.step(go_factor, turn_val);
            }
            break;
        }
    }

    // Destroy Windows and Queue
    al_destroy_display(display);
    al_destroy_event_queue(queue);
    al_destroy_timer(main_timer);
    al_destroy_timer(car_step_timer);

    display = nullptr;
    queue = nullptr;
    main_timer = nullptr;
    car_step_timer = nullptr;

    // Return Success
    return 0;
}
