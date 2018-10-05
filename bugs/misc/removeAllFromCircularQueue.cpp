#include<iostream>
#include<string>
#include<vector>

using namespace std;

int* array = new int[10];
int frontIdx=-1;
int rearIdx=-1;
int CAPACITY=10;
int SIZE=0;

void enqueue(const int& item)
{
	if(CAPACITY==SIZE)
	{
		cout << "queue is full" << endl;
		return;
	}

	// Is this the first item in the queue.
	if(SIZE==0)
	{
		frontIdx=0;
		rearIdx=0;
		cout << "special inserting at position = " << frontIdx << endl;
		array[frontIdx]=item;
	}
	else
	{
		int insertIdx=(rearIdx+1)%CAPACITY;
		array[insertIdx]=item;
		cout << "inserting at position = " << insertIdx << endl;
		rearIdx=insertIdx;
	}
	++SIZE;		
}

int dequeue()
{
	int returnItem;
	if(SIZE==0)
	{
		cout << "Error : The queue is empty. Nothing to dequeue!" << endl;
		return(-1);
	}

	// Special case only 1 item present in the queue.
	if(SIZE==1 && frontIdx==rearIdx)
	{	
		returnItem=array[frontIdx];
		frontIdx=rearIdx=-1;
	}
	else
	{
		// Rest of all the cases simply return the item at frontIdx and increment it.
		returnItem=array[frontIdx];
		frontIdx=(frontIdx+1)%CAPACITY;
	}
	--SIZE;
	return(returnItem);
}

void showQueueItems()
{
	if(SIZE==0)
		return;

	if(rearIdx>=frontIdx)
	{
		for(int i=frontIdx; i<rearIdx; ++i)
			cout << array[i] << " , ";
		cout << array[rearIdx] << '\n';
	}
	else
	{
		for(int i=frontIdx; i<CAPACITY;++i)
			cout << array[i] << " , ";
		for(int i=0; i<rearIdx; ++i)
			cout << array[i] << ", ";
		cout << array[rearIdx] << '\n';
	}
}

bool removeAll(const int& item)
{
	// remove all instances of the item present in the queue. i.e the first occurance if any found. 
	// return true is modification was done else false.

	// case 1 : empty queue. do nothing.
	if(SIZE==0)
		return(false);

	// case 2 : one item and is same as the item to be removed. simply reset the queue and return true.
	if(SIZE==1&&array[frontIdx]==item)
	{
		frontIdx=rearIdx=-1;
		SIZE=0;
		return(true);
	}

	// case 3 : The queue has not wrapped around end , i.e rearIdx>=frontIdx.
	if(rearIdx>frontIdx)
	{
		int j=frontIdx;
		int skipCount=0;
		for(int i=frontIdx; i<=rearIdx; ++i)
		{
			if(array[i]!=item)
			{
				array[j]=array[i];
				++j;

			}
			else
				++skipCount;		
		}
		--j;
		rearIdx=j;
		SIZE-=skipCount;
		if(SIZE==0)
			frontIdx=rearIdx=-1;

		if(skipCount)
			return(true);		
	}

	// case 4 : The queue is wrapped around. This can be more tricky and complicated to implement.
	if(rearIdx<frontIdx)
	{
		cout << "I am running the special case" << endl;

		int j=frontIdx;
		int skipCount=0;

		// Front frontIdx to capacity-1.
		for(int i=frontIdx; i<CAPACITY;++i)
		{
			if(array[i]!=item)
			{
				array[j]=array[i];
				++j;
				j=j%CAPACITY;
			}
			else
				++skipCount;
		}

		// From index 0 to rearIdx.
		for(int i=0; i<=rearIdx; ++i)
		{
			if(array[i]!=item)
			{
				array[j]=array[i];
				++j;
				j=j%CAPACITY;
			}
			else
				++skipCount;
		}
		--j;
		if(j==-1)
			j=CAPACITY-1;
		rearIdx=j;
		SIZE-=skipCount;
		if(SIZE==0)
			frontIdx=rearIdx=-1;

		if(skipCount)
			return(true);
	}
	return(false);
}

int main(int argc, char* argv[])
{
	vector<int> items = { 1, 2, 3, 4, 5, 6, 7, 7, 7, 10 };  // works!
	//vector<int> items = { 7, 7, 7, 7, 7, 6, 7, 7, 7, 10 };  //  works!
	//vector<int> items = { 7, 7, 7, 7, 7, 6, 8, 9, 10, 11 }; // works!
	for(const int& iter : items)
		enqueue(iter);

	frontIdx=5;
	rearIdx=4;
	SIZE=10;

	showQueueItems();
	removeAll(7);
	showQueueItems();

	cout << "frontIdx = " << frontIdx << ", rearIdx = " << rearIdx << ", SIZE = " << SIZE << endl;

	// delete the array at the end.
	delete[] array;
	return(0);
}

