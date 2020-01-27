#define SIDEROADLENGTH 2000 // length is in metres
#define MAINROADLENGTH 5000

#include <cmath>
#include <string>
#include "DataStructure.h"
#include "StreetsDatabaseAPI.h"
#include "OSMDatabaseAPI.h"
#include "m1.h"
//#include "m2.h"
//#include "m3.h"
//#include "m4.h"

using namespace std;

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

// load the map
bool load_map(string map_path) {
    // if successful load
    if (loadStreetsDatabaseBIN(map_path)) {
        auto iter = map_path.find("streets.bin");
        map_path.erase(iter);
        thread loadOSM(loadOSMDatabaseBIN, map_path);
        thread loadSearch(load_search_database);
        
        // load information
        load_streets();
        load_intersections();
        load_poi();
        load_features();
        load_osmid();
        
        return true;
    } else {
        return false;
    }
        
}

//close the map

void close_map() {
    closeStreetDatabase();
    cout << "INFO: Window closed" << endl;
    // destroy any data structures you created in load_map
    // ...
}

