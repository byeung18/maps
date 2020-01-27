#include "Streets.h"

using namespace std;

Streets::streets(string _name, unsigned _id) {
    name = _name;
    id = _id;
}

void Streets:: insertIntersectionID(vector<unsigned> _intersectionID) {
    intersectionID = _intersectionID;
}

void Streets:: insertStreetSegmentInfo(StreetSegmentInfo info) {
    streetSegments.push_back(info);
}

void Streets:: insertStreetSegmentIDs(unsigned streetSegmentIDs) {
    streetSegmentIDs.push_back(streetSegmentIDs);
}

string Streets:: getStreetName() {
    return (name);
}

unsigned Streets:: getStreetId() {
    return (id);
}

Streets::~streets() {
    streetSegments.clear();
    streetSegmentIDs.clear();
    intersectionID.clear();
}

    



