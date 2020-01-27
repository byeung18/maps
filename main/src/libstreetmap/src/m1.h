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
//load and close map
bool load_map(string map_name);
void cleanup_data_structures();
void close_map();

//load map info
void load_streets();
void load_intersections();
void load_poi();

bool are_directly_connected(unsigned id1, unsigned id2);

std::vector<unsigned> find_street_ids_from_name(std::string street_name);
std::vector<unsigned> find_intersection_street_segments(unsigned intersection_id);
std::vector<std::string> find_intersection_street_names(unsigned intersection_id);
std::vector<unsigned> find_adjacent_intersections(unsigned intersection_id);

std::vector<unsigned> find_street_street_segments(unsigned street_id);
std::vector<unsigned> find_all_street_intersections(unsigned street_id);
std::vector<unsigned> find_intersection_ids_from_street_names(std::string street2, std::string street2);

double find_distance_between_two_points(LatLon point1, LatLon point2);
double find_street_segment_length(unsigned segment_id)
double find_street_length(unsigned street_id);

double find_street_segment_travel_time(unsigned segment_id);
