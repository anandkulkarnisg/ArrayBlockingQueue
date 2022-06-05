#include<iostream>
#include<string>
#include<vector>

#include "ArrayBlockingQueue.h"

using namespace std;

int main(int argc, char* argv[])
{

	// Step 1 : define a array blocking queue for size of 10.
	ArrayBlockingQueue<int> blockingQueue(10);

	// Step 2 : Add 10 elements to make the queue full.
	vector<int> vec = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
	for(unsigned int i=0; i<vec.size(); ++i)
		blockingQueue.add(vec[i]);

	// Step 3 : verify the same.
	cout << blockingQueue.toString() << endl;

	// Step 4 : Let us take 3 elements out of the queue.
	int retrieveItem;
	for(int i=0; i<3; ++i)
	{
		retrieveItem=blockingQueue.take();
		cout << "The item retrieved is = " << retrieveItem << endl;
	}
	cout << blockingQueue.toString() << endl;

	// Step 5 : Let us try and stick in 3 items into the queue now.
	for(int i=0; i<3; ++i)
		blockingQueue.add(10*i+1);
	cout << blockingQueue.toString() << endl;

	// Step 6 : Let us try to empty the queue and keep printing the items.
	while(blockingQueue.size()!=0)
		cout << "The retrieved item is = " << blockingQueue.take() << endl;
	cout << blockingQueue.toString() << endl;

	return(0);
}
