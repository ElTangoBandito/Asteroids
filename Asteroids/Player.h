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
	sf::Color shipColor;
	bool collided;
	int invincibleTime;
	int fireRate;
	sf::Vector2f upLeft;
	sf::Vector2f downLeft;
	sf::Vector2f upRight;
	sf::Vector2f downRight;
	float radius;
	sf::Mouse playerMouse;
	bool mouseControl;
	sf::Sound* boosterSound;
	bool boostSoundPlayed;

public:
	Player(float lengthIn, float widthIn, int windowSizeXIn, int windowSizeYIn, sf::Sound* soundIn);
	~Player();

	void reset();
	void updateOrigin();
	void update(float deltaTime, sf::Vector2f mouseCoordIn);
	void draw(sf::RenderWindow* windowIn);

};

