/*
 * LogicUnitTest.cpp
 *
 *  Created on: Dec 2, 2016
 *      Author: uriah
 */


#include "gtest/gtest.h"
#include "../ServerSide/Logic.h"
#include <iostream>

using namespace std;

class LogicUnitTest : public :: testing::Test {};

/*
 * Tests:
 * The constructor will define a valid
 * grid and a valid taxicenter.
 */
TEST (LogicUnitTest, construct)
{
	Logic logic;
	EXPECT_EQ(NULL,	logic.getCenter());
	EXPECT_EQ(NULL, logic.getGrid());
}
