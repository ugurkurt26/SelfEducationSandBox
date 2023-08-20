/*
 * MagicMushroom.hpp
 *
 *  Created on: Aug 11, 2023
 *      Author: ugur
 */

#ifndef MAGICMUSHROOM_HPP_
#define MAGICMUSHROOM_HPP_

#include "Object.hpp"

class MagicMushroom : public Object
{
    public:
        void collisionEffects() override;
        void collisionEffects(Object& targetObject, cv::Point& manipulatedPoint, cv::Point centerPoint, int effectNumber);

        std::chrono::_V2::system_clock::time_point spawnTime;
		std::chrono::seconds elapsedSinceSpawn;

        std::chrono::_V2::system_clock::time_point effectStartTime;
		std::chrono::seconds elapsedSinceEffectStart;

    private:
        void disorientateEffect(cv::Point& manipulatedPoint, cv::Point centerPoint);
        void speedyGonzalesEffect(Object& targetObject);
};

#endif /* MAGICMUSHROOM_HPP_ */
