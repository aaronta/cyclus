// CycArithmeticTests.cpp
#include <map>
#include <gtest/gtest.h>

#include "CycException.h"
#include "CycArithmetic.h"

using namespace std;

//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -    
class CycArithmeticTest : public ::testing::Test {
  protected:
    vector<double> empty_vec_;
    vector<double> extreme_vec_;
    vector<double> ten_to_one_vec_;
    vector<double> zeros_vec_;
    vector<double> ones_vec_;
    map<int, double> empty_map_;
    map<int, double> zeros_map_;
    map<int, double> ten_to_one_map_;
    map<int, double> extreme_map_;
    double ten_to_one_sum_;

    virtual void SetUp(){
      ten_to_one_sum_=0.0;
      for(int i=0; i<10; ++i){
        ones_vec_.push_back(1);
        zeros_vec_.push_back(0);
        zeros_map_.insert(make_pair(i,0));
        ten_to_one_vec_.push_back(10-i);
        ten_to_one_map_.insert(make_pair(10-i,10-i));
        ten_to_one_sum_+=10-i;
      }
      extreme_vec_.push_back(1e16);
      extreme_vec_.push_back(1e-16);
      extreme_vec_.push_back(1);
    }
    virtual void TearDown() {
    }
};

//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -    
TEST_F(CycArithmeticTest, KahanSumZero){
  EXPECT_FLOAT_EQ(0, CycArithmetic::KahanSum(zeros_vec_));
}

//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -    
TEST_F(CycArithmeticTest, KahanSumEmpty){
  EXPECT_FLOAT_EQ(0, CycArithmetic::KahanSum(empty_vec_));
}

//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -    
TEST_F(CycArithmeticTest, KahanSumOrdinary){
  EXPECT_FLOAT_EQ(10, CycArithmetic::KahanSum(ones_vec_));
}

//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -    
TEST_F(CycArithmeticTest, KahanSumExtremeVals){
  EXPECT_FLOAT_EQ(1e16+1e-16+1, CycArithmetic::KahanSum(extreme_vec_));
}

//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -    
TEST_F(CycArithmeticTest, sortAscendingVecEmpty) {
  vector<double> sorted;
  EXPECT_NO_THROW(sorted = CycArithmetic::sort_ascending(empty_vec_));
  EXPECT_TRUE(sorted.empty());
}

//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -    
TEST_F(CycArithmeticTest, sortAscendingMapEmpty) {
  vector<double> sorted;
  EXPECT_NO_THROW(sorted = CycArithmetic::sort_ascending(empty_map_));
  EXPECT_TRUE(sorted.empty());
}

//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -    
TEST_F(CycArithmeticTest, sortAscendingVecZeros) {
  vector<double> sorted;
  EXPECT_NO_THROW(sorted = CycArithmetic::sort_ascending(zeros_vec_));
  vector<double>::iterator it;
  EXPECT_FALSE(sorted.empty());
  for(it=sorted.begin(); it!=sorted.end(); ++it) { 
    EXPECT_FLOAT_EQ(0, *it);
  }
}

//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -    
TEST_F(CycArithmeticTest, sortAscendingMapZeros) {
  vector<double> sorted;
  EXPECT_NO_THROW(sorted = CycArithmetic::sort_ascending(zeros_map_));
  vector<double>::iterator it;
  EXPECT_FALSE(sorted.empty());
  for(it=sorted.begin(); it!=sorted.end(); ++it) { 
    EXPECT_FLOAT_EQ(0, *it);
  }
}

//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -    
TEST_F(CycArithmeticTest, sortAscendingVec) {
  vector<double> sorted;
  EXPECT_NO_THROW(sorted = CycArithmetic::sort_ascending(ten_to_one_vec_));
  vector<double>::iterator it;
  EXPECT_FALSE(sorted.empty());
  int i=1;
  for(it=sorted.begin(); it!=sorted.end(); ++it) { 
    EXPECT_FLOAT_EQ(i, *it);
    i++;
  }
}

//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -    
TEST_F(CycArithmeticTest, sortAscendingMap) {
  vector<double> sorted;
  EXPECT_NO_THROW(sorted = CycArithmetic::sort_ascending(ten_to_one_map_));
  vector<double>::iterator it;
  EXPECT_FALSE(sorted.empty());
  int i = 1;
  for(it=sorted.begin(); it!=sorted.end(); ++it) { 
    EXPECT_FLOAT_EQ(i, *it);
    i++;
  }
}

