#pragma once

#include "core/Player.hpp"

#define SHY_RADIUS 25

namespace agario {
  namespace bot {

    template<bool renderable>
    class HungryShyBot : public agario::Player<renderable> {
      static constexpr agario::color default_color = agario::color::purple;
    public:
      typedef agario::Player<renderable> Player;

      HungryShyBot(agario::pid pid, const std::string &name, agario::color color) : Player(pid, name, color) {}
      HungryShyBot(agario::pid pid, const std::string &name) : HungryShyBot(pid, name, default_color) {}
      explicit HungryShyBot(const std::string &name) : HungryShyBot(-1, name) {}
      explicit HungryShyBot(agario::pid pid) : HungryShyBot(pid, "HungryShyBot") {}

      void take_action(const GameState<renderable> &state) override {
        this->action = agario::action::none; // no splitting or anything

        // check if there are any big players nearby
        for (auto &pair : state.players) {
          Player &other_player = *pair.second;
          if (other_player == *this) continue; // skip self

          // is it nearby?
          auto distance = this->location().distance_to(other_player.location());

          // it is scary?
          if (distance < SHY_RADIUS && other_player.mass() > mass()) {
            // yes! run (directly) away!
            this->target = this->location() - (other_player.location() - this->location());
            return;
          }
        }

        // no cells are too close for comfort... forage for foods
        distance min_distance = agario::distance::max();
        for (auto &pellet : state.pellets) {
          distance dist = pellet.location().distance_to(this->location());
          if (dist < min_distance) {
            this->target = pellet.location(); // i can haz cheeseburger?
            min_distance = dist;
          }
        }
      }

    };

  }
}