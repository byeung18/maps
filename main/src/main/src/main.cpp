#include "StreetsDatabaseAPI.h"
#include "m1.h"
//#include "m2.h"
#include <iostream>
#include <vector>
#include <algorithm>
//#include "m3.h"
//#include "m4.h
#include "LatLon.h"

using namespace std;


int main(int argc, char* argv[]) {
//	string city = argv[1];	
//	string show_me_map;
//	cout << "Which map do you want: \n" << "	a) Toronto" << endl;
//	cin >> show_me_map;
//	string bin = "/home/brian/Desktop/maps/assignment0/main/src/" + city +".bin";
//	cout << "Opening maps for " << show_me_map << endl;
//	load_map(bin);
//    close_map();


	LatLon p1(100, 100);
	LatLon p2(150, 150);
	LatLon p3(200, 200);

	LatLon* p = &p1;
	p1 = p2;
	cout << p << endl;
	cout << (*p).lat << endl;
	cout << &p1.lat << endl;
	cout << &p2.lat << endl;
	
	p1.lat = p3.lat;
	cout << &p1.lat << endl;
	cout << &p3.lat << endl;

	



    return 0;
}
