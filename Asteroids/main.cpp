#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>
#include <SFML/System.hpp>
#include <SFML/OpenGL.hpp>
#include <SFML/Main.hpp>
#include <vector>
#include <iostream>
#include <sstream>
#include <math.h>
#include <random>
#include "Player.h"
#include "Asteroid.h"
#include "Laser.h"

int findBucket(int xIn, int yIn);
void spawnAsteroid(std::vector <Asteroid*>* listIn);

float deltaTimePrev = 0.0f;
float deltaTimeCurrent = 0.0f;
float deltaTime = 1.0f;
int windowSizeX = 800;
int windowSizeY = 600;
float playerLength = 20.0f;
float playerWidth = 20.0f;
bool gameOver = false;
int stageLevel = 1;
int playerLives = 3;

void updateDT(sf::Clock* clockIn) {
	deltaTimePrev = deltaTimeCurrent;
	deltaTimeCurrent = clockIn->getElapsedTime().asMilliseconds();
	deltaTime = deltaTimeCurrent - deltaTimePrev;
}

void checkShipCollision(Asteroid* asteroidIn, Player* playerIn) {
	float distance = sqrt(pow((playerIn->origin.x - asteroidIn->origin.x), 2) + pow((playerIn->origin.y - asteroidIn->origin.y), 2));
	float sum = playerIn->length/2 + asteroidIn->radius;
	if (distance < sum) {
		playerIn->collided = true;
	}
}

void checkAsteroidCollision(Asteroid* asteroidIn, Asteroid* secondAsteroidIn) {
	float distance = sqrt(pow((secondAsteroidIn->origin.x - asteroidIn->origin.x), 2) + pow((secondAsteroidIn->origin.y - asteroidIn->origin.y), 2));
	float sum = secondAsteroidIn->radius + asteroidIn->radius;
	if (distance < sum ){
		asteroidIn->collideLifeColor = 30;
		secondAsteroidIn->collideLifeColor = 30;

		bool alreadyCollided = false;

		sf::Vector2f originOne = asteroidIn->origin;
		sf::Vector2f originTwo = secondAsteroidIn->origin;
		sf::Vector2f velocityOne = asteroidIn->velocity;
		sf::Vector2f velocityTwo = secondAsteroidIn->velocity;

		for (int i = 0; i < asteroidIn->collidedList->size(); i++) {
			if (asteroidIn->collidedList->at(i)->origin.x == originTwo.x && asteroidIn->collidedList->at(i)->origin.y == originTwo.y) {
				alreadyCollided = true;
			}
		}

		if (!alreadyCollided) {
			asteroidIn->collidedList->push_back(secondAsteroidIn);
			secondAsteroidIn->collidedList->push_back(asteroidIn);
			if (originOne.x >= originTwo.x) {
				asteroidIn->velocity.x = abs(velocityTwo.x);
				secondAsteroidIn->velocity.x = -(abs(velocityOne.x));
			}
			else if (originOne.x < originTwo.x) {
				asteroidIn->velocity.x = -(abs(velocityTwo.x));
				secondAsteroidIn->velocity.x = abs(velocityOne.x);
			}

			if (originOne.y >= originTwo.y) {
				asteroidIn->velocity.y = abs(velocityTwo.y);
				secondAsteroidIn->velocity.y = -abs(velocityOne.y);
			}
			else if (originOne.y < originTwo.y) {
				asteroidIn->velocity.y = -abs(velocityTwo.y);
				secondAsteroidIn->velocity.y = abs(velocityOne.y);
			}
			//std::cout << "Collision detected between asteroid 1 at: " << originOne.x << ", " << originOne.y << " and " << originTwo.x << ", " << originTwo.y << "\n";
		}
	}
}

void checkAsteroidListCollision(std::vector <Asteroid*>* listIn) {
	for (int i = 0; i < listIn->size(); i++) {
		for (int j = 0; j < listIn->size(); j++) {
			if (i != j) {
				checkAsteroidCollision(listIn->at(i), listIn->at(j));
			}
		}
	}
}

void checkLaserCollision(Asteroid* asteroidIn, Laser* laserIn) {
	float distance = sqrt(pow((laserIn->origin.x - asteroidIn->origin.x), 2) + pow((laserIn->origin.y - asteroidIn->origin.y), 2));
	float sum = laserIn->radius + asteroidIn->radius;
	if (distance < sum && !laserIn->outOfRange) {
		laserIn->outOfRange = true;
	}
}

void placeIntoBuckets(std::vector <std::vector<Asteroid*>*>* bucketListIn, Asteroid* asteroidIn) {
	int upLeft = findBucket(asteroidIn->upLeft.x, asteroidIn->upLeft.y);
	int upRight = findBucket(asteroidIn->upRight.x, asteroidIn->upRight.y);
	int downLeft = findBucket(asteroidIn->downLeft.x, asteroidIn->downLeft.y);
	int downRight = findBucket(asteroidIn->downRight.x, asteroidIn->downRight.y);
	if (upLeft != -1) {
		bucketListIn->at(upLeft)->push_back(asteroidIn);
		//std::cout << "upLeft was placed in bucket : " << upLeft << "\n";
	}
	if (upRight != -1 && upRight != upLeft) {
		bucketListIn->at(upRight)->push_back(asteroidIn);
		//std::cout << "upRight was placed in bucket : " << upRight << "\n";
	}
	if (downLeft != -1 && downLeft != upLeft && downLeft != upRight) {
		bucketListIn->at(downLeft)->push_back(asteroidIn);
		//std::cout << "downLeft was placed in bucket : " << downLeft << "\n";
	}
	if (downRight != -1 && downRight != upLeft && downRight != upRight && downRight != downLeft) {
		bucketListIn->at(downRight)->push_back(asteroidIn);
		//std::cout << "downRight was placed in bucket : " << downRight << "\n";
	}

}

int findBucket(int xIn, int yIn) {
	if (xIn <= -20 || xIn >= windowSizeX + 20 || yIn <= -20 || yIn >= windowSizeY + 20) {
		return -1;
	}
	int column = floor(xIn / 100);
	int row = floor(yIn / 100);
	int result = column + (8 * row);
	if (result > 47) {
		return -1;
	}
	return result;
}

void createStage(std::vector <Asteroid*>* listIn) {
	int numberOfAsteroids = stageLevel + 3;
	for (int i = 0; i < numberOfAsteroids; i++) {
		spawnAsteroid(listIn);
	}
}

void spawnAsteroid(std::vector <Asteroid*>* listIn) {
	int randomX = rand() % windowSizeX;
	int randomY = rand() % windowSizeY;
	int speedModifier = 20;
	float xVelocity = (rand() % (speedModifier + stageLevel) - (speedModifier/2)) * 0.01;
	float yVelocity = (rand() % (speedModifier + stageLevel) - (speedModifier/2)) * 0.01;
	Asteroid* newAsteroid;
	newAsteroid = new Asteroid(3, sf::Vector2f(randomX, randomY), windowSizeX, windowSizeY);
	newAsteroid->velocity = sf::Vector2f(xVelocity, yVelocity);
	listIn->push_back(newAsteroid);
}

void clearVector(std::vector <std::vector<Asteroid*>*>* bucketListIn) {
	for (int i = 0; i < bucketListIn->size(); i++) {
		bucketListIn->at(i)->clear();
	}
}

void reset() {
	gameOver = false;
	stageLevel = 1;
	playerLives = 3;
}

int main()
{
	srand(time(0));
	sf::RenderWindow window(sf::VideoMode(windowSizeX, windowSizeY), "My Window!");
	window.setVerticalSyncEnabled(true);
	sf::Clock clock;

	//bucketList Vector
	int numberOfBuckets = (windowSizeX / 100) * (windowSizeY / 100);
	std::vector <std::vector<Asteroid*>*> bucketList;
	for (int i = 0; i < numberOfBuckets; i++) {
		std::vector<Asteroid*>* asteroidBucketList;
		asteroidBucketList = new std::vector<Asteroid*>;
		bucketList.push_back(asteroidBucketList);
	}

	//Asteroid vector
	std::vector <Asteroid*> asteroidList;
	std::vector <int> asteroidRemoveList;

	//player stuff
	Player player(playerLength, playerWidth, windowSizeX, windowSizeY);

	//Asteroid stuff
	createStage(&asteroidList);

	//laser stuff
	std::vector <Laser*> laserList;
	std::vector <int> laserRemoveList;
	while (window.isOpen())
	{
		updateDT(&clock);

		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}
		window.clear(sf::Color::Black);

		//grind display
		for (int i = 0; i <= windowSizeX / 100; i++) {
			sf::RectangleShape line(sf::Vector2f(1.0f, 800));
			int pos = i * 100;
			if (i == 8) {
				pos -= 1;
			}
			line.setPosition(sf::Vector2f(pos, 0));
			window.draw(line);
			for (int j = 0; j <= windowSizeY/100; j++) {
				sf::RectangleShape line(sf::Vector2f(1.0f, 800));
				line.setPosition(sf::Vector2f(i * 100, j * 100));
				line.rotate(90);
				window.draw(line);
			}
		}

		//sort asteroids into buckets
		clearVector(&bucketList);
		for (int i = 0; i < asteroidList.size(); i++) {
			placeIntoBuckets(&bucketList, asteroidList.at(i));
		}

		player.update(deltaTime);
		player.draw(&window);
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space) && player.fireRate == 0) {
			player.fireRate = 30;
			Laser* newLaser;
			newLaser = new Laser(player.origin, windowSizeX, windowSizeY, player.rotation);
			laserList.push_back(newLaser);
		}
		//collision stuff

		for (int i = 0; i < bucketList.size(); i++) {
			if (bucketList.at(i)->size() > 1) {
				checkAsteroidListCollision(bucketList.at(i));
			}
		}

		for (int i = 0; i < asteroidList.size(); i++) {
			checkShipCollision(asteroidList.at(i), &player);
			asteroidList.at(i)->update(deltaTime);
			asteroidList.at(i)->draw(&window);
		}

		for (int i = 0; i < laserList.size(); i++) {
			laserList.at(i)->update(deltaTime);
			laserList.at(i)->draw(&window);
			int bucketToCheck = findBucket(laserList.at(i)->position.x, laserList.at(i)->position.y);
			if (bucketToCheck != -1) {
				for (int j = 0; j < bucketList.at(bucketToCheck)->size(); j++) {
					checkLaserCollision(bucketList.at(bucketToCheck)->at(j), laserList.at(i));
				}
			}
			if (laserList.at(i)->outOfRange) {
				laserRemoveList.push_back(i);
			}
		}


		//delete lasers & asteroids
		for (int i = laserRemoveList.size() - 1; i > -1; i--) {
			laserList.erase(laserList.begin() + laserRemoveList.at(i));
		}
		laserRemoveList.clear();
		window.display();

	}
	return 0;
}