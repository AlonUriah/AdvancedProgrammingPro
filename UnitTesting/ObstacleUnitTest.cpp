/*
 * ObstacleUnitTest.cpp
 *
 *  Created on: Dec 2, 2016
 *      Author: uriah
 */


#include "gtest/gtest.h"
#include "../Common/Obstacle.h"
#include <iostream>

using namespace std;

class ObstacleUnitTest : public :: testing::Test
{

};

/*
 * Tests:
 * Constructor which is v
 */
TEST (ObstacleUnitTest, construct)
{
	Point p(5,5);
	Obstacle obs(&p, "Gym");
	EXPECT_EQ("Gym", obs.getDescription());
}


