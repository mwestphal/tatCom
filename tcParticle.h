#include <SFML/Graphics.hpp>

class tcParticle
{
public:
  tcParticle(sf::Vector2f position, sf::Vector2f out):
    Position(position),
    Out(out),
    Move(out),
    Width(20),
    Step(0)
    {
    }

  tcParticle(sf::Vector2f position, sf::Vector2f out, sf::Vector2f move, unsigned int width, unsigned int step):
    Position(position),
    Out(out),
    Move(move),
    Width(width),
    Step(step)
    {
    }

  ~tcParticle()
    {
    }

  sf::Vector2f Out;
  sf::Vector2f Move;
  sf::Vector2f Position;
  unsigned int Width;
  unsigned int Step;
};
