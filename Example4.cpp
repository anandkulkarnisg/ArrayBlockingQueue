#include<iostream>
#include<string>
#include<thread>

#include "ArrayBlockingQueue.h"

using namespace std;

ArrayBlockingQueue<int> blockingQueue(100);

void addItemsToQueue(const int& seedItem)
{
	int i=seedItem;
	while(i<100)
	{
		blockingQueue.add(i);
		i+=2;
	}
}

int main(int argc, char* argv[])
{
	// Create two threads that will push even and odd elements into the blocking queue.
	thread evenThread(&addItemsToQueue, 0);
	thread oddThread(&addItemsToQueue, 1);

	// Join the threads.
	evenThread.join();
	oddThread.join();

	// Display the blocking queue status. Must contain all the items without loss.
	cout << blockingQueue.toString() << endl;

	// Drain the items of the queue to a vector. Then display the items.
	vector<int> vec;
	blockingQueue.drainTo(vec);

	// The vector items are displayed below.
	for(const auto& iter : vec)
		cout << iter << " , ";
	cout << '\n';

	// Test if the blocking queue is empty now.
	cout << blockingQueue.toString() << endl;
	return(0);
}
