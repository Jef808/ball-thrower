#include <SFML/Graphics.hpp>

#include "particlegenerator.h"
#include "solver.h"

#include <algorithm>
#include <vector>

int main() {
  sf::Vector2u window_size = {1920u, 1080u};
  sf::Vector2f window_size_f = {1920.0f, 1080.0f};

  float max_frame_rate = 144.0f;

  sf::CircleShape Constraint{400.0f};
  Constraint.setOrigin({400.0f, 400.0f});
  Constraint.setPosition(window_size_f * 0.5f);
  Constraint.setFillColor(sf::Color::Black);

  sf::Color WindowColor = {120u, 120u, 120u};

  std::vector<Particle> particles;

  uint32_t particles_per_second = 10;
  uint32_t frequency = static_cast<uint32_t>(max_frame_rate / particles_per_second);

  sf::Vector2f position = window_size_f * 0.5f - sf::Vector2f{0.0f, 300.0f};
  ParticleGenerator generator(position, 5.0f, 20.0f, frequency, particles);

  Solver solver{particles};

  std::vector<sf::CircleShape> circles;

  auto window = sf::RenderWindow({window_size.x, window_size.y}, "CMake SFML Project");
  window.setFramerateLimit(max_frame_rate);

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

    solver.update(1.0f / max_frame_rate);

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
