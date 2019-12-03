#include "Asteroid.h"
#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>
#include <SFML/System.hpp>
#include <SFML/OpenGL.hpp>
#include <SFML/Main.hpp>
#include <math.h>
#include <iostream>

Asteroid::Asteroid(int lifeIn, sf::Vector2f positionIn, int windowSizeXIn, int windowSizeYIn, sf::Texture* asteroidTextureIn){//, sf::Texture* asteroidTextureIn) {
	this->position = positionIn;
	this->life = lifeIn;
	switch (this->life) {
	case 1:
		this->radius = 7.0f;
		break;
	case 2:
		this->radius = 12.0f;
		break;
	case 3:
		this->radius = 20.0f;
		break;
	}
	this->pi = 3.14159265358979323846;
	this->asteroidColor = sf::Color::White;
	this->windowSizeX = windowSizeXIn;
	this->windowSizeY = windowSizeYIn;
	this->velocity = sf::Vector2f(0.0f, 0.0f);
	this->collideLifeColor = 0;
	this->collidedWithAsteroid = false;
	this->collidedList = new std::vector<Asteroid*>;
	this->markedForDelete = false;
	this->asteroidTexture = asteroidTextureIn;
	this->rotation = 0;
	this->rotationRate = rand() % 20 - 10;
	this->soundFreeLife = 10;
	updateOrigin();
	//this->asteroidTexture = asteroidTextureIn;
}

Asteroid::~Asteroid(){}

void Asteroid::spawnChildren(std::vector <Asteroid*>* asteroidListIn, int stageLevelIn) {
	int randomX = rand() % (int)this->radius - (int)this->radius;
	int randomY = rand() % (int)this->radius - (int)this->radius;
	int speedModifier = 20;
	float xVelocity = (rand() % (speedModifier + stageLevelIn) - (speedModifier / 2)) * 0.01;
	float yVelocity = (rand() % (speedModifier + stageLevelIn) - (speedModifier / 2)) * 0.01;
	Asteroid* newAsteroid;
	newAsteroid = new Asteroid(this->life, sf::Vector2f(this->origin.x + randomX, this->origin.y + randomY), windowSizeX, windowSizeY, this->asteroidTexture);
	if (randomX >= 0) {
		xVelocity = abs(xVelocity);
	}
	else {
		xVelocity = -abs(xVelocity);
	}
	if (randomY >= 0) {
		yVelocity = abs(yVelocity);
	}
	else {
		yVelocity = -abs(yVelocity);
	}

	newAsteroid->velocity = sf::Vector2f(xVelocity, yVelocity);
	asteroidListIn->push_back(newAsteroid);
}

void Asteroid::updateOrigin() {
	this->origin.x = this->position.x + radius;
	this->origin.y = this->position.y + radius;
	this->upLeft = this->position;
	this->upRight.x = this->position.x + radius * 2;
	this->upRight.y = this->position.y;
	this->downLeft.x = this->position.x;
	this->downLeft.y = this->position.y + radius * 2;
	this->downRight.x = this->position.x + radius * 2;
	this->downRight.y = this->position.y + radius * 2;
	//std::cout << "UpLeft: " << this->upLeft.x << ", " << this->upLeft.y << "\n";
	//std::cout << "UpRight: " << this->upRight.x << ", " << this->upRight.y << "\n";
	//std::cout << "downLeft: " << this->downLeft.x << ", " << this->downLeft.y << "\n";
	//std::cout << "downRight: " << this->downRight.x << ", " << this->downRight.y << "\n";
}

void Asteroid::update(float deltaTime) {
	this->position.x += this->velocity.x * deltaTime;
	this->position.y += this->velocity.y * deltaTime;
	if (this->position.x <= -this->radius * 2) {
		this->position.x = this->windowSizeX;
	}
	else if (this->position.x >= windowSizeX) {
		this->position.x = -this->radius*2;
	}
	if (this->position.y <= -this->radius * 2) {
		this->position.y = this->windowSizeY;
	}
	else if (this->position.y >= windowSizeY) {
		this->position.y = -this->radius * 2;
	}
	if (collideLifeColor > 0) {
		collideLifeColor--;
		asteroidColor = sf::Color::Yellow;
	}
	else {
		asteroidColor = sf::Color::White;
	}
	this->collidedWithAsteroid = false;
	this->collidedList->clear();
	if (this->soundFreeLife > 0) {
		this->soundFreeLife--;
	}
	updateOrigin();
}

void Asteroid::draw(sf::RenderWindow* windowIn) {
	sf::CircleShape c(this->radius);
	c.setPosition(this->origin);
	c.setOrigin(this->radius, this->radius);
	c.setFillColor(asteroidColor);
	c.setTexture(this->asteroidTexture);
	c.setRotation(this->rotation);
	this->rotation += this->rotationRate;
	windowIn->draw(c);
}