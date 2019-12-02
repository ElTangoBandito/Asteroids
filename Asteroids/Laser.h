#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>
#include <SFML/System.hpp>
#include <SFML/OpenGL.hpp>
#include <SFML/Main.hpp>
#include <iostream>
class Laser
{
public:
	float radius;
	int windowSizeX;
	int windowSizeY;
	sf::Vector2f position;
	sf::Vector2f velocity;
	sf::Vector2f origin;
	float rotation;
	float speed;
	sf::Color laserColor;
	bool outOfRange;
	double pi;

public:
	Laser(sf::Vector2f positionIn, int windowSizeXIn, int windowSizeYIn, float rotationIn);
	~Laser();

	void updateOrigin();
	void update(float deltaTime);
	void draw(sf::RenderWindow* windowIn);

};

