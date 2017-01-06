/*
 * LoggerUnitTest.cpp
 *
 *  Created on: Dec 2, 2016
 *      Author: uriah
 */


#include "gtest/gtest.h"
#include "../Common/Logger.h"
#include <iostream>
#include <cstdio>
#include <vector>

using namespace std;

class LoggerUnitTest : public :: testing::Test
{

};

TEST (LoggerUnitTest, SaveLog)
{
	Logger logger;
	logger.SaveLog();
	FILE* f= fopen("test.txt", "r");
	ASSERT_EQ(NULL, f);

	f = fopen("test.txt", "w");
	fputs("Hello", f);
	fclose(f);
}
TEST (LoggerUnitTest, OpenLog)
{
	Logger logger;
	vector<char*>* s = logger.OpenLog("test.txt");
	EXPECT_EQ(NULL, s);
	remove("text.txt");
}
