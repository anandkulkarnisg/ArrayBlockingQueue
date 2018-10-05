#include<iostream>
#include<string>
#include<thread>
#include<algorithm>

#include "ArrayBlockingQueue.h"

using namespace std;

ArrayBlockingQueue<int> blockingQueue(10);

string getThreadId()
{
    auto myid = this_thread::get_id();
    stringstream ss;
    ss << myid;
    string resultString = ss.str();
    return(resultString);
}

void takeAndDisplayQueueItem()
{
	try
	{
		int item = blockingQueue.take();
		cout << "The item value is = " << item << endl;
	}
	catch(const exception& e)
	{
		cout << e.what() << endl;
	}
}

void putItemsIntoQueue(const int& delayInSecs)
{
	string threadId = getThreadId();
	cout << "I am in thread id = " << threadId << " sleeping for " << delayInSecs << " seconds." << endl; 
	this_thread::sleep_for(chrono::seconds(delayInSecs));
	blockingQueue.put(10*delayInSecs);
	cout << "I am in thread id = " << threadId << " finished putting item = " << 10*delayInSecs << endl; 
}

int main(int argc, char* argv[])
{
	vector<thread> takeThreadPool;
	takeThreadPool.reserve(5);

	for(unsigned int i=0; i<5; ++i)
		takeThreadPool.emplace_back(thread(&takeAndDisplayQueueItem));

	cout << "The main thread id = " << getThreadId() << "is waiting for 5 secs to proceed" << endl;
	this_thread::sleep_for(chrono::seconds(5));
	cout << "main thread is proceeding now" << endl;

	vector<thread> putThreadPool;
	putThreadPool.reserve(5);

	for(unsigned int i=1; i<=5; ++i)
		putThreadPool.emplace_back(thread(&putItemsIntoQueue, i+5));
	
	for_each(takeThreadPool.begin(), takeThreadPool.end(), [&](thread& threadItem) { threadItem.join(); });	
	for_each(putThreadPool.begin(), putThreadPool.end(), [&](thread& threadItem) { threadItem.join(); });

	return(0);

}
