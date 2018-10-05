#include<iostream>
#include<string>

#include "ArrayBlockingQueue.h"

using namespace std;

int main(int argc, char* argv[])
{
	vector<int> vec = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
	size_t size = vec.size();
	
	// Step 1 : have a queue setup and get an Iterator to it, then simply display the elements.
	
	ArrayBlockingQueue<int> blockingQueue(size);
	for(unsigned int i=0; i<size; ++i)
		blockingQueue.add(vec[i]);
		
	cout << blockingQueue.toString() << endl;	
	Iterator<int> queueIterator = blockingQueue.iterator();

	while(queueIterator.hasNext())
		cout << queueIterator.getNext() << ",";
	cout << '\n';

	// Step 2 : Let us make the queue wrap around. Let us first try to remove first 6 elements.
	for(unsigned int i=0; i<5; ++i)
		cout << "popped out item = " << blockingQueue.take() << endl;
	
	for(unsigned int i=1; i<=5; ++i)
	{
		cout << "Adding item = " << 10*i << endl;
		blockingQueue.add(10*i);
	}
	cout << blockingQueue.toString() << endl;

	// Step 3 : Let us take a new iterator and print the elements.
	Iterator<int> iter = blockingQueue.iterator();
	while(iter.hasNext())
		cout << iter.getNext() << ", " << endl;

	// Question : If somebody used iterator in threaded env , how will they ensure data integrity ?

	return(0);
}
