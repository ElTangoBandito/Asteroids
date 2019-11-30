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

int main()
{
	sf::RenderWindow window(sf::VideoMode(windowSizeX, windowSizeY), "My Window!");
	window.setVerticalSyncEnabled(true);
	sf::Clock clock;


	//player stuff
	Player player(playerLength, playerWidth, windowSizeX, windowSizeY);

	while (window.isOpen())
	{
		//float dt = clock.getElapsedTime().asMilliseconds();
		//clock = sf::Clock();

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


		window.display();

	}
	return 0;
}