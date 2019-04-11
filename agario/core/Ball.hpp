#pragma once

#include <math.h>

#include "core/types.hpp"
#include "settings.hpp"

namespace Agario {

  class Ball {
  public:
    Ball() = delete;
    explicit Ball(const Location &loc) : x(loc.x), y(loc.y) { }
    Ball(distance x, distance y) : Ball(Location(x, y)) { }

    virtual distance radius() const = 0;
    virtual Agario::mass mass() const = 0;

    distance height() const { return 2 * radius(); }
    distance width() const { return 2 * radius(); }

    bool collides_with(const Ball &other) {
      auto sqr_rads = pow(radius() + other.radius(), 2);
      return sqr_rads <= sqr_distance_to(other);
    }

    Location location() const { return Location(x, y); }

    bool operator <(const Ball& other) const {
      return mass() * CELL_EAT_MARGIN < other.mass();
    }

    bool operator >(const Ball& other) const {
      return mass() > other.mass() * CELL_EAT_MARGIN;
    }

    bool operator ==(const Ball &other) const {
      return mass() == other.mass();
    }

    distance x;
    distance y;
    virtual ~Ball() = default;
  private:

    distance sqr_distance_to(const Ball &other) {
      return (location() - other.location()).norm_sqr();
    }
  };

  // Virtual inheritance because of
  class MovingBall : virtual public Ball {
  public:
    using Ball::Ball;
    MovingBall() = delete;
    MovingBall(Location &loc, Velocity &v) : Ball(loc), velocity(v) { }
    Agario::Velocity velocity;
  };

}
