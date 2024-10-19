#ifndef COLLISIONS_H_
#define COLLISIONS_H_

#include "particle.h"
#include <SFML/System/Vector2.hpp>
#include <cmath>

inline float distance(const sf::Vector2f &a, const sf::Vector2f &b) {
  const float dx = a.x - b.x;
  const float dy = a.y - b.y;
  return std::sqrt(dx * dx + dy * dy);
}

inline bool collide(const Particle& a, const Particle& b) {
  const float dist = distance(a.center, b.center);
  return dist < a.radius + b.radius;
}

inline bool solve_collision(Particle &a, Particle &b) {
  const float dist = distance(a.center, b.center);
  const float move_amount = (dist - a.radius - b.radius) / 2;

}

#endif // COLLISIONS_H_
