#ifndef SOLVER_H_
#define SOLVER_H_

#include <SFML/System/Vector2.hpp>
#include "particle.h"
#include <algorithm>
#include <cstdint>
#include <vector>

inline float length(const sf::Vector2f &a) {
  return std::sqrt(a.x * a.x + a.y * a.y);
}

inline float distance(const sf::Vector2f &a, const sf::Vector2f &b) {
  const float dx = a.x - b.x;
  const float dy = a.y - b.y;
  return std::sqrt(dx * dx + dy * dy);
}

struct Solver {
  const sf::Vector2f gravity = {0.0f, 1000.0f};
  std::vector<Particle> &particles;

  Solver(std::vector<Particle> &particles)
      : particles{particles} {}

  void update(float dt) {
    applyGravity();
    applyConstraint();
    solveCollisions();
    updatePositions(dt);
  }

  void updatePositions(float dt) {
    std::for_each(particles.begin(), particles.end(), [dt](auto& obj) {
      obj.updatePosition(dt);
    });
  }

  void applyGravity() {
    std::for_each(particles.begin(), particles.end(), [&](auto& obj) {
      obj.accelerate(gravity);
    });
  }

  void applyConstraint() {
    const sf::Vector2f position{960.0f, 540.0f};
    const float radius = 400.0f;

    std::for_each(particles.begin(), particles.end(), [&](auto &obj) {
      const sf::Vector2f to_obj = obj.position_current - position;
      const float dist = length(to_obj);

      if (dist + obj.radius > radius) {
        const sf::Vector2f n = to_obj / dist;
        obj.position_current = position + n * (radius - obj.radius);
      }
    });
  }

  void solveCollisions() {
    for (auto &obj_1 : particles) {
      for (auto &obj_2 : particles) {
        if (&obj_1 != &obj_2) {
          const auto collision_axis =
              obj_1.position_current - obj_2.position_current;
          const float dist = length(collision_axis);
          if (dist < obj_1.radius + obj_2.radius) {
            const float delta = obj_1.radius + obj_2.radius - dist;
            float v1 = obj_2.radius / (obj_1.radius + obj_2.radius);
            float v2 = obj_1.radius / (obj_1.radius + obj_2.radius);
            const auto n = collision_axis / dist;
            obj_1.position_current += v1 * delta * n;
            obj_2.position_current -= v2 * delta * n;
          }
        }
      }
    }
  }
};

#endif // SOLVER_H_
