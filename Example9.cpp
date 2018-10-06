#include<iostream>
#include<string>
#include<vector>
#include<thread>
#include<cstdlib>
#include<algorithm>

#include "ArrayBlockingQueue.h"

using namespace std;

constexpr int capacity = 1000;
ArrayBlockingQueue<int> blockingQueue(capacity);

void populateQueueRandom(const int& countLimit)
{
	int i=0;
	while(i<countLimit)
	{
		blockingQueue.put(rand()%100);		
		cout << "finished adding an item" << endl;
		this_thread::sleep_for(chrono::milliseconds(100));
		++i;
	}
}

void takeRandomFromQueue(const int& countLimit)
{
	int i=0;
	while(i<countLimit)
	{
		blockingQueue.take();
		cout << "finished taking an item" << endl;
		this_thread::sleep_for(chrono::milliseconds(100));
		++i;
	}
}

int main(int argc, char* argv[])
{
	ArrayBlockingQueue<int> blockingQueue(1000);
	vector<thread> putThreadPool , takeThreadPool;
	putThreadPool.reserve(10);
    takeThreadPool.reserve(10);

	for(unsigned int i=0; i<10; ++i)
		putThreadPool.emplace_back(thread(&populateQueueRandom, 1000));

	for(unsigned int i=0; i<10; ++i)
		takeThreadPool.emplace_back(thread(&takeRandomFromQueue, 1000));
	
	for_each(putThreadPool.begin(), putThreadPool.end(), [&](thread& threadItem) { threadItem.join(); });	
	for_each(takeThreadPool.begin(), takeThreadPool.end(), [&](thread& threadItem) { threadItem.join(); });	
	
	cout << "The finishing queue status is : " << blockingQueue.toString() << endl;
	return(0);

}
