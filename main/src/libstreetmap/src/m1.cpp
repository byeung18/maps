#define SIDEROADLENGTH 2000 // length is in metres
#define MAINROADLENGTH 5000

#include <cmath>
#include "m1.h"
//#include "m2.h"
//#include "m3.h"
//#include "m4.h"

using namespace std;

// We want to use the unordered map to reduce computation time



// These global variables will be needed in multiple functions.


// load the map
bool load_map(string map_name) /*done*/ {

return 0;
}

//close the map

void close_map() {
    closeStreetDatabase();
    cout << "INFO: Window closed" << endl;
    // destroy any data structures you created in load_map
    // ...
}

// Finds the average speed limit for a given street
double averageSpeed(unsigned streetID) {

return 0;
}

//function to return intersection id for an intersection name

unsigned find_intersection_id_from_name(string intersection_name) /*TESTED*/ {

    // This function implements hashing of the input name identical to that done in load map
    // As a result we can simply index into the unordered map to receive the street ID
    // Cascading if statements are used to sort the street name in descending order if there are three involved
    //


return 0;
}

// find street ID from name

unsigned find_street_id_from_name(string street_name)  {
    //Remember that hack we did in load_map? Well here it is!

	return 0;
} /*done*/

vector<unsigned> find_intersection_street_segments(string intersection_name) /*TESTED*/ {
    //convert the intersection name to an id and use the other find_intersection_street_segments function
    //
    vector<unsigned> empty;
    return empty;
}

//get the IDs of all street segments meeting at a given intersection

vector<unsigned> find_intersection_street_segments(unsigned intersection_id) /*TESTED*/ {
    // MOST LIKELY THE PREVIOUS FUNCTION CAN CALL THIS ONE and that saves some space in coding...

    // 1. call getIntersectionStreetSegmentCount(unsigned intersectionID) from the API
    // to set size of the vector
    // 2. create vector of this size
    // 3. using some sort of loop, call getIntersectionStreetSegment(unsigned intersectionID,unsigned idx)
    // from the API to get the street segment ID and add to the vector
    // 4. return the vector

    vector<unsigned> empty;
    return empty;
}

//find distance between two coordinates
double find_distance_between_two_points(LatLon point1, LatLon point2) /*TESTED*/ {
	double avglat = (point1.lat + point2.lat) / 2;
	double x1 = point1.lon + cos(avglat);
	double x2 = point2.lon + cos(avglat);
	double y1 = point1.lat;
   	double y2 = point2.lat;

	return sqrt(pow((y1 - y2), 2) + pow((x1 - x2), 2));
}

//function to return street names at an intersection, given its name

vector<string> find_intersection_street_names(string intersection_name) /*TESTED*/ {

    vector<string> empty;
    return empty;
}


//If we want the street names, simply get a vector of IDs and convert them.
vector<string> find_intersection_street_names(unsigned intersection_id) {
  

    vector<string> empty;
    return empty;
}

vector<unsigned> find_intersection_street_IDs(unsigned intersection_id) /*TESTED*/ {


    vector<unsigned> empty;
    return empty;
}

//can you get from intersection1 to intersection2 using a single street segment (hint: check for 1-way streets too)

bool are_directly_connected(string intersection_name1, string intersection_name2) /*FINISHED, NOT TESTED*/ {
	vector<unsigned> adjacentInts = find_adjacent_intersections(intersection_name1); // find the intersections directly connected to intersection 1

	return 0;
} //NOT TESTED

//find all intersections connected by one street segment from given intersection (hint: check for 1-way streets too)

vector<unsigned> find_adjacent_intersections(string intersection_name) {
  
    vector<unsigned> empty;
    return empty;
} /*NOT DONE*/

//for a given street, return all the street segments
vector<unsigned> find_street_street_segments(string street_name) /*TESTED*/ {

    vector<unsigned> empty;
    return empty;
}

vector<unsigned> find_street_street_segments_from_id(unsigned int id) {

    vector<unsigned> empty;
    return empty;
}

//for a given street, find all the intersections

vector<unsigned> find_all_street_intersections(string street_name) {
    // 0. create the intersection vector
    // 1. call the above thing to get all the street segments (creates a segment vector)
    // 2. go through the vector - call getStreetSegmentEnds(unsigned streetSegmentID) on each segment
    // 3. check if duplicate: if not duplicated, add to intersection vector
    // 4. return intersection vector

    vector<unsigned> empty;
    return empty;
} //NOT FINISHED

//find the length of a given street segments

double find_street_segment_length(unsigned street_segment_id) /*TESTED*/ {

	return 0;
} // done

//find the length of a whole street

double find_street_length(string street_name) /*TESTED*/ {
 
       return 0;	
} // done

double find_street_length_from_id(unsigned int id) {
    
	return 0;
}

//find the travel time to drive a street segment (time(minutes) = distance(Km)/speed_limit(Km/h)*60)

double find_segment_travel_time(unsigned street_segment_id) /*TESTED*/ {
    // I couldn't find the units explicitly stated for anything, but I assume that
    // the distances were given in KM and the speed limits would be KM/H
    // I'm guessing that the "60" there in the above comment converts it to minutes
    // But running the unit tests led me off by a factor of 1000
    // So I'm guessing the distance was originally in meters
    // in any case, it passes the unit tester now
    // the 0.060 takes into account that discrepancy of M -> KM conversion and HOUR -> MIN
    // I'd have defined it but honestly I have no idea why this is
    // If only people told me what units for distances/ speed limits were.. then
    // there wouldn't be use for this magic number and this humongous comment

	return 0;
   }

//find the nearest point of interest (by name) to a given position

unsigned int find_closest_point_of_interest(LatLon my_position) {


	return 0;
} // NOT TESTED
