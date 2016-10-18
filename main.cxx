#include <SFML/Graphics.hpp>

#include <math.h>
#include <iostream>
#include <random>
#include <queue>
#include <chrono>

const int SCR_WIDTH = 1080;
const int SCR_HEIGHT = 1080;
const int CIRCLE_RADIUS = 100;
const int NUMBER_OF_PARTICLES = 30;
const int MAX_STEP = 1000;

double norm(const sf::Vector2f& vector)
{
  return std::sqrt(vector.x*vector.x + vector.y*vector.y);
}

double dot(const sf::Vector2f& u, const sf::Vector2f& v)
{
  return u.x * v.x + u.y * v.y;
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
    // construct a trivial random generator engine from a time-based seed:
  unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
  std::default_random_engine generator (seed);
  std::normal_distribution<double> distribution(0.0,M_PI/2);

  // create the window
  sf::ContextSettings settings;
  settings.antialiasingLevel = 8;
  sf::RenderWindow window(sf::VideoMode(SCR_WIDTH, SCR_HEIGHT), "TatCom", sf::Style::Default, settings);

  // run the program as long as the window is open
  while (window.isOpen())
   {
   // clear the window with black color
      window.clear(sf::Color::White);


      // draw center circle
      sf::CircleShape shape(CIRCLE_RADIUS+1);
      shape.setPosition((SCR_WIDTH/2) - (CIRCLE_RADIUS+1), (SCR_HEIGHT/2) - (CIRCLE_RADIUS+1));
      shape.setFillColor(sf::Color::Black);
      window.draw(shape);

      // draw particle paths
      sf::Vector2f centerPoint((SCR_WIDTH/2) , (SCR_HEIGHT/2));
      unsigned int nParticle = (2 * M_PI * CIRCLE_RADIUS)/8;
//      unsigned int nParticle = 6;
      std::queue<std::pair<sf::Vector2f, sf::Vector2f> > particleQueue;
      for(int i = 0; i < nParticle; i++)
        {
          // get position on circle
          sf::Vector2f outVector(std::cos((i * 2 * M_PI) / nParticle), std::sin((i * 2 * M_PI) / nParticle));
          sf::Vector2f particlePoint(centerPoint.x + CIRCLE_RADIUS * outVector.x, centerPoint.y + CIRCLE_RADIUS * outVector.y);
          particleQueue.push(std::make_pair(particlePoint, outVector));
        }

      while (!particleQueue.empty())
        {
          std::pair<sf::Vector2f, sf::Vector2f> particle = particleQueue.front();
          particleQueue.pop();
          sf::Vector2f particlePoint = particle.first;
          sf::Vector2f outVector = particle.second;
          sf::Vector2f moveVector = outVector;
          sf::Vector2f normalVector(-moveVector.y, moveVector.x);
          unsigned int width = 20;
          sf::Vector2f previousPoint1(particlePoint + sf::Vector2f((width / 2) * normalVector.x, (width / 2) * normalVector.y));
          sf::Vector2f previousPoint2(particlePoint + sf::Vector2f((width / 2) * -normalVector.x, (width / 2) * -normalVector.y));
          int nStep = 0;
          while(true)
            {
              // create an empty shape
              sf::ConvexShape convex;

              // resize it to 5 points
              convex.setPointCount(4);
              convex.setFillColor(sf::Color::Black);

              // define the points
              convex.setPoint(0, previousPoint1);
              convex.setPoint(1, previousPoint2);

              //rotateBy(moveVector, 0.0001 * (nStep ));
              moveVector = outVector;
              rotateBy(moveVector, distribution(generator));

              particlePoint.x += moveVector.x*1;
              particlePoint.y += moveVector.y*1;
              nStep++;
              normalVector = sf::Vector2f(-moveVector.y, moveVector.x);
              previousPoint1 = sf::Vector2f(particlePoint + sf::Vector2f((width / 2) * normalVector.x, (width / 2) * normalVector.y));
              previousPoint2 = sf::Vector2f(particlePoint + sf::Vector2f((width / 2) * -normalVector.x, (width / 2) * -normalVector.y));
              convex.setPoint(2, previousPoint2);
              convex.setPoint(3, previousPoint1);
              window.draw(convex);
              if (norm(particlePoint - centerPoint) > 300 || nStep > MAX_STEP)
                {
                  break;
                }
            }
        }
      // end the current frame
      window.display();

      while (window.isOpen())
        {
        // check all the window's events that were triggered since the last iteration of the loop
        sf::Event event;
        while (window.pollEvent(event))
          {
          // "close requested" event: we close the window
          if (event.type == sf::Event::Closed)
            {
            window.close();
            }
          }
        if (event.type == sf::Event::KeyPressed)
          {
          break;
          }
        }
   }
  return 0;
}
