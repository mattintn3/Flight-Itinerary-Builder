#include <iostream>
#include <fstream>
#include "flightMap.h"
#include "type.h"

using namespace std;

int main() {
  // data files
  ifstream cities("cities.dat");
  ifstream flights("flights.dat");
  ifstream requests("requests.dat");

  // Check if files are found in project directory
  if (!cities || !flights || !requests) {
    cout << "Error! Could not open one of files" << endl;
    return 1;
  }

  FlightMapClass flightMap; // create instance of FlightMapClass
  
  // read cities and build map
  flightMap.ReadCities(cities);
  flightMap.BuildMap(flights);

  string originCity, destinationCity; // used for storage of cities read from input file

  while (requests >> originCity >> destinationCity) // read in from requests.dat
  {
    flightMap.FindPath(originCity, destinationCity); // find path and print itinerary
    cout << endl; // separate each new request
  }
  
  // Close input files 
  cities.close();
  flights.close();
  requests.close();

  return 0;
}