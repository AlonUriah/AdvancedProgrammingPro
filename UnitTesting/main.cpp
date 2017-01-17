/*
 * TestingMain.cpp
 *
 *  Created on: Jan 16, 2017
 *      Author: alon
 */
#include "gtest/gtest.h"
#include <iostream>

#include "../Common/Factories/GridFactory.h"
#include "../Common/Grid.h"
#include "../SearchAlgo/Searchable.h"
#include "../Common/Point.h"
#include "../SearchAlgo/BFS.h"
#include <stack>

using namespace std;

int main(int argc, char** argv){
	const int HEIGHT = 2000;
	const int WIDTH = 2000;

	GridFactory* factory = new GridFactory(HEIGHT,WIDTH);
	Grid* grid = factory->create();

	Point s_p(0,0);
	Point e_p(HEIGHT/2,WIDTH/2);

	Searchable* s = grid->getNodeAt(s_p);
	Searchable* e = grid->getNodeAt(e_p);

	stack<Searchable*> root = BFS::getRoot(*grid,s,e);

	delete grid;
	delete factory;


	/*::testing::InitGoogleTest(&argc, argv);
	RUN_ALL_TESTS();
	cout << "FINISHED";*/
	return 0;
}



