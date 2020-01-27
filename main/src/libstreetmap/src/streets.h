#ifndef STREETS_H
#define STREETS_H

#include <vector>
#include <algorithm>
#include <cmath>
#include <map>
#include <string>
#include "StreetsDatabaseAPI.h"
#include "m1.h"

using namespace std;

class Streets {
private:
    unsigned id;
    string name;
public:
    vector<unsigned> intersectionID;
    vector<StreetSegmentInfo> streetSegments;
    vector<unsigned> streetSegmentIDs;

    topLevelStreets(string _name, unsigned _id);

    void insertIntersectionID(vector<unsigned> _intersectionID);

    void insertStreetSegmentInfo(StreetSegmentInfo info);

    void insertStreetSegmentIDs(unsigned streetSegmentsIDs);

    string getStreetName();

    unsigned getStreetId();

    ~Streets();
};

#endif


