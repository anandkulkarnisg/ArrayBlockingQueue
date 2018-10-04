#include<iostream>
#include<string>
#include<vector>

#include "ArrayBlockingQueue.h"

using namespace std;

int main(int argc, char* argv[])
{
	size_t queueSize=10;
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
	// Let us try to dequeue the items from the queue.
	size_t i=0;
	while(i<vec.size()-2)
	{
		int queueItem=testQueue.take();
		cout << "The item popped out is = " << queueItem << testQueue.toString() << endl;
		++i;
	}	

	// Verify that the queue is empty now.
	cout << "The queue empty status = " << (testQueue.size()==0) << endl;

	// Attempt to take polling with timeout and see if we fail to get a result.
	pair<bool,int> returnItem = testQueue.poll(10, TimeUnit::Seconds);
	cout << "The returnItem status is : " << returnItem.first << ", " << returnItem.second << endl; 
	return(0);
}
