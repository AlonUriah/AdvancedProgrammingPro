//============================================================================
// Name        : BFSUnitTest.cpp
// Author      : Alon Aviv Uriah Ahrak
// Description : Unit test for BFS class.
//				 Validate with a simple query of (0,0)->(1,1).
//				 Validate that there is no root (empty one) to a nonexisting node.
//				 Validate that each node on the root is available and that there is no root
//				 		if one searchable node is surrounded by not-available nodes.
//============================================================================

#include "../SearchAlgo/BFS.h"
#include "../SearchAlgo/Searchable.h"
#include "../Common/Point.h"
#include "../Common/Grid.h"
#include <stack>
#include "gtest/gtest.h"

using namespace std;
class BFSUnitTest : public :: testing::Test{
public:
	Grid* map;
	Point startP;
	Point destP;

	/*
	 * Constructor.
	 */
	BFSUnitTest(){
		SetUp();
	}

protected:
	virtual void SetUp(){
		map = new Grid(2,2);
		startP = Point(0,0);
		destP = Point(1,1);
	}
	virtual void TestBody(){ }
	virtual void TearDown(){
		delete map;
	}
};

/*
 * Assert the simple case of the expected root
 * between (0,0) to (1,1).
 */
TEST(BFSUnitTest, ValidateExpectedResult){
	BFSUnitTest test;
	Searchable* startNode = test.map->getNodeAt(test.startP);
	Searchable* endNode = test.map->getNodeAt(test.destP);
	stack<Searchable*> root = BFS::getRoot(*test.map,startNode,endNode);

	Point pArray[3];
	pArray[0] = Point(0,0);
	pArray[1] = Point(0,1);
	pArray[2] = Point(1,1);

	for(int i=0; i<3; i++){
		ASSERT_EQ(root.top()->getValue(),pArray[i]);
		root.pop();
	}
}

/*
 * Validates the there is no root to
 * a non existing node.
 */
TEST(BFSUnitTest, NonExistingNodeSearch){
	BFSUnitTest test;
	Point startP(0,0);
	Searchable* startNode = test.map->getNodeAt(startP);
	Point nonExistP(-1,-1);
	Searchable* nonExistNode = new Searchable(nonExistP);

	stack<Searchable*> root = BFS::getRoot(*test.map,startNode,nonExistNode);
	int count=0;
	while(!root.empty()){
		count++;
		root.pop();
	}
	delete nonExistNode;
	ASSERT_EQ(count,0);
}

/*
 * Validate that BFS algorithm does not go through
 * obstacles.
 */
TEST(BFSUnitTest, AllThroughableNodes){
	BFSUnitTest test;
	Searchable* startNode = test.map->getNodeAt(test.startP);
	Searchable* endNode = test.map->getNodeAt(test.destP);
	//Naive test - check that for some root all of the nodes are available
	stack<Searchable*> root = BFS::getRoot(*test.map,startNode,endNode);
	while(!root.empty())
	{
		ASSERT_EQ(root.top()->isAvailable(),true);
		root.pop();
	}

	//Surrounds (0,0) and assert that it has no root to anywhere
	Point up(0,1);
	Point right(1,0);
	Searchable* upNode = test.map->getNodeAt(up);
	Searchable* rightNode =  test.map->getNodeAt(right);
	upNode->setAvailability(false);
	rightNode->setAvailability(false);
	root = BFS::getRoot(*test.map,startNode,endNode);
	int count=0;
	while(!root.empty()){
		count++;
		root.pop();
	}
	ASSERT_EQ(count,0);
}

