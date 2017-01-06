/*
 * TaxiUnitTest.cpp
 *
 *  Created on: Dec 2, 2016
 *      Author: uriah
 */


#include "gtest/gtest.h"
#include "../Common/Common.h"
#include "../Common/Taxi.h"
#include <iostream>

using namespace std;

class TaxiUnitTest : public :: testing::Test
{

};

TEST (TaxiUnitTest, construct)
{
	Taxi taxi(26, 'H', 'B');

	EXPECT_EQ(26, taxi.getKm());

	taxi.setKm(45);
	EXPECT_EQ(45, taxi.getKm());
}
