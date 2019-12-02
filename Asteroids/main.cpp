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
int gameScore = 0;
bool gameStart = false;
bool gameStartSelected = true;
bool exitGameSelected = false;

void updateDT(sf::Clock* clockIn) {
	deltaTimePrev = deltaTimeCurrent;
	deltaTimeCurrent = clockIn->getElapsedTime().asMilliseconds();
	deltaTime = deltaTimeCurrent - deltaTimePrev;
}

void checkShipCollision(Asteroid* asteroidIn, Player* playerIn) {
	float distance = sqrt(pow((playerIn->origin.x - asteroidIn->origin.x), 2) + pow((playerIn->origin.y - asteroidIn->origin.y), 2));
	float sum = playerIn->length/2 + asteroidIn->radius;
	if (distance < sum && playerIn->invincibleTime == 0 && !gameOver) {
		playerLives--;
		if (playerLives == 0) {
			gameOver = true;
		}
		else {
			playerIn->position.x = windowSizeX / 2 - playerIn->width / 2;
			playerIn->position.y = windowSizeY / 2 - playerIn->length / 2;
			playerIn->invincibleTime = 200;
		}
	}
}

void checkShipCollisionLists(Player* playerIn, std::vector <std::vector<Asteroid*>*>* bucketListIn) {
	int upLeft = findBucket(playerIn->upLeft.x, playerIn->upLeft.y);
	int upRight = findBucket(playerIn->upRight.x, playerIn->upRight.y);
	int downLeft = findBucket(playerIn->downLeft.x, playerIn->downLeft.y);
	int downRight = findBucket(playerIn->downRight.x, playerIn->downRight.y);
	if (upLeft != -1) {
		for (int j = 0; j < bucketListIn->at(upLeft)->size(); j++) {
			checkShipCollision(bucketListIn->at(upLeft)->at(j), playerIn);
		}
	}
	if (upRight != -1 && upRight != upLeft) {
		for (int j = 0; j < bucketListIn->at(upRight)->size(); j++) {
			checkShipCollision(bucketListIn->at(upRight)->at(j), playerIn);
		}
	}
	if (downLeft != -1 && downLeft != upLeft && downLeft != upRight) {
		for (int j = 0; j < bucketListIn->at(downLeft)->size(); j++) {
			checkShipCollision(bucketListIn->at(downLeft)->at(j), playerIn);
		}
	}
	if (downRight != -1 && downRight != upLeft && downRight != upRight && downRight != downLeft) {
		for (int j = 0; j < bucketListIn->at(downRight)->size(); j++) {
			checkShipCollision(bucketListIn->at(downRight)->at(j), playerIn);
		}
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
	if (distance < sum && !laserIn->outOfRange && gameStart) {
		laserIn->outOfRange = true;
		asteroidIn->life--;
		asteroidIn->markedForDelete = true;
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

void reset(std::vector<Laser*>* laserListIn, std::vector<Asteroid*>* asteroidListIn, Player* playerIn) {
	gameStart = false;
	gameStartSelected = true;
	exitGameSelected = false;
	gameOver = false;
	stageLevel = 1;
	playerLives = 3;
	laserListIn->clear();
	asteroidListIn->clear();
	playerIn->position.x = windowSizeX / 2 - playerIn->width / 2;
	playerIn->position.y = windowSizeY / 2 - playerIn->length / 2;
	playerIn->invincibleTime = 200;
	playerIn->reset();
}

int main()
{
	srand(time(0));
	sf::RenderWindow window(sf::VideoMode(windowSizeX, windowSizeY), "Asteroids Shenanigans!");
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

	//font stuff
	sf::Font font;
	if (!font.loadFromFile("Resources/Fonts/Mister Pumpkins Aged.ttf"))
		return -1;
	sf::Text scoreText;
	scoreText.setFont(font);
	scoreText.setFillColor(sf::Color::White);
	scoreText.setCharacterSize(20);
	scoreText.setPosition(sf::Vector2f(62, 0));
	sf::Text scoreStringText;
	scoreStringText.setFont(font);
	scoreStringText.setFillColor(sf::Color::White);
	scoreStringText.setCharacterSize(20);
	scoreStringText.setString("Score :");
	sf::Text livesText;
	livesText.setFont(font);
	livesText.setFillColor(sf::Color::White);
	livesText.setString("Lives :");
	livesText.setPosition(sf::Vector2f(windowSizeX - 150, 0));
	livesText.setCharacterSize(20);
	sf::Text livesDisplay;
	livesDisplay.setFont(font);
	livesDisplay.setFillColor(sf::Color::White);
	livesDisplay.setCharacterSize(20);
	livesDisplay.setPosition(sf::Vector2f(windowSizeX - 90, 0));
	sf::RectangleShape mainMenu(sf::Vector2f(350, 200));;
	mainMenu.setPosition(250, 200);
	mainMenu.setFillColor(sf::Color(117, 116, 110, 100));
	mainMenu.setOutlineColor(sf::Color(255, 255, 255, 100));
	mainMenu.setOutlineThickness(3);
	sf::Text startGameText;
	startGameText.setFont(font);
	startGameText.setFillColor(sf::Color::White);
	startGameText.setCharacterSize(30);
	startGameText.setPosition(100,120);
	startGameText.setString("Press 'Enter' to select. Press 'Esc' at anytime to quit.\n            Press 'M' to toggle mouse control.");
	sf::Text startGameButtonText;
	startGameButtonText.setFont(font);
	startGameButtonText.setFillColor(sf::Color::Blue);
	startGameButtonText.setCharacterSize(25);
	startGameButtonText.setPosition(375,240);
	startGameButtonText.setString("Game Start");
	sf::Text exitGameText;
	exitGameText.setFont(font);
	exitGameText.setFillColor(sf::Color::White);
	exitGameText.setCharacterSize(25);
	exitGameText.setPosition(375, 310);
	exitGameText.setString("Exit Game");
	sf::Text gameOverText;
	gameOverText.setFont(font);
	gameOverText.setFillColor(sf::Color::White);
	gameOverText.setCharacterSize(30);
	gameOverText.setPosition(150, 300);
	gameOverText.setString("Game Over! Press 'ESC' to go back to main menu.");

	//mouse stuff
	sf::Mouse playerMouse;

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
		//for (int i = 0; i <= windowSizeX / 100; i++) {
		//	sf::RectangleShape line(sf::Vector2f(1.0f, 800));
		//	int pos = i * 100;
		//	if (i == 8) {
		//		pos -= 1;
		//	}
		//	line.setPosition(sf::Vector2f(pos, 0));
		//	window.draw(line);
		//	for (int j = 0; j <= windowSizeY/100; j++) {
		//		sf::RectangleShape line(sf::Vector2f(1.0f, 800));
		//		line.setPosition(sf::Vector2f(i * 100, j * 100));
		//		line.rotate(90);
		//		window.draw(line);
		//	}
		//}

		//sort asteroids into buckets
		clearVector(&bucketList);
		for (int i = 0; i < asteroidList.size(); i++) {
			placeIntoBuckets(&bucketList, asteroidList.at(i));
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space) || playerMouse.isButtonPressed(sf::Mouse::Button::Left) && player.fireRate == 0 && gameStart && !gameOver) {
			player.fireRate = 30;
			Laser* newLaser;
			newLaser = new Laser(player.origin, windowSizeX, windowSizeY, player.rotation);
			laserList.push_back(newLaser);
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::M)) {
			if (player.mouseControl) {
				player.mouseControl = false;
			}
			else {
				player.mouseControl = true;
			}
		}
		//collision stuff

		for (int i = 0; i < bucketList.size(); i++) {
			if (bucketList.at(i)->size() > 1) {
				checkAsteroidListCollision(bucketList.at(i));
			}
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
		sf::Vector2f mouseCoord = sf::Vector2f(sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y);
		if (gameStart && !gameOver) {
			player.update(deltaTime, mouseCoord);
			player.draw(&window);
			checkShipCollisionLists(&player, &bucketList);
		}
		//old ship collision
		for (int i = 0; i < asteroidList.size(); i++) {
			//checkShipCollision(asteroidList.at(i), &player);
			if (asteroidList.at(i)->markedForDelete) {
				asteroidRemoveList.push_back(i);
			}
			asteroidList.at(i)->update(deltaTime);
			asteroidList.at(i)->draw(&window);
		}

		//delete lasers & asteroids
		for (int i = laserRemoveList.size() - 1; i > -1; i--) {
			laserList.erase(laserList.begin() + laserRemoveList.at(i));
		}
		laserRemoveList.clear();

		for (int i = asteroidRemoveList.size() - 1; i > -1; i--) {
			switch (asteroidList.at(asteroidRemoveList.at(i))->life) {
			case 2:
				asteroidList.at(asteroidRemoveList.at(i))->spawnChildren(&asteroidList, stageLevel);
				asteroidList.at(asteroidRemoveList.at(i))->spawnChildren(&asteroidList, stageLevel);
				asteroidList.at(asteroidRemoveList.at(i))->spawnChildren(&asteroidList, stageLevel);
				break;
			case 1:
				asteroidList.at(asteroidRemoveList.at(i))->spawnChildren(&asteroidList, stageLevel);
				asteroidList.at(asteroidRemoveList.at(i))->spawnChildren(&asteroidList, stageLevel);
				break;
			case 0:
				gameScore += 10;
				break;
			}
			asteroidList.erase(asteroidList.begin() + asteroidRemoveList.at(i));
		}
		asteroidRemoveList.clear();

		if (asteroidList.size() == 0) {
			stageLevel++;
			player.position.x = windowSizeX / 2 - player.width / 2;
			player.position.y = windowSizeY / 2 - player.length / 2;
			player.invincibleTime = 200;
			createStage(&asteroidList);
		}

		std::ostringstream ss;
		ss << gameScore;
		std::ostringstream livesSS;
		livesSS << playerLives;
		std::string printMe(ss.str());
		std::string printLives(livesSS.str());
		scoreText.setString(printMe);
		livesDisplay.setString(printLives);
		if (gameStart) {
			window.draw(livesDisplay);
			window.draw(scoreText);
			window.draw(scoreStringText);
			window.draw(livesText);
		}
		else {
			window.draw(mainMenu);
			window.draw(startGameText);
			window.draw(startGameButtonText);
			window.draw(exitGameText);
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Enter)) {
				if (exitGameSelected) {
					return -1;
				}
				if (gameStartSelected) {
					gameStart = true;
				}
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up)) {
				startGameButtonText.setFillColor(sf::Color::Blue);
				exitGameText.setFillColor(sf::Color::White);
				gameStartSelected = true;
				exitGameSelected = false;
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down)) {
				startGameButtonText.setFillColor(sf::Color::White);
				exitGameText.setFillColor(sf::Color::Blue);
				gameStartSelected = false;
				exitGameSelected = true;
			}
		}

		if (gameOver) {
			window.draw(gameOverText);
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape) && gameStart == true) {
			reset(&laserList, &asteroidList, &player);
			createStage(&asteroidList);
		}
		window.display();

	}
	return 0;
}