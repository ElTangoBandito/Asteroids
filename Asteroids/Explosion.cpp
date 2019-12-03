#include "Explosion.h"
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>
#include <SFML/System.hpp>
#include <SFML/OpenGL.hpp>
#include <SFML/Main.hpp>
#include <iostream>

Explosion::Explosion(sf::Vector2f positionIn, int typeIn) {
	this->position = positionIn;
	this->type = typeIn;
	this->explosionColor = sf::Color(rand() % 256, rand() % 256, rand() % 256, 125);
	this->radius = 1.0f;
	switch (this->type) {
	case 0:
		this->life = 60;
		this->radiusIncreaseRate = 0.1;
		break;
	case 1:
		this->life = 30;
		this->radiusIncreaseRate = 0.13;
		break;
	case 2:
		this->life = 15;
		this->radiusIncreaseRate = 0.15;
		break;
	case 3:
		this->life = rand() % 30 + 40;
		this->radiusIncreaseRate = rand() % 15 * 0.01;
		break;
	}
	updateOrigin();
}

Explosion::~Explosion() {}

void Explosion::updateOrigin() {
	this->origin.x = this->position.x + this->radius;
	this->origin.y = this->position.y + this->radius;
}
void Explosion::update(float deltaTime) {
	//this->explosionColor = sf::Color(rand() % 256, rand() % 256, rand() % 256, 125);
	this->radius += this->radiusIncreaseRate * deltaTime;
	this->life--;
	updateOrigin();
}
void Explosion::draw(sf::RenderWindow* windowIn) {
	sf::CircleShape c(this->radius);
	c.setPosition(this->origin);
	c.setOrigin(this->radius*2, this->radius*2);
	c.setFillColor(sf::Color(255,255,255,0));
	c.setOutlineColor(this->explosionColor);
	c.setOutlineThickness(2);
	windowIn->draw(c);
}