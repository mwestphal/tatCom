#include <SFML/Graphics.hpp>

#include <math.h>
#include <iostream>

const int SCR_WIDTH = 800; 
const int SCR_HEIGHT = 800;
const int CIRCLE_RADIUS = 100;
const int NUMBER_OF_PARTICLES = 30;
const int MAX_STEP = 1000;

double norm(const sf::Vector2f& vector)
{
  return std::sqrt(vector.x*vector.x + vector.y*vector.y);
}

void rotateBy(sf::Vector2f& vector, float angleInRad)
{
  float cs = cos(angleInRad);
  float sn = sin(angleInRad);
  float xTmp = vector.x;
  vector.x = xTmp * cs - vector.y * sn;
  vector.y = xTmp * sn + vector.y * cs;
}

int main()
{
  // create the window
  sf::RenderWindow window(sf::VideoMode(SCR_WIDTH, SCR_HEIGHT), "TatCom");

  // run the program as long as the window is open
  while (window.isOpen())
   {
      // check all the window's events that were triggered since the last iteration of the loop
      sf::Event event;
      while (window.pollEvent(event))
        {
          // "close requested" event: we close the window
          if (event.type == sf::Event::Closed)
            window.close();
        }

      // clear the window with black color
      window.clear(sf::Color::White);


      // draw center circle
      sf::CircleShape shape(CIRCLE_RADIUS+1);
      shape.setPosition((SCR_WIDTH/2) - CIRCLE_RADIUS, (SCR_HEIGHT/2) - CIRCLE_RADIUS);
      shape.setFillColor(sf::Color::Black);
      window.draw(shape);

      // draw particle paths
      sf::Vector2f centerPoint((SCR_WIDTH/2) , (SCR_HEIGHT/2));
      for(int i = 0; i < NUMBER_OF_PARTICLES; i++)
        {
          // get position on circle
          sf::Vector2f outVector(std::cos((i * 2 * M_PI) / NUMBER_OF_PARTICLES), std::sin((i * 2 * M_PI) / NUMBER_OF_PARTICLES));
          sf::Vector2f particlePoint(centerPoint.x + CIRCLE_RADIUS * outVector.x, centerPoint.y + CIRCLE_RADIUS * outVector.y);
          sf::Vector2f moveVector = outVector;
          std::vector<sf::Vertex> vertices;
          int nStep = 0;
          while(true)
            {
              vertices.push_back(sf::Vertex(sf::Vector2f(particlePoint)));
              vertices.back().color = sf::Color::Black;
              rotateBy(moveVector, 0.0001 * (nStep ));
              particlePoint.x += moveVector.x;
              particlePoint.y += moveVector.y;
              nStep++;
              if (norm(particlePoint - centerPoint) > 3000 || nStep > MAX_STEP)
                {
                  break;
                }
            }
          window.draw(&vertices[0], vertices.size(), sf::LineStrip);
        }
      // end the current frame
      window.display();
    }

  return 0;
}