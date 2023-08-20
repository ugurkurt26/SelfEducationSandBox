/*
 * Carrot.hpp
 *
 *  Created on: Aug 11, 2023
 *      Author: ugur
 */

#ifndef CARROT_HPP_
#define CARROT_HPP_

#include "Object.hpp"

#include <boost/random/random_device.hpp>
#include <boost/random/uniform_int_distribution.hpp>

class Carrot : public Object
{
    public:
        Carrot();
        void UpdateNewPosition(cv::Point dropCoordinate, cv::Size fieldBoundaries) override;
        void UpdateNewPosition(cv::Size fieldBoundaries, cv::Rect avoidedArea);
        void collisionEffects() override;

        double angle;

    private:
        boost::random::random_device rng;
};

#endif /* CARROT_HPP_ */
