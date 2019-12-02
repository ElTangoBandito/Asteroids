#include "Laser.h"
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

Laser::Laser(sf::Vector2f positionIn, int windowSizeXIn, int windowSizeYIn, float rotationIn) {
	this->position = positionIn;
	this->windowSizeX = windowSizeXIn;
	this->windowSizeY = windowSizeYIn;
	this->radius = 0.5;
	this->laserColor = sf::Color::Green;
	this->outOfRange = false;
	this->speed = 0.5;
	this->rotation = rotationIn;
	this-> pi = 3.14159265358979323846;
	updateOrigin();
}
Laser::~Laser() {}

void Laser::updateOrigin() {
	this->origin.x = this->position.x + this->radius;
	this->origin.y = this->position.y + this->radius;
}

void Laser::update(float deltaTimeIn) {
	this->velocity.x = std::cos(this->rotation * pi / 180.0) * speed * deltaTimeIn;
	this->velocity.y = std::sin(this->rotation * pi / 180.0) * speed * deltaTimeIn;
	this->position.x -= this->velocity.x;
	this->position.y -= this->velocity.y;
	if (this->position.x <= -20 || this->position.x >= windowSizeX + 20 || this->position.y <= -20 || this->position.y >= windowSizeY + 20) {
		this->outOfRange = true;
	}
	updateOrigin();
}
void Laser::draw(sf::RenderWindow* windowIn) {
	if (!this->outOfRange) {
		sf::RectangleShape line(sf::Vector2f(2.0f, 10));
		line.setPosition(this->position);
		line.setRotation(this->rotation + 90);
		line.setFillColor(laserColor);
		windowIn->draw(line);
	}
}