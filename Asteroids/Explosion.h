#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>
#include <SFML/System.hpp>
#include <SFML/OpenGL.hpp>
#include <SFML/Main.hpp>
#include <iostream>

class Explosion
{
public:
	float radius;
	sf::Vector2f position;
	sf::Vector2f origin;
	int life;
	int type;
	sf::Color explosionColor;
	float radiusIncreaseRate;

public:
	Explosion(sf::Vector2f positionIn, int typeIn);
	~Explosion();

	void updateOrigin();
	void update(float deltaTime);
	void draw(sf::RenderWindow* windowIn);
};

