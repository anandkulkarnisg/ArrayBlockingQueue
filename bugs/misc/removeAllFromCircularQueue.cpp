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

bool remove(const int& item)
{
	// remove one instance of the item present in the queue. i.e the first occurance if any found. 
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
		int currentIdx=frontIdx;
		int itemPos=-1;
		while(currentIdx<=rearIdx)
		{				
			if(array[currentIdx]==item)
			{
				itemPos=currentIdx;
				break;
			}
			++currentIdx;
		}

		if(itemPos!=-1)
		{
			currentIdx=itemPos;
			while(currentIdx<rearIdx)
			{
				array[currentIdx]=array[currentIdx+1];
				currentIdx++;
			}
			--rearIdx;
			--SIZE;
			return(true);
		}		
	}

	// case 4 : The queue has wrapped around the end , ie rearIdx<frontIdx. This one is a bit laborious.
	if(rearIdx<frontIdx)
	{
		int currentIdx=frontIdx;
		int itemPos=-1;
		while(currentIdx<=CAPACITY-1)
		{
			if(array[currentIdx]==item)
			{
				itemPos=currentIdx;
				break;
			}
			++currentIdx;
		}

		if(itemPos!=-1)
		{
			currentIdx=itemPos;
			while(currentIdx<CAPACITY-1)
			{
				array[currentIdx]=array[currentIdx+1];
				currentIdx++;
			}
		}

		if(itemPos != -1)
		{
			// We need to shift all our elements wrapped around by 1 position from end onwards.
			currentIdx=CAPACITY-1;
			while(currentIdx!=rearIdx)
			{	
				array[currentIdx]=array[(currentIdx+1)%CAPACITY];
				++currentIdx;
				currentIdx=currentIdx%CAPACITY;
			}
			--rearIdx;
			if(rearIdx==-1)
				rearIdx=CAPACITY-1;
			--SIZE;
			return(true);
		}

		// There is a case where item to be removed is on the wrapped up side i.e between index 0 and rearIdx.
		currentIdx=0;
		itemPos=-1;
		while(currentIdx<=rearIdx)
		{	
			if(array[currentIdx]==item)
			{
				itemPos=currentIdx;
				break;
			}
			++currentIdx;
		}

		// We have found the element and needs a removal.
		if(itemPos!=-1)
		{
			currentIdx=itemPos;
			while(currentIdx<rearIdx)
			{
				array[currentIdx]=array[currentIdx+1];
				++currentIdx;
			}
			--rearIdx;
			if(rearIdx==-1)
				rearIdx=CAPACITY-1;
			--SIZE;
			return(true);
		}
	}
	return(false);
}

int main(int argc, char* argv[])
{
	vector<int> items = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
	for(const int& iter : items)
		enqueue(iter);

	frontIdx=5;
	rearIdx=1;

	cout << "removal status of 100 = " << remove(100) << endl;
	cout << "The indices are frontIdx = " << frontIdx << ", the rearIdx = " << rearIdx << endl;

	if(frontIdx>rearIdx)
	{
		for(int i=frontIdx; i<CAPACITY;++i)
			cout << array[i] << ",";
		for(int i=0; i<=rearIdx; ++i)
			cout << array[i] << ",";
	}
	else
	{
		for(int i=frontIdx; i<=rearIdx; ++i)
			cout << array[i] << ",";
		cout << '\n';
	}

	// delete the array at the end.
	delete[] array;
	return(0);
}

