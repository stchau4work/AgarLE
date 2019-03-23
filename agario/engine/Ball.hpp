#ifndef AGARIO_BALL_HPP
#define AGARIO_BALL_HPP

#include "Player.hpp"
#include "types.hpp"
#include <math.h>
#include "settings.hpp"

namespace Agario {

  class Ball {
  public:
    explicit Ball(const Location &loc) : x(loc.x), y(loc.y) { }
    explicit Ball(Location &&loc) : x(loc.x), y(loc.y) { }
    explicit Ball(distance x, distance y) : Ball(Location(x, y)) { }

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

    distance x; // todo: turn these into a single Location
    distance y;
    virtual ~Ball() = default;
  private:

    distance sqr_distance_to(const Ball &other) {
      return (location() - other.location()).norm_sqr();
    }
  };

  class MovingBall : public Ball {
  public:
    explicit MovingBall(distance x, distance y) : Ball(x, y) { }
    explicit MovingBall(Location &&loc) : Ball(loc) { }
    MovingBall(Location &loc, Velocity &v) : Ball(loc), velocity(v) { }

    Agario::Velocity velocity;
  };


}

#endif //AGARIO_BALL_HPP
