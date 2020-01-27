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
vector<string>* streetSegmentName;
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
        clean_data_structures();
        
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
void clean_data_structures() {
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

    if (streetSegmentsName != nullptr) {
        delete [] streetSegmentName;
        pointer_streetSegmentsName = nullptr;
    }
}

void close_map() {
    //Clean-up your map related data structures here
    clean_data_structures();

    if (searchDatabase.root != nullptr) {
        delete searchDatabase.root;
    }
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
