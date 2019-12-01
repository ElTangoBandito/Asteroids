#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>
#include <SFML/System.hpp>
#include <SFML/OpenGL.hpp>
#include <SFML/Main.hpp>
#include <vector>
#include <iostream>
#include <sstream>
#include <math.h>
#include <random>
#include "Player.h"
#include "Asteroid.h"

float deltaTimePrev = 0.0f;
float deltaTimeCurrent = 0.0f;
float deltaTime = 1.0f;
int windowSizeX = 800;
int windowSizeY = 600;
float playerLength = 20.0f;
float playerWidth = 20.0f;

void updateDT(sf::Clock* clockIn) {
	deltaTimePrev = deltaTimeCurrent;
	deltaTimeCurrent = clockIn->getElapsedTime().asMilliseconds();
	deltaTime = deltaTimeCurrent - deltaTimePrev;
}

void checkShipCollision(Asteroid* asteroidIn, Player* playerIn) {
	float distance = sqrt(pow((playerIn->origin.x - asteroidIn->origin.x), 2) + pow((playerIn->origin.y - asteroidIn->origin.y), 2));
	float sum = playerIn->length/2 + asteroidIn->radius;
	if (distance < sum) {
		playerIn->collided = true;
	}
}

int main()
{
	sf::RenderWindow window(sf::VideoMode(windowSizeX, windowSizeY), "My Window!");
	window.setVerticalSyncEnabled(true);
	sf::Clock clock;

	//Asteroid vector
	std::vector <Asteroid*> asteroidList;

	//player stuff
	Player player(playerLength, playerWidth, windowSizeX, windowSizeY);

	//Asteroid stuff
	Asteroid testAsteroid(20.0f, &asteroidList, sf::Vector2f(400, 400));
	Asteroid testAsteroid2(12.0f, &asteroidList, sf::Vector2f(300, 400));
	Asteroid testAsteroid3(7.0f, &asteroidList, sf::Vector2f(200, 400));
	asteroidList.push_back(&testAsteroid);
	asteroidList.push_back(&testAsteroid2);
	asteroidList.push_back(&testAsteroid3);
	while (window.isOpen())
	{
		updateDT(&clock);

		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}
		window.clear(sf::Color::Black);

		player.update(deltaTime);
		player.draw(&window);
		for (int i = 0; i < asteroidList.size(); i++) {
			checkShipCollision(asteroidList.at(i), &player);
			asteroidList.at(i)->update(deltaTime);
			asteroidList.at(i)->draw(&window);
		}

		window.display();

	}
	return 0;
}