#include "type.h"
//Implement the struct type defined in type.h here

// Define < overloaded method
bool flightRec::operator<(const flightRec & rhs) const {
  return (this->price < rhs.price);
}

// Define == overloaded method
bool flightRec::operator==(const flightRec & rhs) const {
  return (this->origin == rhs.origin &&
          this->flightNum == rhs.flightNum &&
          this->destination == rhs.destination &&
          this->price == rhs.price);
}

// Define << overloaded method
ostream& operator<<(ostream & os, const flightRec & f) {
  os << f.origin << " " << f.flightNum << " " << " " << f.destination << " " << f.price;
  return os;
}

