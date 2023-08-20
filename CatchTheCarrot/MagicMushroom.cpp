/*
 * MagicMushroom.cpp
 *
 *  Created on: Aug 11, 2023
 *      Author: ugur
 */

#include "MagicMushroom.hpp"

void MagicMushroom::collisionEffects()
{
    
}

void MagicMushroom::collisionEffects(Object& targetObject, cv::Point& manipulatedPoint, cv::Point centerPoint, int effectNumber)
{
    if(effectNumber == 0)
    {
        disorientateEffect(manipulatedPoint, centerPoint);
    }
    else if(effectNumber == 1)
    {
        speedyGonzalesEffect(targetObject);
    }
    else
    {
        disorientateEffect(manipulatedPoint, centerPoint);
        speedyGonzalesEffect(targetObject);
    }
}

void MagicMushroom::disorientateEffect(cv::Point& manipulatedPoint, cv::Point centerPoint)
{
    manipulatedPoint.x = 2 * centerPoint.x - manipulatedPoint.x;
    manipulatedPoint.y = 2 * centerPoint.y - manipulatedPoint.y;
}

void MagicMushroom::speedyGonzalesEffect(Object& targetObject)
{
    std::cout << "speedyGonzalesEffect" << std::endl;
}
    