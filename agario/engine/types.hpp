#ifndef AGARIO_TYPES_HPP
#define AGARIO_TYPES_HPP

#include <cmath>
//#include "THST/QuadTree.h"

namespace Agario {

  typedef float position;
  typedef float length;
  typedef double angle;

  typedef unsigned int mass;
  typedef unsigned int score;

  typedef unsigned short pid;
  typedef unsigned long tick;

  enum Color { red = 0, orange = 1, yellow = 2, green = 3, blue = 4, purple = 5 };

  class Location {
  public:
    explicit Location(position x, position y) : x(x), y(y) { }
    position x;
    position y;
  };

  class Velocity {
  public:
    explicit Velocity() : dx(0), dy(0) { }
    explicit Velocity(length dx, length dy) : dx(dx), dy(dy) { }
    explicit Velocity(Agario::angle angle, Agario::length speed) :
    dx(speed * std::cos(angle)), dy(speed * std::sin(angle)) { }
    Velocity(Velocity&& v) : dx(v.dx), dy(v.dy) { }
    Velocity(const Velocity& v) : dx(v.dx), dy(v.dy) { }

    Agario::angle direction() const {
      auto angle = std::atan(dx / dy);
      if (dx < 0) {
        if (dy > 0) angle += M_PI;
        else angle -= M_PI;
      }
      return static_cast<Agario::angle>(angle);
    }

    Velocity& operator +=(const Velocity &rhs) {
      dx += rhs.dx;
      dy += rhs.dy;
      return *this;
    }

    Velocity& operator -=(const Velocity &rhs) {
      dx -= rhs.dx;
      dy -= rhs.dy;
      return *this;
    }

    length dx;
    length dy;
  };

  inline Velocity operator+(Velocity lhs, const Velocity& rhs) {
    lhs += rhs;
    return lhs;
  }

  inline Velocity operator-(Velocity lhs, const Velocity& rhs) {
    lhs -= rhs;
    return lhs;
  }

//  template <typename T> struct Box2 {
//    T min[2];
//    T max[2];
//
//    explicit operator spatial::BoundingBox<int, 2>() const {
//      return spatial::BoundingBox<int, 2>(min, max);
//    }
//  };

}


#endif //AGARIO_TYPES_HPP
