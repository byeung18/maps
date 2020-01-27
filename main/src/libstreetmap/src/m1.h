#pragma once

#include <iostream>
#include <utility>
#include <vector>
#include <string>
#include <time.h>
#include <cmath>
#include <boost/unordered_map.hpp>

#include "easygl_constants.h"
#include "graphics.h"
#include "IntersectionGraph.h"
#include "StreetsDatabaseAPI.h"
#include "LatLon.h"

#include "Feature.h"
#include "Highway.h"
#include "POI.h"
#include "Street.h"

using namespace std;

extern vector<Street> streets;
//load map
bool load_map(string map_name);

//close map
void close_map();


