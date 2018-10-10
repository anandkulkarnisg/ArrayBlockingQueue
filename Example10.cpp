#include<iostream>
#include<string>
#include<thread>

#include "ArrayBlockingQueue.h"

using namespace std;

ArrayBlockingQueue<int> blockingQueue(10, true);

void takeMethod(const long& waitTime)
{
	this_thread::sleep_for(std::chrono::seconds(waitTime));
	cout << "I am thread id = " << this_thread::get_id() << " queuing for the items" << endl;
	int item = blockingQueue.take();
	cout << "I am currently in thread id = " << this_thread::get_id() << " and the item = " << item << endl;
}

void putMethod(const int& putItem)
{
	cout << "now i am putting an item = " << putItem << endl;
	blockingQueue.put(putItem);
	
	this_thread::sleep_for(std::chrono::seconds(2));
	cout << "now i am putting an item = " << putItem+2 << endl;
	blockingQueue.put(putItem+2);
}

int main(int argc, char* argv[])
{
	// We first invoke two threads with different wait times so that they properly queue to get an item.
	// Then we release one by one item further.	
	
	thread t1(&takeMethod, 5);
	thread t2(&takeMethod, 10);

	// Now we wait for 8 seconds and add items into the queue.
	this_thread::sleep_for(std::chrono::seconds(8));
	thread t3(&putMethod, 25);

	// join the threads.
	t1.join();
	t2.join();
	t3.join();

	// Exit gracefully.
	return(0);	
}
