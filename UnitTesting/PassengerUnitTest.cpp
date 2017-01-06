/*
 * PassengerUnitTest.cpp
 *
 *  Created on: Dec 2, 2016
 *      Author: uriah
 */
#include "gtest/gtest.h"
#include "../Common/Passenger.h"
#include <iostream>

using namespace std;

class PassengerUnitTest : public :: testing::Test
{

};

TEST (PassengerUnitTest, construct)
{
	Point src(5,5);
	Point dest(10,2);
	Passenger p(&src, &dest);


	EXPECT_EQ(src, *(p.getSource()));
	EXPECT_EQ(dest, *(p.getDestination()));
}


