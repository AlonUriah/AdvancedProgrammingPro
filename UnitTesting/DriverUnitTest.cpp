/*
 * DriverUnitTest.cpp
 *
 *  Created on: Dec 2, 2016
 *      Author: uriah
 */


#include "gtest/gtest.h"
#include "../Common/Driver.h"
#include "../Common/Common.h"
#include <iostream>

using namespace std;

class DriverUnitTest : public :: testing::Test
{

};

TEST (DriverUnitTest, construct)
{
	Driver d;
	EXPECT_GT(0, d.getId());
	EXPECT_GT(17.0, d.getAge());
	EXPECT_LT(120.0, d.getAge());
	EXPECT_NE(NULL, d.getStatus());
	EXPECT_GT(0, d.getXp());
	//EXPECT_GT(0, d.getSatisfaction());

	Driver g(2222244, 25.5, SINGLE, 5, 0);
	EXPECT_EQ(2222244, d.getId());
	EXPECT_EQ(25.5, d.getAge());
	EXPECT_EQ(SINGLE, d.getStatus());
	EXPECT_EQ(5, d.getXp());
	//EXPECT_EQ(0, d.getSatisfaction());

	//d.setStatus(SINGLE);
	EXPECT_EQ(SINGLE, d.getStatus());

	int xp = 52;
	//d.setExperience(xp);
	EXPECT_EQ(xp, d.getXp());

	double sat = 28.8;
	//d.setSatisfaction(sat);
	//EXPECT_EQ(sat, d.getSatisfaction());
}
