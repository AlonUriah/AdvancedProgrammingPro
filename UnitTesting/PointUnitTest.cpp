//============================================================================
// Name        : PointUnitTest.cpp
// Author      : Alon Aviv Uriah Ahrak
// Description : Unit test for Point object.
//				 Check all of Point constructors (default, explicit and copy).
//				 Check Point setters and getters.
//============================================================================
#include "../Common/Point.h"
#include "gtest/gtest.h"

using namespace std;
class PointUnitTest : public :: testing::Test{
public:
	int _x;
	int _y;

	/*
	 * Constructor.
	 */
	PointUnitTest(int x, int y){
		_x = x;
		_y = y;
		SetUp();
	}

protected:
	virtual void SetUp(){ }
	virtual void TestBody(){ }
	virtual void TearDown(){ }
};

/*
 * Validate all of Point's constructors.
 * Tests the default initialization to (0,0),
 * the copy constructor and the explicit initialization.
 */
TEST(PointUnitTest, ValidatePointInit){
	PointUnitTest test(0,0);
	Point defaultPoint;

	Point initPoint(0,0);
	ASSERT_EQ(defaultPoint,initPoint);

	Point copiedPoint(initPoint);
	ASSERT_EQ(copiedPoint, initPoint);
}

/*
 * Tests Point arguments getters and setters.
 */
TEST(PointUnitTest, ValidatePointAssignment){
	PointUnitTest test(5,7);
	Point point(test._x, test._y);
	point.setX(point.getX() + test._y);
	point.setY(point.getX() - point.getY());
	point.setX(point.getX() - point.getY());

	Point expected(7,5);
	ASSERT_EQ(point, expected);
}
