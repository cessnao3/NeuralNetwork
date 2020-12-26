#include <iostream>

#include "neuralnet.h"

#include <allegro5/allegro.h>

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
    if (!al_init() || !al_install_keyboard())
    {
        std::cerr << "Unable to initialize Allegro" << std::endl;
    }

    // Initialize the display
    ALLEGRO_DISPLAY* display = al_create_display(1024, 600);

    // Initialize the event queue and add events to the main queue
    ALLEGRO_EVENT_QUEUE* queue = al_create_event_queue();
    al_register_event_source(queue, al_get_keyboard_event_source());
    al_register_event_source(queue, al_get_display_event_source(display));

    // Define an event timer for loop
    ALLEGRO_TIMER* main_timer = al_create_timer(1.0 / 30.0);
    al_register_event_source(queue, al_get_timer_event_source(main_timer));
    al_start_timer(main_timer);

    // Define a loop for running
    bool running = true;
    bool is_white = true;
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
            if (event.keyboard.keycode == ALLEGRO_KEY_ESCAPE)
            {
                running = false;
            }
            else if (event.keyboard.keycode == ALLEGRO_KEY_SPACE)
            {
                is_white = !is_white;
            }
            break;
        case ALLEGRO_EVENT_TIMER:
            if (event.timer.source == main_timer)
            {
                if (is_white)
                {
                    al_clear_to_color(al_map_rgb(255, 255, 255));
                }
                else
                {
                    al_clear_to_color(al_map_rgb(0, 0, 0));
                }

                al_flip_display();
            }
            break;
        }
    }

    // Destroy Windows and Queue
    al_destroy_display(display);
    al_destroy_event_queue(queue);
    al_destroy_timer(main_timer);

    display = nullptr;
    queue = nullptr;
    main_timer = nullptr;

    // Return Success
    return 0;
}
