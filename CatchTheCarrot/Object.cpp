/*
 * Object.cpp
 *
 *  Created on: Aug 11, 2023
 *      Author: ugur
 */

#include <vector>
#include "Object.hpp"

Object::Object()
{
	this->size = cv::Size(50,50);
}
Object::~Object()
{
}

std::shared_ptr<cv::Mat> Object::GetCharacterImage()
{
	return selectedTexture.second;
}

void Object::ChangeAnimation(cv::Point directionToLook)
{
	if( textures.size() > 0 && (selectedTexture.first + 1) < static_cast<int>(textures.size()) )
	{
		selectedTexture.first += 1;
		selectedTexture.second = std::make_shared<cv::Mat>(textures.at(selectedTexture.first));
	}
	else
	{
		selectedTexture.first = 0;
		selectedTexture.second = std::make_shared<cv::Mat>(textures.at(0));
	}
}

void Object::LoadTexturesFromPath(std::string texturesPathasString, bool isFlip)
{
	texturesPath = boost::filesystem::path(texturesPathasString);
	cv::Mat tempTexture;

	if( boost::filesystem::is_directory(texturesPath) )
	{
		boost::filesystem::directory_iterator end_itr;

		for( boost::filesystem::directory_iterator itr(texturesPath); itr != end_itr; ++itr )
		{
			if( boost::filesystem::is_regular_file(itr->path()) && itr->path().extension() == ".png" )
			{
				tempTexture = cv::imread(itr->path().string(), cv::IMREAD_UNCHANGED);

				cv::resize(tempTexture, tempTexture, this->size);

				if(isFlip)
				{
					cv::Mat flippedImage;
					cv::flip(tempTexture, flippedImage, 1);

					tempTexture = flippedImage;
				}

				this->textures.push_back(tempTexture);
			}
		}
	}

	if( this->textures.empty() )
	{
		tempTexture = cv::Mat(this->size.width, this->size.height, CV_8UC3, cv::Scalar(rand(), 10, rand()));
		this->textures.push_back(tempTexture);
	}

	selectedTexture.first = 0;
	selectedTexture.second = std::make_shared<cv::Mat>(textures.at(0));
}

void Object::UpdateSpeed(int newSpeed)
{
	this->speed = newSpeed;
}

void Object::UpdateNewPosition(cv::Point newPositionCoordinate, cv::Size fieldBoundaries)
{
    // Calculate distances to new coordinate
    int distanceFromTargetX = newPositionCoordinate.x - (this->position.x + this->size.width / 2);
    int distanceFromTargetY = newPositionCoordinate.y - (this->position.y + this->size.height / 2);

    double distanceToTarget = sqrt(distanceFromTargetX * distanceFromTargetX + distanceFromTargetY * distanceFromTargetY);

    // If distance to target is greater than 0, move object
    if (distanceToTarget > 0)
    {
        // Directional calculations to the destination
        double directionX = distanceFromTargetX / distanceToTarget;
        double directionY = distanceFromTargetY / distanceToTarget;

        // Calculate X and Y direction movement amounts
        double deltaX = directionX * this->speed;
        double deltaY = directionY * this->speed;

        // Limit the amount of movement
        if (abs(deltaX) > abs(distanceFromTargetX))
        {
            deltaX = distanceFromTargetX;
        }
        if (abs(deltaY) > abs(distanceFromTargetY))
        {
            deltaY = distanceFromTargetY;
        }

        // Update object coordinate
        this->position.x += deltaX;
        this->position.y += deltaY;

        // Check and fix boundaries
        if (this->position.x < 0)
        {
            this->position.x = 0;
        }
        if (this->position.y < 0)
        {
            this->position.y = 0;
        }
        if (this->position.x + this->size.width > fieldBoundaries.width)
        {
            this->position.x = fieldBoundaries.width - this->size.width;
        }
        if (this->position.y + this->size.height > fieldBoundaries.height)
        {
            this->position.y = fieldBoundaries.height - this->size.height;
        }
    }
}


void Object::collisionEffects()
{

}



