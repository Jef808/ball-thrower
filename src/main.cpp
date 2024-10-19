#include <SFML/Graphics.hpp>

#include "particle.h"
#include "solver.h"

#include <algorithm>
#include <random>
#include <vector>

enum class Rotation {
  Left,
  Right
};

struct ParticleGenerator {
  sf::Vector2f position;
  float radius_min;
  float radius_max;
  uint32_t frequency;

  ParticleGenerator(sf::Vector2f position, float radius_min, float radius_max,
                    uint32_t frequency, std::vector<Particle> &particles)
      : position{position}, radius_min{radius_min}, radius_max{radius_max},
        frequency{frequency}, particles{particles} {
    colors.push_back(sf::Color::Red);
    colors.push_back(sf::Color::Blue);
    colors.push_back(sf::Color::Yellow);
    colors.push_back(sf::Color::Green);
    colors.push_back(sf::Color::Magenta);
    colors.push_back(sf::Color::Cyan);
  }

  std::vector<Particle> &particles;

  std::vector<sf::Color> colors;
  sf::Vector2f direction{0.0f, 1.0f};

  void generate() {
    static uint32_t counter = 0;
    static std::random_device rd{};
    static std::mt19937 eng{rd()};

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

int main() {
  sf::Vector2f window_size = {1920.0f, 1080.0f};

  sf::CircleShape Constraint{400.0f};
  Constraint.setOrigin({400.0f, 400.0f});
  Constraint.setPosition(window_size / 2.0f);
  Constraint.setFillColor(sf::Color::Black);

  sf::Color WindowColor = {120u, 120u, 120u};

  std::vector<Particle> particles;

  float particles_per_second = 10.0f;
  uint32_t frequency = static_cast<uint32_t>(144.0f / particles_per_second);

  sf::Vector2f position = window_size / 2.0f - sf::Vector2f{0.0f, 300.0f};
  ParticleGenerator generator(position, 2.0f, 15.0f, frequency, particles);

  Solver solver{particles};

  std::vector<sf::CircleShape> circles;

  auto window = sf::RenderWindow({1920u, 1080u}, "CMake SFML Project");
  window.setFramerateLimit(144);

  while (window.isOpen())
  {
      for (auto event = sf::Event(); window.pollEvent(event);)
      {
          if (event.type == sf::Event::Closed)
          {
              window.close();
          } else if (event.type == sf::Event::KeyPressed)
          {
            if (event.key.code == sf::Keyboard::Q ||
                event.key.code == sf::Keyboard::Escape)
            {
                window.close();
            }
          } else if (event.type == sf::Event::MouseMoved) {
            float x = static_cast<float>(event.mouseMove.x);
            float y = static_cast<float>(event.mouseMove.y);
            generator.aim({x, y});
          }
      }

      generator.generate();

      solver.update(1.0f / 144.0f);

      circles.clear();
      std::transform(particles.begin(), particles.end(),
                     std::back_inserter(circles), [](const auto &particle) {
                       sf::CircleShape circle{particle.radius};
                       circle.setOrigin({particle.radius, particle.radius});
                       circle.setPosition(particle.position_current);
                       circle.setFillColor(particle.color);
                       return circle;
                     });

      window.clear(WindowColor);
      window.draw(Constraint);

      for (const auto &circle : circles) {
        window.draw(circle);
      }

      window.display();
  }
}
