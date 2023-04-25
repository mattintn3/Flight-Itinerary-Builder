#include "flightMap.h"
#include <sstream>
#include <iomanip>
#include <string>
#include <stack>



FlightMapClass::FlightMapClass() { // default constructor
  size = 0;
  map = nullptr;
}

FlightMapClass::FlightMapClass(const FlightMapClass &f) { // copy constructor
  size = f.size;
  map = f.map;
  visited = f.visited;
}

FlightMapClass::~FlightMapClass() { // destructor
  if (map != nullptr) { // check if map contains data, if yes clear and delete
    for (int i = 0; i < size; i++)
      {
        map[i].clear();
      }
      delete[] map;
  }
}

void FlightMapClass::ReadCities(ifstream &myCities) { // read and store cities into vector
  myCities >> size; // read first line of cities.dat since first line is number of cities
  string line;
  visited.resize(size);
  for (int i = 0; i < size; i++)
  {
    visited[i] = false;
  }
  while (myCities >> line) { // loop to store rest of data into vector
    cities.push_back(line);
  }
}

void FlightMapClass::BuildMap(ifstream &myFlights) { // builds flight map
  map = new list<flightRec>[size];
  int flightNum, price;
  string origin, destination;
  // Read data from file
  while (myFlights >> flightNum >> origin >> destination >> price) // read in data in the order it is stored in flights.dat
    {
      int originIndex;
      for (int i = 0; i < size; i++) { // find out index for origin within datafile and store into originIndex
        if (cities[i] == origin){
          originIndex = i;
          break;
        }
      }
      flightRec f; // creating new instance of flighrec

      // populate correct data into f
      f.flightNum = flightNum;
      f.price = price;
      f.origin = origin;
      f.destination = destination;

      map[originIndex].push_back(f); // add f to back of list
    }
}

void FlightMapClass::DisplayMap() { // displays flight map in neatly formatted table
  cout << setw(10) << "Origin" << setw(20) << "Destination" << setw(15) << "Flight" << setw(14) << "Price" << endl;
  cout << "===================================================================" << endl;
  for (int i = 0; i < size; i++){ // outer portion of the nested for loop will print the From origin to: statement using citiesVect
    if (map[i].size() > 0) {
      cout << "From " << cities[i] << " to:\n";
      for (auto iterator = map[i].begin(); iterator != map[i].end(); iterator++){ // create iterator for list, since it cannot be indexed like an array, an iterator must be created to store positioning. auto keyword automatically sets datatype
        cout << setw(30) << iterator->destination << setw(15) << iterator->flightNum << setw(10) << "$" << iterator->price << endl; // output itinerary data
      }
      cout << "-------------------------------------------------------------------" << "\n";
    }
  }
}

bool FlightMapClass::CheckCity(string cityName) const { // check if cityName is a city served
  for (int i = 0; i < size; i++) {
    if (cities[i] == cityName)
      return true;
  }
  return false;
}

void FlightMapClass::DisplayAllCities() const { // display all cities
  for (int i = 0; i < size; i++) {
    cout << cities[i] << endl;
  }
}

void FlightMapClass::MarkVisited(int city) { // mark city as visited for findPath method
  visited[city] = true;
}

void FlightMapClass::UnvisitAll() { // mark all cities as false for visited
  for (int i = 0; i < size; i++) {
    visited[i] = false;
  }
}

bool FlightMapClass::IsVisited(int city) const { // check if city is visited
  if (visited[city]) {
    return true;
  }
  else
    return false;
}

bool FlightMapClass::GetNextCity(string fromCity, string &nextCity) { // determine next unvisited city
  int cityIndex = GetCityNumber(fromCity);
  auto iterator = map[cityIndex].begin();
  
  while (iterator != map[cityIndex].end()){
    if(!IsVisited(GetCityNumber(iterator->destination))){
      nextCity = iterator->destination;
      return true;
    }
    iterator++;
  }
  return false;
}

int FlightMapClass::GetCityNumber(string cityName) const { // get the city number 
  for (int i = 0; i < size; i++){
    if (cities[i] == cityName) {
    return i;
    }
  }
  return -1; // -1 is invalid index, used when city does not exist
}

string FlightMapClass::GetCityName(int cityNumber) const { // get city name
  return cities[cityNumber];
}

void FlightMapClass::FindPath(string originCity, string destinationCity) { // using similar isPath algorithm from class to find flight paths
  stack<string> aStack; // aStack will be used to keep track of cities visited
  stack<flightRec> flightStack, tempStack; // track flights taken(flightStack) and tempStack to reverse order
  string topCity, nextCity;
  bool success;

  UnvisitAll(); // clear marks on all cities
  
  aStack.push(originCity); // push origin city to stack first
  MarkVisited(GetCityNumber(originCity));
  topCity = aStack.top();

  
  while (!aStack.empty() && (topCity != destinationCity)) { // while stack is not empty and destination has not been reached, continue search
    success = GetNextCity(topCity, nextCity);
    if(!success){
      aStack.pop(); // if no city has been found, backtrack and pop last city and flight
      if(!flightStack.empty()){
        flightStack.pop();
      }
    }
    else { // visit each city, push to stack
      aStack.push(nextCity);
      MarkVisited(GetCityNumber(nextCity));
      
      for (auto iterator = map[GetCityNumber(topCity)].begin(); iterator != map[GetCityNumber(topCity)].end(); iterator++) // push flights into flightStack and iterate through flight records with same origin as topCity
        {
          auto& flight = *iterator; // allow direct access to data
          if (flight.destination == nextCity) { // check if destination matches nextCity
            flightStack.push(flight);
            break;
          }
        }
    }
    if (!aStack.empty()) { // update top city if stack is not empty
      topCity = aStack.top();
    }
  }
  if (aStack.empty()) { // if stack is empty then no path exists
    cout << "Sorry, BlueSky airline does not fly from " << originCity << " to " << destinationCity << endl;
  }
  else { // if stack is not empty, print itinerary
    int totalCost = 0;
    cout << "Request is to fly from " << originCity << " to " << destinationCity << endl;
    cout << "BlueSky airline serves between these two cities." << endl;
    cout << "The flight itinerary is: " << endl;
    cout << "Flight #"  << setw(10) << "From" << setw(10) << "To" << setw(10) << "Cost" << endl;

    while (!flightStack.empty()) { // order of flightStack is reversed to display itinerary in correct order
      tempStack.push(flightStack.top());
      flightStack.pop();
    }
    while (!tempStack.empty()) { // print itinerary and calculate cost
      flightRec flight = tempStack.top();
      tempStack.pop();

      cout << flight.flightNum << setw(15) << right << flight.origin << setw(15) << right << flight.destination << setw(5) << right << "$" << flight.price << endl;
      totalCost += flight.price;
    }
    cout << "Total: $" << totalCost << endl;
  }
}