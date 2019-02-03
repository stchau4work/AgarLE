#include <iostream>
#include <algorithm>

#include "Engine.hpp"
#include "Entities.hpp"
#include "utils.hpp"

namespace Agario {

  void Engine::tick() {
    std::cout << "tick: " << ticks << std::endl;

    std::vector<Food> created_masses;
    for (Player &player : players)
      tick_player(player);

    ticks++;
  }

  void Engine::tick_player(Player &player) {
    move_player(player);

    std::vector<Cell> created_cells;
    for (Cell &cell : player.cells) {
      eat_pellets(cell);
      eat_food(cell);

      // todo: change VIRUS_SIZE to static member of Virus?
      check_virus_collisions(cell, created_cells);

      if (player.action == Agario::action::feed) {

        // todo: create mass
      }

      if (player.action == Agario::action::split) {
        // todo: split
      }
    }

    // add any cells that were created
    player.cells.insert(std::end(player.cells),
                        std::begin(created_cells),
                        std::end(created_cells));

    // player collisions
    for (Player &other : players) {
      // todo: player collisions
    }
  }

  void Engine::move_player(Player &player) {
    // todo
  }

  void Engine::add_pellets(int num_pellets) {
    while (num_pellets > 0) {
      pellets.emplace_back(random_location());
      num_pellets--;
    }
  }

  void Engine::add_virus(int num_virus) {
    while (num_virus > 0) {
      viruses.emplace_back(random_location());
      num_virus--;
    }
  }

  void Engine::eat_pellets(Cell &cell) {
    auto prev_size = total_pellets();
    std::remove_if(pellets.begin(), pellets.end(),
                   [&](const Pellet &pellet) {
                     return cell.collides_with(pellet);
                   });

    auto num_eaten = total_pellets() - prev_size;
    cell.increment_mass(num_eaten * FOOD_MASS);
  }

  void Engine::eat_food(Cell &cell) {
    if (cell.mass() < MASS_MASS) return;
    auto prev_size = total_foods();
    std::remove_if(foods.begin(), foods.end(),
                   [&](const Food &mass) {
                     return cell.collides_with(mass);
                   });
    auto num_eaten = total_foods() - prev_size;
    cell.increment_mass(num_eaten * MASS_MASS);
  }

  void Engine::check_virus_collisions(Cell &cell, std::vector<Cell> &created_cells) {
    for (unsigned long i = 0; i < viruses.size(); i++) {
      Virus &virus = viruses[i];
      if (cell.mass() < virus.mass()) continue;

      if (cell.collides_with(virus)) {

        disrupt(cell, created_cells);

        std::swap(viruses[i], viruses.back()); // O(1) removal
        viruses.pop_back();
        return; // only collide with one
      }
    }
  }

  void Engine::disrupt(Cell &cell, std::vector<Cell> &created_cells) {
    Agario::mass total_mass = cell.mass(); // mass to conserve

    // reduce the cell by roughly this ratio CELL_POP_REDUCTION, making sure the
    // amount removes is divisible by CELL_POP_SIZE
    cell.reduce_mass_by_factor(CELL_POP_REDUCTION);
    cell.increment_mass((total_mass - cell.mass()) % CELL_POP_SIZE);

    Agario::mass pop_mass = total_mass - cell.mass(); // mass conservation
    int num_new_cells = div_round_up<Agario::mass>(pop_mass, CELL_POP_SIZE);

    Agario::mass remaining_mass = pop_mass;

    Agario::angle theta = cell.velocity.direction();
    for (int c = 0; c < num_new_cells; c++) {
      Agario::angle dvel_angle = cell.velocity.direction() + (2 * M_PI * c / num_new_cells);
      Agario::Velocity vel = cell.velocity + Velocity(dvel_angle, CELL_POP_SPEED);

      Agario::mass new_cell_mass = std::min<Agario::mass>(remaining_mass, CELL_POP_SIZE);

      created_cells.emplace_back(cell.location(), vel, new_cell_mass, cell.player());
      remaining_mass -= new_cell_mass;
    }
  }

  Agario::Location Engine::random_location() {
    return Location(rand() % canvas_width, rand() % canvas_height);
  }


}

