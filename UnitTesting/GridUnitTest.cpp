//============================================================================
// Name        : GridUnitTest.cpp
// Author      : Alon Aviv Uriah Ahrak
// Description : Unit test for Grid object.
//				 Validate that the value of node in the place of (x,y) is (x,y)
//				 Validate that by default Grid is created with 10x10
//				 Validate that Grid throws an exception in case of a bad initialization
//				 Validate neighbors list is empty in case of a node surrounded by not available nodes
//						and that neighbors list does not contain more than 2 neighbors for corner nodes.
// 				 Validate that Grid throws an exception in a bad query to getNodeAt(Point)
//============================================================================
#include "../Common/Point.h"
#include "../Common/Grid.h"
#include "../Common/Node.h"
#include "gtest/gtest.h"

using namespace std;
class GridUnitTest : public :: testing::Test{
public:
	Grid* map;
	int height;
	int width;

	/*
	 * Test constructor.
	 * Creates a grid with x,y margins.
	 */
	GridUnitTest(int x, int y){
		height = y;
		width = x;
		SetUp();
	}

protected:
	virtual void SetUp(){
		map = new Grid(width,height);
	}
	virtual void TestBody(){ }
	virtual void TearDown(){
		delete map;
	}
};

/*
 * Assert the simple initialization of the Grid.
 * Checks if the value at LOCATION (x,y) is (x,y).
 */
TEST(GridUnitTest, ValidateInitValues){
	GridUnitTest test(4,4);

	/*
	 * Tests the default initialization - private method initGrid() and
	 * public method getNodeAt(Point&)
	 */
	Point point(3,3);
	Searchable* searchable = test.map->getNodeAt(point);
	ASSERT_EQ(searchable->getValue(), point);
}

/*
 * Tests default constructor and 10x10 default size.
 */
TEST(GridUnitTesting, ValidateInitSizes){
	Grid map;
	ASSERT_EQ(map.getHeight(),10);
	ASSERT_EQ(map.getWidth(), 10);
}

/*
 * Verifies that map is 0 if it was initialized with negative
 * numbers.
 */
TEST(GridUnitTest, BadInitTest){
	//Tests that constructor returns null in the case of bad margins
	try {
		GridUnitTest test(-1,-1);
		FAIL() << "Expected: std::invalid_argument";
	}
	catch (invalid_argument const& err) {
		ASSERT_EQ(0,0);
	}
	catch(...){
		FAIL() << "Expected: std::invalid_argument";
	}
}

/*
 * Tests getNeighbors using two scenarios.
 * In case a node has no neighbors (i.e. they are not available).
 * In a regular case.
 * Validate that corener has no more than 2 neighbors.
 */
TEST(GridUnitTest,ValidateNeighbors){
	GridUnitTest test(2,2);

	Point corner(0,0);
	Point up(0,1);
	Point right(1,0);

	Searchable* cornerNode = test.map->getNodeAt(corner);
	//Surrounds (0,0) and assert that it has no root to anywhere
	Searchable* upNode = test.map->getNodeAt(up);
	Searchable* rightNode =  test.map->getNodeAt(right);
	upNode->setAvailability(false);
	rightNode->setAvailability(false);

	vector<Searchable*> neighbors = test.map->getNodesNeighbors(cornerNode);
	int count=0;
	for(vector<Searchable*>::iterator it = neighbors.begin(); it != neighbors.end(); ++it){
		count++;
	}
	EXPECT_EQ(count,0);

	Point upperRight(test.map->getWidth()-1,test.map->getHeight()-1);
	Searchable* upperRightNode = test.map->getNodeAt(upperRight);
	vector<Searchable*> cornerNeighbors = test.map->getNodesNeighbors(upperRightNode);

	count=0;
	for(vector<Searchable*>::iterator it = neighbors.begin(); it != neighbors.end(); ++it){
		count++;
	}
	EXPECT_LE(count,2);
}

/*
 * Grid should throw an exception for an invalid input
 * as a point to search.
 */
TEST(GridUnitTest, BadGetNodeQuery){
	GridUnitTest test(2,2);
	Point badLocation(-1,-1);
	try{
		test.map->getNodeAt(badLocation);
		FAIL () << "Bad query should throw an exception";
	}
	catch(invalid_argument const& err){
		ASSERT_EQ(0,0);
	}
	catch(...){
		FAIL () << "Bad query should throw an exception";
	}
}
