#include "Player.h"
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

Player::Player(float lengthIn, float widthIn, int windowSizeXIn, int windowSizeYIn) {
	this->length = lengthIn;
	this->width = widthIn;
	this->windowSizeX = windowSizeXIn;
	this->windowSizeY = windowSizeYIn;
	this->position.x = windowSizeXIn / 2 - width / 2;
	this->position.y = windowSizeYIn / 2 - length / 2;
	this->life = 1;
	this->rotation = 90;
	this->speed = 0.15;
	this->rotateSpeed = 0.2;
	this->velocityLife = 0;
	shipTexture.loadFromFile("Resources/Textures/Ship.png");
	this->shipColor = sf::Color::White;
	this->collided = false;
	this->fireRate = 0;
	this->invincibleTime = 200;
	this->radius = this->length / 2;
	this->mouseControl = false;
	updateOrigin();
}

Player::~Player() {};

void Player::reset() {
	this->velocityLife = 0;
	this->rotation = 90;
	this->collided = false;
	this->life = 1;
}

void Player::updateOrigin() {
	this->origin.x = this->position.x + this->width/2;
	this->origin.y = this->position.y + this->length / 2;
	this->upLeft = this->position;
	this->upRight.x = this->position.x + this->radius * 2;
	this->upRight.y = this->position.y;
	this->downLeft.x = this->position.x;
	this->downLeft.y = this->position.y + this->radius * 2;
	this->downRight.x = this->position.x + this->radius * 2;
	this->downRight.y = this->position.y + this->radius * 2;
}

void Player::update(float deltaTimeIn, sf::Vector2f mouseCoordIn) {
	double pi = 3.14159265358979323846;
	bool goRight = false;
	bool goLeft = false;
	if (this->mouseControl) {
		float dx = this->origin.x - mouseCoordIn.x;
		float dy = this->origin.y - mouseCoordIn.y;
		float dRotation = ((atan2(dy, dx)) * 180 / pi);
		if (dRotation < 0) {
			dRotation += 360;
		}
		float dRight = abs(this->rotation - dRotation);
		if (dRight < 180) {
			if (this->rotation < dRotation) {
				goRight = true;
			}
			else {
				goLeft = true;
			}
		}
		else {
			if (this->rotation < dRotation) {
				goLeft = true;
			}
			else {
				goRight = true;
			}
		}
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left) || goLeft) {
		this->rotation -= rotateSpeed * deltaTimeIn;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right) || goRight) {
		this->rotation += rotateSpeed * deltaTimeIn;
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up) || playerMouse.isButtonPressed(sf::Mouse::Button::Right)) {
		this->velocityLife = 300;
		this->velocity.x = std::cos(this->rotation * pi /180.0 ) * speed * deltaTimeIn;
		this->velocity.y = std::sin(this->rotation * pi / 180.0) * speed * deltaTimeIn;
	}

	if (this->velocityLife >= 0) {
		this->velocityLife--;
		this->velocity.x = this->velocity.x * .99;
		this->velocity.y = this->velocity.y * .99;
	}
	else {
		this->velocity = sf::Vector2f(0, 0);
	}
	this->position.x -= this->velocity.x;
	this->position.y -= this->velocity.y;
	if (this->position.x + this->width >= this->windowSizeX || this->position.x <= 0) {
		this->position.x += this->velocity.x;
		this->position.y += this->velocity.y;
	}
	if (this->position.y + this->length >= this->windowSizeY || this->position.y <= 0) {
		this->position.x += this->velocity.x;
		this->position.y += this->velocity.y;
	}
	if (collided) {
		this->shipColor = sf::Color::Yellow;
	}
	else {
		//this->shipColor = sf::Color::White;
	}
	if (this->fireRate > 0) {
		this->fireRate--;
	}
	if (this->invincibleTime > 0) {
		invincibleTime--;
		if (this->invincibleTime % 5 == 0) {
			this->shipColor = sf::Color(rand() % 256, rand() % 256, rand() % 256, 255);
		}
	} else {
		this->shipColor = sf::Color::White;
	}
	updateOrigin();
}

void Player::draw(sf::RenderWindow* windowIn) {
	sf::RectangleShape rect(sf::Vector2f(this->length, this->width));
	rect.setTexture(&this->shipTexture);
	rect.setPosition(this->origin);
	rect.setOrigin(this->length/2, this->width/2);
	rect.setFillColor(shipColor);
	rect.rotate(this->rotation);
	this->rotation = rect.getRotation();
	windowIn->draw(rect);
}