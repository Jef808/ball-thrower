#ifndef PARTICLE_H_
#define PARTICLE_H_

#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Color.hpp>
#include <cmath>

inline float length(const sf::Vector2f &a) {
  return std::sqrt(a.x * a.x + a.y * a.y);
}

inline float distance(const sf::Vector2f &a, const sf::Vector2f &b) {
  const float dx = a.x - b.x;
  const float dy = a.y - b.y;
  return std::sqrt(dx * dx + dy * dy);
}

struct VerletObject {
  sf::Vector2f position_current;
  sf::Vector2f position_old;
  sf::Vector2f acceleration{0.0f, 0.0f};

  void updatePosition(float dt) {
    const sf::Vector2f velocity = position_current - position_old;

    // Save current position
    position_old = position_current;

    // Perform Verlet integration
    position_current += velocity + acceleration * dt * dt;

    // Reset acceleration
    acceleration.x = acceleration.y = 0;
  }

  void accelerate(sf::Vector2f acc) {
    acceleration += acc;
  }
};

struct Particle: VerletObject {
  float radius;
  sf::Color color;
};

#endif // PARTICLE_H_
