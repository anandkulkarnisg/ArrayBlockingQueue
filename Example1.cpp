#include<iostream>
#include<string>
#include<vector>

#include "ArrayBlockingQueue.h"

using namespace std;

int main(int argc, char* argv[])
{
  constexpr size_t queueSize=10;
  ArrayBlockingQueue<int> testQueue(queueSize);
  vector<int> vec = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12 };
  bool insertStatus=false;
  for(unsigned int i=0; i<vec.size(); ++i)
  {
    try
    {
      insertStatus=testQueue.add(vec[i]);
      cout << "inserted the item -> " << vec[i] << ", The insertion status = " << insertStatus << endl;
      cout << testQueue.toString() << endl;
    }
    catch(const std::exception& e)
    {
      cout << e.what() << endl;
    }
  }	
  cout << "attempting to reset the array" << endl;
  testQueue.clear();
  cout << "After the clear the status is : " << endl << testQueue.toString() << endl;	
  return(0);
}
