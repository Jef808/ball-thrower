#ifndef PARTICLEGENERATOR_H_
#define PARTICLEGENERATOR_H_

#include <SFML/Graphics.hpp>

#include "particle.h"

#include <array>
#include <random>

struct ParticleGenerator {
  sf::Vector2f position;
  float radius_min;
  float radius_max;
  uint32_t frequency;
  std::vector<Particle> &particles;

  ParticleGenerator(sf::Vector2f position, float radius_min, float radius_max,
                    uint32_t frequency, std::vector<Particle> &particles)
      : position{position}, radius_min{radius_min}, radius_max{radius_max},
        frequency{frequency}, particles{particles} {}

  std::array<sf::Color, 6> colors = {
    sf::Color::Red,
    sf::Color::Blue,
    sf::Color::Yellow,
    sf::Color::Green,
    sf::Color::Magenta,
    sf::Color::Cyan
  };
  sf::Vector2f direction{-1.0f, 0.0f};

  uint32_t counter = 0;
  std::random_device rd{};
  std::mt19937 eng{rd()};

  void generate() {
    if (counter % frequency == 0) {
      sf::Vector2f velocity = 6.0f * direction;

      Particle particle;
      std::uniform_real_distribution<float> dist(radius_min, radius_max);
      particle.position_current = position;
      particle.position_old = position - velocity;
      particle.radius = dist(eng);

      std::uniform_int_distribution idist(0, 5);
      sf::Color color = colors[idist(eng)];
      particle.color = color;
      particles.push_back(particle);
    }
    ++counter;
  }

  void aim(sf::Vector2f target) {
    auto to_target = target - position;
    direction = to_target / length(to_target);
  }
};

#endif // PARTICLEGENERATOR_H_
