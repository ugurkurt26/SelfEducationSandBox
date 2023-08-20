/*
 * Carrot.cpp
 *
 *  Created on: Aug 11, 2023
 *      Author: ugur
 */

#include "Carrot.hpp"

Carrot::Carrot()
{
    this->angle = 45;
}

void Carrot::collisionEffects()
{
}

void Carrot::UpdateNewPosition(cv::Point dropCoordinate, cv::Size fieldBoundaries)
{
    // Açısal değişimi hesapla
    double radians = angle * CV_PI / 180.0;
    position.x += speed * std::cos(radians);
    position.y += speed * std::sin(radians);

    // Kenarlara çarpma kontrolü
    if (position.x <= 0 || position.x >= fieldBoundaries.width)
    {
        angle = 180.0 - angle; // Yansıma açısını değiştir
    }

    if (position.y <= 0 || position.y >= fieldBoundaries.height)
    {
        angle = -angle; // Yansıma açısını değiştir
    }
}

void Carrot::UpdateNewPosition(cv::Size fieldBoundaries, cv::Rect avoidedArea)
{
    boost::random::uniform_int_distribution<> randomCoordinateX(0, fieldBoundaries.width - this->size.width);
    int dropPointX = 0;
    do
    {
        dropPointX = randomCoordinateX(rng);

    } while (dropPointX >= avoidedArea.x && dropPointX <= avoidedArea.x);

    boost::random::uniform_int_distribution<> randomCoordinateY(0, fieldBoundaries.height - this->size.height);
    int dropPointY = 0;
    {
        dropPointY = randomCoordinateY(rng);
    }
    while (dropPointY >= avoidedArea.y && dropPointY <= avoidedArea.y);

    this->position = cv::Point(dropPointX, dropPointY);
}
