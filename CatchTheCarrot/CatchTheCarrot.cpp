/*
 * CatchTheCarrot.cpp
 *
 *  Created on: Aug 9, 2023
 *      Author: ugur
 */


#include <thread>
#include <iostream>
#include "GameMainProcesses.hpp"

int main()
{
	//Set game parameters.

	std::string gameTexturesPath = "../Images/";
	std::string gameName = "Catch The Carrot";

	GameMainProcesses mGame;

	if(mGame.LoadParameters(gameTexturesPath, gameName))
	{
		//Start Game
		mGame.Process();

		std::cout << "Game ended successfully. See you in another life brother :)" << std::endl;
	}
	else
	{
		std::cout << "A problem was encountered while loading the parameters.";
	}

	return 0;
}

