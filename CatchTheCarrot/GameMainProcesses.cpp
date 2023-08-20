/*
 * CatchTheCarrot.cpp
 *
 *  Created on: Aug 9, 2023
 *      Author: ugur
 */

#include <thread>
#include <iostream>
#include "GameMainProcesses.hpp"

GameMainProcesses::GameMainProcesses()
{
	this->isParametersLoaded = false;
	this->gameName = "Unknown Game";
}

cv::Point GameMainProcesses::mousePosition = cv::Point(0, 0);

bool GameMainProcesses::LoadParameters(std::string texturesPath, std::string gameName)
{
	isParametersLoaded = false;

	try
	{
		gameScore = 0;
		this->gameName = gameName;
		groundTexture = cv::imread(texturesPath + "grass_texture.jpg");
		cv::resize(groundTexture, groundTexture, cv::Size(500, 500));
		dynamicGroundTexture = groundTexture;
		windowSizes = cv::Rect(0, 0, dynamicGroundTexture.cols, dynamicGroundTexture.rows);

		// Set characters sizes
		mCarrot.size = cv::Size(30, 30);
		mRabbit.size = cv::Size(50, 50);
		mMushroom.size = cv::Size(30, 30);

		// Load characters textures
		mCarrot.LoadTexturesFromPath(texturesPath + "Carrot", false);
		mRabbit.LoadTexturesFromPath(texturesPath + "Rabbit", false);
		mMushroom.LoadTexturesFromPath(texturesPath + "MushRoom", true);

		// Set characters speeds
		mCarrot.UpdateSpeed(25);
		mRabbit.UpdateSpeed(20);
		mMushroom.UpdateSpeed(15);

		// Set characters start positions
		mCarrot.position = cv::Point(50, 50);
		mRabbit.position = cv::Point(50, 50);
		mMushroom.position = cv::Point(50, 100);

		mousePosition = mRabbit.position;
		mCarrot.UpdateNewPosition(cv::Size(windowSizes.width, windowSizes.height), cv::Rect(mRabbit.position, mRabbit.size));

		mushroomRespawnTime = 15;
		mushroomEffectTime = 5;

		isParametersLoaded = true;
	}
	catch (const std::exception &e)
	{
		std::cerr << e.what() << '\n';
	}

	return isParametersLoaded;
}

void GameMainProcesses::Process()
{
	mExitFlag = false;

	int desiredFrameRate = 10; // Game FPS
	std::chrono::nanoseconds frameDuration(1000000000 / desiredFrameRate);
	auto startTime = std::chrono::high_resolution_clock::now();

	// Mushroom respawn algorithm paramters
	auto mushroomLastSpawnTime = startTime;
	auto mushroomCurrentSpawnTime = startTime;
	auto mushroomEffectStartTime = startTime;
	bool isSpawn = false;
	bool hasCollided = false;
	bool mushroomEffectExitFlag = false;
	int mushroomEffectNumber = 0;
	boost::random::random_device rng;
	boost::random::uniform_int_distribution<> mushroomRandomEfect(0, 2);

	boost::random::uniform_int_distribution<> carrotRandomAngle(1, 360);

	cv::namedWindow(gameName, cv::WINDOW_NORMAL);

	while (!mExitFlag)
	{
		// Processes Start
		startTime = std::chrono::high_resolution_clock::now();

		// clean terrain
		dynamicGroundTexture = groundTexture.clone();
		windowSizes = cv::Rect(0, 0, dynamicGroundTexture.cols, dynamicGroundTexture.rows);

		// Check rabbit-mushroom interaction
		auto currentTime = std::chrono::high_resolution_clock::now();

		auto elapsedMushroomSpawnTime = std::chrono::duration_cast<std::chrono::seconds>(currentTime - mushroomLastSpawnTime);
		if (elapsedMushroomSpawnTime.count() >= mushroomRespawnTime)
		{
			if (!isSpawn)
			{
				mushroomCurrentSpawnTime = std::chrono::high_resolution_clock::now();
				isSpawn = true;
			}

			auto elapsedmushroomCurrentSpawnTime = std::chrono::duration_cast<std::chrono::seconds>(currentTime - mushroomCurrentSpawnTime);
			if ((elapsedmushroomCurrentSpawnTime.count() >= mushroomEffectTime) || hasCollided)
			{
				if (!hasCollided)
				{
					mushroomEffectExitFlag = true;
				}
				else
				{
					cv::Point groundMidpoint = cv::Point (dynamicGroundTexture.cols/2, dynamicGroundTexture.rows/2);

					mMushroom.collisionEffects(mRabbit, mousePosition, groundMidpoint, 0);
					auto elapsedmushroomEffectTime = std::chrono::duration_cast<std::chrono::seconds>(currentTime - mushroomEffectStartTime);

					if (elapsedmushroomEffectTime.count() >= mushroomEffectTime)
					{
						mushroomEffectExitFlag = true;
					}
				}
			}
			else
			{
				calculateObjectNewPosition(mMushroom, mRabbit.position);

				if (collisionDetector(mMushroom, mRabbit))
				{
					hasCollided = true;
					mushroomEffectStartTime = std::chrono::high_resolution_clock::now();
				}
			}
		}

		if (mushroomEffectExitFlag)
		{
			mushroomEffectExitFlag = false;
			isSpawn = false;
			hasCollided = false;
			mushroomLastSpawnTime = std::chrono::high_resolution_clock::now();
			mushroomEffectNumber = mushroomRandomEfect(rng); 
		}

		// Check rabbit-mouse interaction
		cv::setMouseCallback(gameName, getMouseCoordinate);
		calculateObjectNewPosition(mRabbit, mousePosition);

		// Check rabbit-carrot interaction
		if (collisionDetector(mRabbit, mCarrot))
		{
			// drop new random coordinate
			mCarrot.UpdateNewPosition(cv::Size(windowSizes.width, windowSizes.height), cv::Rect(mRabbit.position, mRabbit.size));
			mCarrot.angle = (double)carrotRandomAngle(rng);

			gameScore += 1;
		}
		else
		{
			calculateObjectNewPosition(mCarrot, mCarrot.position);
		}

		drawScore();

		cv::imshow(gameName, dynamicGroundTexture);

		if (cv::waitKey(30) == 27) // Wait for 'esc' key press to exit
		{
			mExitFlag = true;
		}

		// Processes End

		// set loop speed based on set fps
		auto endTime = std::chrono::high_resolution_clock::now();
		auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(endTime - startTime);
		auto elapsedAsSec = std::chrono::duration_cast<std::chrono::seconds>(endTime - startTime);
		if (elapsed < frameDuration)
		{
			std::this_thread::sleep_for(frameDuration - elapsed);
		}
	}

	cv::destroyAllWindows();
}

void GameMainProcesses::calculateObjectNewPosition(Object &character, cv::Point newCoordinate)
{
	// prevent character from leaving the field
	if (newCoordinate.x < 0)
	{
		newCoordinate.x = 0;
	}
	else if (newCoordinate.x > windowSizes.width)
	{
		newCoordinate.x = windowSizes.width - character.size.width;
	}

	if (newCoordinate.y < 0)
	{
		newCoordinate.y = 0;
	}
	else if (newCoordinate.y > windowSizes.height)
	{
		newCoordinate.y = windowSizes.height - character.size.height;
	}
	
	character.UpdateNewPosition(newCoordinate, windowSizes.size());
	character.ChangeAnimation(newCoordinate);

	// Overlay character texture over ground texture(with transparent)

	for (int i = 0; i < (*character.GetCharacterImage()).rows; ++i)
	{
		for (int j = 0; j < (*character.GetCharacterImage()).cols; ++j)
		{
			cv::Vec4b pixel = (*character.GetCharacterImage()).at<cv::Vec4b>(i, j);
			if (pixel[3] > 0)
			{
				// Alfa kanalı > 0 ise (transparan değilse)
				dynamicGroundTexture.at<cv::Vec3b>(character.position.y + i, character.position.x + j)[0] = pixel[0];
				dynamicGroundTexture.at<cv::Vec3b>(character.position.y + i, character.position.x + j)[1] = pixel[1];
				dynamicGroundTexture.at<cv::Vec3b>(character.position.y + i, character.position.x + j)[2] = pixel[2];
			}
		}
	}
}

void GameMainProcesses::getMouseCoordinate(int event, int x, int y, int flags, void *userdata)
{
	if (event == cv::EVENT_MOUSEMOVE)
	{
		mousePosition.x = x;
		mousePosition.y = y;
	}
}

bool GameMainProcesses::collisionDetector(Object &firstCharacter, Object &secondCharacter)
{
	bool detectCollision = false;

	cv::Rect firstCharacterBBox = cv::Rect(firstCharacter.position, firstCharacter.size);
	cv::Rect secondCharacterBBox = cv::Rect(secondCharacter.position, secondCharacter.size);

	if ((firstCharacterBBox & secondCharacterBBox).area() > 0)
	{
		detectCollision = true;
	}

	return detectCollision;
}

void GameMainProcesses::drawScore()
{
	// Metin ve yazı tipi ayarları
	std::string text = std::to_string(this->gameScore);
	int fontFace = cv::FONT_HERSHEY_SIMPLEX;
	double fontScale = 1;
	cv::Scalar fontColor(255, 255, 255); // Beyaz renk
	int thickness = 2;

	// Metnin boyutunu hesapla
	cv::Size textSize = cv::getTextSize(text, fontFace, fontScale, thickness, 0);

	// Metni görüntünün sağ üst köşesine yerleştir
	int x = windowSizes.width - textSize.width - 20; // 20 piksel sağdan boşluk
	int y = textSize.height + 20;							 // 20 piksel yukarıdan boşluk
	cv::Point position(x, y);

	// Metni görüntüye yazdır
	cv::putText(dynamicGroundTexture, text, position, fontFace, fontScale, fontColor, thickness);
}

cv::Point GameMainProcesses::calculateDriftMotionNextPosition(cv::Size fieldBoundaries, cv::Point currentLocation)
{
}