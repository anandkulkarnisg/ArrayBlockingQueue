#include<iostream>
#include<string>

#include "ArrayBlockingQueue.h"

using namespace std;

int main(int argc, char* argv[])
{
	vector<int> vec = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
	size_t size = vec.size();
		
	ArrayBlockingQueue<int> blockingQueue(size);
	for(unsigned int i=0; i<size; ++i)
		blockingQueue.add(vec[i]);
		
	cout << blockingQueue.toString() << endl;	
	Iterator<int> queueIterator = blockingQueue.iterator();

	while(queueIterator.hasNext())
		cout << queueIterator.getNext() << ",";
	cout << '\n';
	return(0);
}
