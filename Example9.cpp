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
    this_thread::sleep_for(chrono::milliseconds(2));
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
    this_thread::sleep_for(chrono::milliseconds(2));
    ++i;
  }
}

int main(int argc, char* argv[])
{
  vector<thread> putThreadPool , takeThreadPool;
  constexpr size_t threadPoolSize=10;
  const int randomLimit=10000;
  putThreadPool.reserve(threadPoolSize);
  takeThreadPool.reserve(threadPoolSize);

  for(unsigned int i=0; i<threadPoolSize; ++i)
    putThreadPool.emplace_back(thread(&populateQueueRandom, randomLimit));

  for(unsigned int i=0; i<threadPoolSize; ++i)
    takeThreadPool.emplace_back(thread(&takeRandomFromQueue, randomLimit));

  for_each(putThreadPool.begin(), putThreadPool.end(), [&](thread& threadItem) { threadItem.join(); });	
  for_each(takeThreadPool.begin(), takeThreadPool.end(), [&](thread& threadItem) { threadItem.join(); });	

  cout << "The finishing queue status is : " << blockingQueue.toString() << endl;
  return(0);

}

