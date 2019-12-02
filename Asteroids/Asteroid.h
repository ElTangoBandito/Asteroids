#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>
#include <SFML/System.hpp>
#include <SFML/OpenGL.hpp>
#include <SFML/Main.hpp>
#include <iostream>

class Asteroid
{
public:
	float radius;
	sf::Vector2f position;
	sf::Vector2f origin;
	sf::Vector2f velocity;
	int life;
	sf::Texture* asteroidTexture;
	sf::Color asteroidColor;
	std::vector<Asteroid*>* collidedList;
	double pi;
	sf::Vector2f upLeft;
	sf::Vector2f downLeft;
	sf::Vector2f upRight;
	sf::Vector2f downRight;
	int windowSizeX;
	int windowSizeY;
	int collideLifeColor;
	bool collidedWithAsteroid;

public:
	Asteroid(int lifeIn, sf::Vector2f positionIn, int windowSizeX, int windowSizeY);//, sf::Texture* asteroidTextureIn);
	~Asteroid();

	void updateOrigin();
	void update(float deltaTime);
	void draw(sf::RenderWindow* windowIn);
};

