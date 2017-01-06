//============================================================================
// Name        : SearchableUnitTest.cpp
// Author      : Alon Aviv Uriah Ahrak
// Description : Unit test for Searchable object.
//				 PLEASE NOTICE THAT NODE IS THE SAME AT THE MOMENT.
//
//============================================================================
#include "../SearchAlgo/Searchable.h"
#include "../Common/Point.h"
#include "gtest/gtest.h"
#include <ostream>

using namespace std;
class SearchableUnitTest : public :: testing::Test{
public:
	Point value;
	int distance;
	Searchable* parent;

	/*
	 * Default constructor of this TEST.
	 */
	SearchableUnitTest(){
		SetUp();
	}

protected:
	virtual void SetUp(){
		value = Point(0,0);
		distance = -1;
		parent = 0;
	}
	virtual void TestBody(){ }
	virtual void TearDown(){ }
};

/*
 * Validates Searchable constructors.
 */
TEST(SearchableUnitTest, ValidateValue){
	SearchableUnitTest test;

	//Test default constructor, default value is (0,0)
	Searchable s1;
	Point s1Value = s1.getValue();
	EXPECT_EQ(test.value,s1Value);

	Searchable s2(test.value);
	Point s2Value = s2.getValue();
	EXPECT_EQ(test.value,s2Value);
}

/*
 * Validates distance setter/getter.
 */
TEST(SearchableUnitTest, ValidateDistance){
	SearchableUnitTest test;

	//Test default constructor, default value is (0,0)
	Searchable s1;
	EXPECT_EQ(s1.getDistance(),-1);

	Searchable s2(test.value);
	s2.setDistance(s1.getDistance()+1);
	s1.setDistance(0);
	EXPECT_EQ(s1.getDistance(), s2.getDistance());
}

/*
 * Check parent setter/getter for a valid input.
 * Verifies that Searchable throw an exception when a searchable is trying
 * to assign  itself as its own parent.
 */
TEST(SearchableUnitTest, ValidateParent){
	SearchableUnitTest test;
	//Valid input
	Searchable rootNode(test.value);

	Point up(0,1);
	Searchable* s1 = new Searchable(up);
	rootNode.setParent(s1);

	EXPECT_EQ(*s1,*(rootNode.getParent()));

	//Invalid input
	try{
		s1->setParent(s1);
		FAIL() << "Infinite loop by bad parent assigning";
	}
	catch(invalid_argument const& err){
		ASSERT_EQ(0,0);
	}
	catch(...){
		FAIL() << "Infinite loop by bad parent assigning";
	}
	delete s1;
}

/*
 * Validates that searchable are created available by default.
 * Validates that searchable availability setter works.
 */
TEST(SearchableUnitTest, ValidateAvailability){
	SearchableUnitTest test;
	Searchable s1(test.value);

	ASSERT_EQ(s1.isAvailable(), true);

	s1.setAvailability(false);
	ASSERT_NE(s1.isAvailable(), true);
}
