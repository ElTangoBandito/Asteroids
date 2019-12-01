#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>
#include <SFML/System.hpp>
#include <SFML/OpenGL.hpp>
#include <SFML/Main.hpp>
#include <iostream>

class Player
{
public: 
	float length;
	float width;
	int windowSizeX;
	int windowSizeY;
	sf::Vector2f position;
	sf::Vector2f origin;
	sf::Vector2f velocity;
	int life;
	float rotation;
	float speed;
	float rotateSpeed;
	sf::Texture shipTexture;
	int velocityLife;

public:
	Player(float lengthIn, float widthIn, int windowSizeXIn, int windowSizeYIn);
	~Player();

	void updateOrigin();
	void update(float deltaTime);
	void draw(sf::RenderWindow* windowIn);

};

