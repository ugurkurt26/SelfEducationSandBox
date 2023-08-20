/*
 * Rabbit.hpp
 *
 *  Created on: Aug 11, 2023
 *      Author: ugur
 */

#ifndef RABBIT_HPP_
#define RABBIT_HPP_

#include "Object.hpp"

class Rabbit : public Object
{
    public:
        void collisionEffects() override;
};

#endif /* RABBIT_HPP_ */
