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

Asteroid::Asteroid(float radiusIn, std::vector<Asteroid*>* asteroidListIn, sf::Vector2f positionIn){//, sf::Texture* asteroidTextureIn) {
	this->radius = radiusIn;
	this->collidedList = asteroidListIn;
	this->position = positionIn;
	this->life = 3;
	this->pi = 3.14159265358979323846;
	updateOrigin();
	//this->asteroidTexture = asteroidTextureIn;

}


Asteroid::~Asteroid(){}

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
	updateOrigin();
}

void Asteroid::draw(sf::RenderWindow* windowIn) {
	sf::CircleShape c(this->radius);
	c.setPosition(this->origin);
	c.setOrigin(this->radius, this->radius);
	c.setFillColor(sf::Color::White);
	windowIn->draw(c);
}