#include <SFML/Graphics.hpp>

#include <math.h>
#include <iostream>
#include <random>
#include <queue>
#include <chrono>
#include <cstdlib>
#include <iostream>
#include <ctime>

#include "tcParticle.h"

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
  std::srand(std::time(0));
  
  // create the window
  sf::ContextSettings settings;
  settings.antialiasingLevel = 8;
  sf::RenderWindow window(sf::VideoMode(SCR_WIDTH, SCR_HEIGHT), "TatCom", sf::Style::Default, settings);

  // run the program as long as the window is open
  while (window.isOpen())
    {
    window.clear(sf::Color::White);

    // draw center circle
    sf::CircleShape shape(CIRCLE_RADIUS+1);
    shape.setPosition((SCR_WIDTH/2) - (CIRCLE_RADIUS+1), (SCR_HEIGHT/2) - (CIRCLE_RADIUS+1));
    shape.setFillColor(sf::Color::Black);
    window.draw(shape);

    // draw particle paths
    sf::Vector2f centerPoint((SCR_WIDTH/2) , (SCR_HEIGHT/2));
    unsigned int nParticle = (2 * M_PI * CIRCLE_RADIUS)/64;
//    unsigned int nParticle = 1;
    std::queue<tcParticle> particleQueue;
    for(int i = 0; i < nParticle; i++)
      {
      // get position on circle
      sf::Vector2f out(std::cos((i * 2 * M_PI) / nParticle), std::sin((i * 2 * M_PI) / nParticle));
      sf::Vector2f position(centerPoint.x + CIRCLE_RADIUS * out.x, centerPoint.y + CIRCLE_RADIUS * out.y);
      particleQueue.push(tcParticle(position, out));
      }
    std::cout<<"nPart:"<<particleQueue.size()<<std::endl;

    while (!particleQueue.empty())
      {
      tcParticle particle = particleQueue.front();
      particleQueue.pop();
      std::cout<<"nPart:"<<particleQueue.size()<<std::endl;
      sf::Vector2f normal(-particle.Move.y, particle.Move.x);
      sf::Vector2f previousPoint1(particle.Position + sf::Vector2f((particle.Width / 2) * normal.x, (particle.Width / 2) * normal.y));
      sf::Vector2f previousPoint2(particle.Position + sf::Vector2f((particle.Width / 2) * -normal.x, (particle.Width / 2) * -normal.y));
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

        //rotateBy(move, 0.0001 * (nStep ));
        particle.Move = particle.Out;
        rotateBy(particle.Move, distribution(generator));

        particle.Position.x += particle.Move.x*1;
        particle.Position.y += particle.Move.y*1;
        nStep++;
        normal = sf::Vector2f(-particle.Move.y, particle.Move.x);
        previousPoint1 = sf::Vector2f(particle.Position + sf::Vector2f((particle.Width / 2) * normal.x, (particle.Width / 2) * normal.y));
        previousPoint2 = sf::Vector2f(particle.Position + sf::Vector2f((particle.Width / 2) * -normal.x, (particle.Width / 2) * -normal.y));
        convex.setPoint(2, previousPoint2);
        convex.setPoint(3, previousPoint1);
        window.draw(convex);
        if (std::rand()%300 == 0 && particle.Width >= 4)// && particleQueue.size() < 100)
          {
          particleQueue.push(tcParticle(particle.Position, particle.Move, particle.Move, particle.Width/2, particle.Step));
          particle.Width-=3;
          std::cout<<"nPart:"<<particleQueue.size()<< " " << particle.Width <<std::endl;
          }
        if (norm(particle.Position - centerPoint) > 300 || nStep > MAX_STEP || particle.Width == 0)
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
