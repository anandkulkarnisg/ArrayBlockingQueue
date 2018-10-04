#include<iostream>
#include<string>

#include "ArrayBlockingQueue.h"

using namespace std;

int main(int argc, char* argv[])
{
	size_t size=10;
	vector<int> vec;
	ArrayBlockingQueue<int> testQueue(size);
	for(unsigned int i=0; i<size; ++i)
		testQueue.add(10*i);
	cout << testQueue.toString() << endl;
	testQueue.drainTo(vec);
	cout << testQueue.toString() << endl;
	cout << "The contents of the drained vec are" << endl;
	for(const auto& iter : vec)
		cout << iter << ",";
	cout << '\n';
	cout << "Again adding the 10 items to the queue" << endl;
    for(unsigned int i=0; i<size; ++i)
       testQueue.add(10*i);
	cout << testQueue.toString() << endl;
	vector<int> vec2;
	testQueue.drainTo(vec2, 5);
	cout << "After draining the 5 items" << endl;
	cout << testQueue.toString() << endl;
	for(const auto& iter : vec2)
		cout << iter << ",";
	cout << '\n';
	return(0);		



}
