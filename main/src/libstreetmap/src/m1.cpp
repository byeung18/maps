#define SIDEROADLENGTH 2000 // length in metres
#define MAINROADLENGTH 5000

#include <algorithm>
#include <cmath>
#include <map>
#include <string>
#include <thread>
#include <unordered_map>
#include <vector>
#include "OSMDatabaseAPI.h"
#include "StreetsDatabaseAPI.h"
#include "Streets.h"
#include "m1.h"
//#include "m2.h"
//#include "m3.h"
//#include "m4.h"

using namespace std;

Streets** streets;
double* streetSegmentTime;
double* streetSegmentLength;
vector<unsigned>* streetSegments;
vector<string>* streetSegmentNames;
vector<StreetSegmentInfo> streetSegments_new;

std::unordered_map<string, std::vector<unsigned>> name_streetID;
std::unordered_map<OSMID, std::string> street_osmid_type;

// global numbers
unsigned numberOfStreets;
unsigned numberOfIntersections;
unsigned numberOfStreetSegments;
unsigned numberOfPointsOfInterest;

// functions to be implemented:
void load_streets();
void load_intersections();
void load_poi();
void load_features();
void load_osmid();

bool load_map(string map_path) {
    // if successful load
    if (loadStreetsDatabaseBIN(map_path)) {
        auto iter = map_path.find("streets.bin");
        map_path.erase(iter);
        thread loadOSM(loadOSMDatabaseBIN, map_path);
        thread loadSearch(load_search_database);
        
        unsigned i = 0;
        numberOfStreets = getNumberOfStreets();
        numberOfIntersections = getNumberOfIntersections();
        numberOfStreetSegments = getNumberOfStreetSegments();
        streetSegments_new.resize(numberOfStreetSegments);
        
        // Ensure all pointers are null
        cleanup_data_structures();
        
        // load information
        load_streets();
        load_intersections();
        load_poi();
        load_features();
        
        // join threads then load OSMID
        loadSearch.join();
        loadOSM.join();
        load_osmid();
        
        return true;
    } else {
        return false;
    }
        
}

// set all pointers to null
void cleanup_data_structures() {
    unsigned i;
    
    if (streets != nullptr) {
        for (i = 0; i < numberOfStreets; i++) {
            if (streets[i] != nullptr) {
                delete streets[i];
                streets[i] = nullptr;
            }
        }
        delete [] streets;
        streets = nullptr;
    }

    if (streetSegmentLength != nullptr) {
        delete [] StreetSegmentLength;
        streetSegmentLength = nullptr;
    }

    if (streetSegmentTime != nullptr) {
        delete [] StreetSegmentTime;
        streetSegmentTime = nullptr;
    }

    if (streetSegments != nullptr) {
        delete [] streetSegments;
        streetSegments = nullptr;
    }

    if (streetSegmentNames != nullptr) {
        delete [] streetSegmentNames;
        pointer_streetSegmentNames = nullptr;
    }
}

void close_map() {
    //Clean-up your map related data structures here
    cleanup_data_structures();
    closeStreetDatabase();
    cout << "INFO: Window closed" << endl;
}

void load_streets() {
    unsigned i;

    streets = new Streets *[numberOfStreets];
    streetSegmentTime = new double [numberOfStreetSegments];
    streetSegmentLength = new double [numberOfStreetSegments];

    // load all streets with ID and name
    for (i = 0; i < numberOfStreets; i++) {
        streets[i] = new topLevelStreets(getStreetName(i), i);
        name_streetID[getStreetName(i)].push_back(i);
    }

    double totalDistance;
    float speed_limit;
    unsigned numCurvePoints;
    unsigned id;

    // store segment info into database
    for (i = 0; i < numberOfStreetSegments; i++) {
        StreetSegmentInfo temp = getStreetSegmentInfo(i);
        id = temp.streetID;
        //store streetSegmentInfo and StreetSegmentIDs into vector
        streets[id]->insertStreetSegmentInfo(temp);
        streets[id]->insertStreetSegmentIDs(i);

        //data needed for m2
        streetSegments_new[i] = temp;

        //total length of street from street segments
        totalDistance = 0.0;

        //LatLon of start and end intersection of street segment
        IntersectionIndex startPoint = temp.from;
        IntersectionIndex endPoint = temp.to;
        speed_limit = temp.speedLimit * 0.27778;
        numCurvePoints = temp.curvePointCount;

        // if straight line
        if (numCurvePoints == 0) {
            double a = find_distance_between_two_points(getIntersectionPosition(startPoint), getIntersectionPosition(endPoint));
            StreetSegmentLength[i] = a;
            StreetSegmentTime[i] = (a / speed_limit);

        } else {
            LatLon point1 = getIntersectionPosition(startPoint);
            LatLon point2;
            
            for (unsigned int k = 0; k < numCurvePoints; k++) {
                //find distance up to last curve point
                point2 = getStreetSegmentCurvePoint(i, k);
                totalDistance += find_distance_between_two_points(point1, point2);
                point1 = point2;
            }
            // distance from last curve point to intersection
            totalDistance += find_distance_between_two_points(point1, getIntersectionPosition(endPoint));
            streetSegmentLength[i] = totalDistance;
            streetSegmentTime[i] = (totalDistance / speed_limit);
        }
    }
}

void load_intersections() {
    unsigned i, j;
    
    set<unsigned> intersections;
    for (i = 0; i < numberOfStreets; i++) {
        // get street segment
        vector<StreetSegmentInfo> temp = streets[i]->streetSegments;
        for (j = 0; j < temp.size(); j++) {
            // insert segment into set
            intersections.insert(temp[j].to);
            intersections.insert(temp[j].from);
        }
        // copy set to vector
        vector<unsigned>intersectionIDs(intersections.begin(), intersections.end());
        // insert intersection into streets
        streets[i]->insertIntersectionID(intersectionIDs);
        // clear vector and set for the next street
        intersections.clear();
        intersectionIDs.clear();
    }
}

void load_poi() /*NOT DONE*/{
    numberOfPointsOfInterest = getNumberOfPointsOfInterest();
    for (unsigned p = 0; p < numberOfPointsOfInterest; p++) {
        //implement
    }
}

double find_distance_between_two_points(LatLon point1, LatLon point2) {
    double latavg = (point1.lat() + point2.lat()) / 2 * DEG_TO_RAD;
    double x1 = point1.lon() * cos(latavg);
    double y1 = point1.lat();
    double x2 = point2.lon() * cos(latavg);
    double y2 = point2.lat();
    return EARTH_RADIUS_IN_METERS * sqrt(pow(y2 - y1, 2.0) + pow(x2 - x1, 2.0));
}

// are the two intersections (id1 and id2) directly connected
bool are_directly_connected(unsigned id1, unsigned id2) {
    // check if valid intersections
    if (id1 > numberOfIntersections || id2 > numberOfIntersections) {
        //cout <<"invalid input"<< endl;
        return false;
    }
    
    unsigned numSegments = getIntersectionSegmentCount(id1);

    if (id1 == id2) {
        return true;
    } else {
        for (unsigned i = 0; i < numSegments; i++) {
            StreetSegmentIndex j = getIntersectionStreetSegment(id1, i);
            StreetSegmentInfo StreetInfo = getStreetSegmentInfo(j);
            if (StreetInfo.from == id1) {
                if (StreetInfo.to == id2) {
                    return true;
                }
            } else if (!StreetInfo.oneWay) {
                if (StreetInfo.from == id2) {
                    return true;
                }
            }
        }
        return false;
    }
}

std::vector<unsigned> find_street_ids_from_name(std::string street_name) {
    // return vector in unordered map
    vector <unsigned> street_ids;
    vector <unsigned> temp = name_streetID[street_name];
    set <unsigned> tempSet(temp.begin(), temp.end());
    street_ids.assign(tempSet.begin(), tempSet.end());
    return street_ids;
}

std::vector<unsigned> find_intersection_street_segments(unsigned intersection_id) {
    return streetSegments[intersection_id];
}

std::vector<std::string> find_intersection_street_names(unsigned intersection_id) {
    return streetSegmentNames[intersection_id];
}

std::vector<unsigned> find_adjacent_intersections(unsigned intersection_id) {
    unsigned numSegments = getIntersectionStreetSegmentCount(intersection_id);
    set<unsigned> adjacent;
    for (unsigned i = 0; i < numSegments; i++) {
        StreetSegmentIndex j = getIntersectionStreetSegment(intersection_id, i);
        StreetSegmentInfo StreetInfo = getStreetSegmentInfo(j);

        if (StreetInfo.from == intersection_id)
            adjacent.insert(StreetInfo.to);
        else if (!StreetInfo.oneWay)
            adjacent.insert(StreetInfo.from);
    }
    return vector<unsigned>(adjacent.begin(), adjacent.end());
}

std::vector<unsigned> find_street_street_segments(unsigned street_id) {
    return streets[street_id]->streetSegmentsIndex;
}

std::vector<unsigned> find_all_street_intersections(unsigned street_id) {
    return streets[street_id]->intersectionID;
}

std::vector<unsigned> find_intersection_ids_from_street_names(std::string street1, std::string street2) {
    vector<unsigned> id1 = find_street_ids_from_name(street1);
    vector<unsigned> id2 = find_street_ids_from_name(street2);
    vector<unsigned> intersection1;
    vector<unsigned> intersection2;
    vector<unsigned> intersection_ids;
    
    if (id1.empty()) {
        return id1;
    } else if (id2.empty()) {
        return id2;
    }
    
    for (vector<unsigned>::iterator it1 = id1.begin(); it1 != id1.end(); it1++) {
        intersectionIDs.insert(intersection1.end(), streets[*it1]->intersectionID.begin(), streets[*it1]->intersectionID.end());
    }
    
    for (vector<unsigned>::iterator it2 = id2.begin(); it2 != id2.end(); it2++) {
        intersection_ids.insert(intersection2.end(), streets[*it2]->intersectionID.begin(), streets[*it2]->intersectionID.end());
    }
    
    set_intersection(intersection1.begin(), intersection1.end(),
            intersection2.begin(), intersection2.end(), back_inserter(intersection_ids));

    return intersection_ids;
}

double find_street_segment_length(unsigned segment_id) {
    return streetSegmentLength[segment_id];
}

double find_street_length(unsigned street_id) {
    double length = 0.0
    vector<unsigned> segments = streets[street_id]->streetSegmentsIndex;
    for (unsigned i = 0; i < segments.size(); i++) {
        length += find_street_segment_length(segments[i]);
    }
    return length;
}

double find_street_segment_travel_time(unsigned segment_id) {
    return streetSegmentTime[segment_id];
}

