#include<iostream>
#include<string>
#include<vector>

#include "ArrayBlockingQueue.h"

using namespace std;

ArrayBlockingQueue<int> blockingQueue(10);

int main(int argc, char* argv[])
{
	vector<int> vec = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
	size_t size = vec.size();

	for(unsigned int i=0; i<size; ++i)
		blockingQueue.add(vec[i]);

	cout << blockingQueue.toString() << endl;
	for(int i=size-1; i>=0; --i)
	{
		cout << "The removal status for the item = " << vec[i] << " is = " << blockingQueue.remove(vec[i]) << endl;
		cout << blockingQueue.toString() << endl;
	}

	cout << "Queue empty status = " << (blockingQueue.size()==0) << endl;
	
	// Test Case 2 : We try to create a queue such that it is wrapped around.
	vector<int> vec2 = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
	size = vec2.size();
	
	for(unsigned int i=0; i<size; ++i)
		blockingQueue.add(vec2[i]);
		
	cout << blockingQueue.toString() << endl; 
	for(unsigned int i=0; i<=5; ++i)
		cout << blockingQueue.take() << endl;
	
	cout << blockingQueue.toString() << endl;	

	// Let us insert some items into the queue.
	for(unsigned int i=11; i<=16;++i)
		blockingQueue.add(i);

	cout << blockingQueue.toString() << endl;
	// Now try and remove random elements from the queue. items 8 and 12 are removed.
	cout << "removal status for item = 8 is = " << blockingQueue.remove(8) << endl;
	cout << blockingQueue.toString() << endl;

	cout << "removal status for item = 12 is = " << blockingQueue.remove(12) << endl;
	cout << blockingQueue.toString() << endl;

	
	return(0);	
}

