
#include "graphics.h"
#include "LatLon.h"
#include "OSMDatabaseAPI.h"
#include "StreetsDatabaseAPI.h"

#include <algorithm>
#include <cmath>
#include <chrono>
#include <iostream>
#include <sstream>
#include <thread>
#include <vector>

#include "m1.h"
#include "m2.h"

void draw_map() {
    init_graphics("maps", t_color(255, 255, 255, 220));
    set_drawing_buffer(OFF_SCREEN);
    
    set_keypress_input(true);
    set_mouse_move_input(true);
    line_entering_demo = true;
    
    event_loop(act_on_button_press, NULL, act_on_key_press, draw_screen);
    close_graphics();
    intersections.clear();
}
