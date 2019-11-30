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
	this->rotateSpeed = 0.1;
	shipTexture.loadFromFile("Resources/Textures/Ship.png");
	updateOrigin();
}

Player::~Player() {};

void Player::updateOrigin() {
	this->origin.x = this->position.x + this->width/2;
	this->origin.y = this->position.y + this->length / 2;
}

void Player::update(float deltaTimeIn) {
	double pi = 3.14159265358979323846;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left)) {
		this->rotation -= rotateSpeed * deltaTimeIn;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right)) {
		this->rotation += rotateSpeed * deltaTimeIn;
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up)) {
		this->velocity.x = std::cos(this->rotation * pi /180.0 ) * speed * deltaTimeIn;
		this->velocity.y = std::sin(this->rotation * pi / 180.0) * speed * deltaTimeIn;
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
		
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down)) {
		this->velocity.x = std::cos(this->rotation * pi / 180.0) * speed * deltaTimeIn;
		this->velocity.y = std::sin(this->rotation * pi / 180.0) * speed * deltaTimeIn;
		this->position.x += this->velocity.x;
		this->position.y += this->velocity.y;
		if (this->position.x + this->width >= this->windowSizeX || this->position.x <= 0) {
			this->position.x -= this->velocity.x;
			this->position.y -= this->velocity.y;
		}
		if (this->position.y + this->length >= this->windowSizeY || this->position.y <= 0) {
			this->position.x -= this->velocity.x;
			this->position.y -= this->velocity.y;
		}
	}
	updateOrigin();
	this->velocity = sf::Vector2f(0, 0);
}

void Player::draw(sf::RenderWindow* windowIn) {
	sf::RectangleShape rect(sf::Vector2f(this->length, this->width));
	rect.setTexture(&this->shipTexture);
	rect.setPosition(this->origin);
	rect.setOrigin(this->length/2, this->width/2);
	rect.setFillColor(sf::Color::White);
	rect.rotate(this->rotation);
	this->rotation = rect.getRotation();
	windowIn->draw(rect);
}