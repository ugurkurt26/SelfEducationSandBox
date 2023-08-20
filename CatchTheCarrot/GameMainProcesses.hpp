/*
 * CatchTheCarrot.hpp
 *
 *  Created on: Aug 9, 2023
 *      Author: ugur
 */

#ifndef GAMEMAINPROCESSES_HPP_
#define GAMEMAINPROCESSES_HPP_

#include <opencv2/opencv.hpp>
#include <vector>

#include "Carrot.hpp"
#include "Rabbit.hpp"
#include "MagicMushroom.hpp"

class GameMainProcesses
{
	public:
		GameMainProcesses();
		bool LoadParameters(std::string texturesPath, std::string gameName);
		void Process();
		void LoginScreen();

		Carrot mCarrot;
		Rabbit mRabbit;
		MagicMushroom mMushroom;

		enum GameLevel : unsigned int
		{
			LEVELLOW,
			LEVELMEDIUM,
			LEVELHIGH
		};

	private:
		bool collisionDetector(Object& firstCharacter, Object& secondCharacter);
		void drawScore();
		int updateNewSpeed();
		void calculateObjectNewPosition(Object& character, cv::Point newCoordinate);
		cv::Point calculateDriftMotionNextPosition(cv::Size fieldBoundaries, cv::Point currentLocation);

		static void getMouseCoordinate(int event, int x, int y, int flags, void* userdata);

		cv::Mat groundTexture;
		cv::Mat dynamicGroundTexture;
		cv::Rect windowSizes;

		std::string gameName;
		int gameProcessFPS;
		int gameLevel;
		int gameScore;

		int mushroomRespawnTime;
		int mushroomEffectTime;
		
		static cv::Point mousePosition;

		bool isParametersLoaded;
		bool mExitFlag;
};

#endif /* GAMEMAINPROCESSES_HPP_ */
