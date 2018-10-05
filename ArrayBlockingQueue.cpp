#include<iostream>
#include<string>

#include "ArrayBlockingQueue.h"
#include "ArrayBlockingQueueImpl.cpp"

using namespace std;

// We first implement the constructor which takes capacity, and fairness mode.
template<typename T> ArrayBlockingQueue<T>::ArrayBlockingQueue(const size_t& capacity, const bool& fair) : m_capacity(capacity), m_fair(fair), m_frontIdx(-1), m_rearIdx(-1), m_size(0)
{
	if(capacity<1)
		throw IllegalArgumentException();
	m_queue = new T[capacity];
	m_name = getName();
}

// We next implement the constructor which takes capacity, fairness mode and a vector from which the ArrayBlockingQueue is initialized.
template<typename T> ArrayBlockingQueue<T>::ArrayBlockingQueue(const size_t& capacity, const bool& fair, const vector<T>& inputCollection) : m_capacity(capacity), m_fair(fair), 
	m_frontIdx(-1), m_rearIdx(-1), m_size(0)
{
	if(capacity<inputCollection.size() || capacity<1lu)
		throw IllegalArgumentException();

	unique_lock<mutex> exclusiveLock(m_mutex);
	m_queue = new T[capacity];
	for(const auto& iter : inputCollection)
		add(iter);
	m_name = getName();
}

// We need a few private internal implementations which manage the insertion and removal from the queue.
template<typename T> bool ArrayBlockingQueue<T>::isEmpty()
{
	return(m_size==0);
}

// We need to identify if the queue is full.
template<typename T> bool ArrayBlockingQueue<T>::isFull()
{
	return(m_size==m_capacity);
}

// We implement the add method now.Inserts the specified element at the tail of this queue if it is possible to do so immediately without exceeding the queue's capacity, 
// returning true upon success and throwing an IllegalStateException if this queue is full.
template<typename T> bool ArrayBlockingQueue<T>::add(const T& item)
{
	unique_lock<mutex> exclusiveLock(m_mutex);
	if(isFull())
		throw IllegalStateException();
	bool returnStatus=enqueue(item);
	return(returnStatus);
}

// We now implement the clear method.Atomically removes all of the elements from this queue. 
template<typename T> void ArrayBlockingQueue<T>::clear()
{
	// We actually dont need to delete anything. simply reset the tracking flags.
	unique_lock<mutex> exclusiveLock(m_mutex);
	m_frontIdx=-1; m_rearIdx=-1; m_size=0;
}

// We need to implement the two critical internal methods enqueue and dequeue methods. They implement actual queue management.
template<typename T> bool ArrayBlockingQueue<T>::enqueue(const T& item)
{
	// All wrapper methods take a lock before calling this function. Please dont lock inside again here.
	if(isFull())
		return(false);

	if(m_frontIdx==-1)
		++m_frontIdx;

	++m_rearIdx;
	m_rearIdx=m_rearIdx%m_capacity;
	m_queue[m_rearIdx]=item;
	++m_size;
	m_cond.notify_all();
	return(true);
} 

// We need to implement the dequeue method now.
template<typename T> pair<bool,T> ArrayBlockingQueue<T>::dequeue()
{
	// All wrapper methods take a lock before calling this function. Please dont lock inside again here.
	T returnItem;
	if(isEmpty())
		return(make_pair(false, T()));

	if(m_frontIdx == m_rearIdx)
	{
		returnItem = m_queue[m_frontIdx];
		m_frontIdx=m_rearIdx=-1;
	}
	else
	{	
		returnItem = m_queue[m_frontIdx];
		m_frontIdx = (m_frontIdx + 1) % m_capacity;
	}
	--m_size;
	m_cond.notify_all();
	return(make_pair(true, returnItem));
}

// Implement simple method to get semaphore name. We dont want to keep deriving this repetative calls of toString().
template<typename T> string ArrayBlockingQueue<T>::getName()
{
	const void * address = static_cast<const void*>(this);
	stringstream stream;
	stream << address;
	string result = stream.str();
	return(result);
}

// Implementation of getThreadId Method. Returns a string of thread-id number.
template<typename T> string ArrayBlockingQueue<T>::getThreadId()
{
	auto myid = this_thread::get_id();
	stringstream ss;
	ss << myid;
	string resultString = ss.str();
	return(resultString);
}

// We need to implement the contains method. Returns true if this queue contains the specified element.
template<typename T> bool ArrayBlockingQueue<T>::contains(const T& item)
{
	unique_lock<mutex> exclusiveLock(m_mutex);
	if(isEmpty())
		return(false);

	bool returnStatus=false;
	if(m_rearIdx>=m_frontIdx)
	{	
		int currentIdx=m_frontIdx;
		while(currentIdx<=m_rearIdx)
		{
			if(m_queue[currentIdx] == item)
				return(true);
			++currentIdx;
		}	
	}	
	else
	{
		int currentIdx=m_frontIdx;
		while(currentIdx<=static_cast<long int>((m_capacity-1)))
		{
			if(m_queue[currentIdx] == item)
				return(true);
			++currentIdx;
		}
		currentIdx=0;
		while(currentIdx != m_rearIdx)
		{
			if(m_queue[currentIdx] == item)
				return(true);
			++currentIdx;
		}
	}	
	return(returnStatus);
}

// We need to implement the drainTo method. Removes all available elements from this queue and adds them to the given collection.
template<typename T> int ArrayBlockingQueue<T>::drainToInternal(vector<T>& target, const int& size)
{
	int returnCount=0;
	if(size == -1)
	{
		while(!isEmpty())
		{
			pair<bool, T> returnItem = dequeue();
			if(returnItem.first)
			{
				target.emplace_back(returnItem.second);
				++returnCount;
			}
		}	
	}
	else
	{
		while(!isEmpty() && returnCount!=size)
		{
			pair<bool, T> returnItem = dequeue();
			if(returnItem.first)
			{
				target.emplace_back(returnItem.second);
				++returnCount;
			}	
		}
	}
	return(returnCount);
}

// Implement the drainTo method as wrapper around above method.
template<typename T> int ArrayBlockingQueue<T>::drainTo(vector<T>& target)
{
	unique_lock<mutex> exclusiveLock(m_mutex);
	int result=drainToInternal(target);
	return(result);
}

// Implement the drainTo method with a given size.
template<typename T> int ArrayBlockingQueue<T>::drainTo(vector<T>& target, const int& size)
{
	unique_lock<mutex> exclusiveLock(m_mutex);
	if(size<1)
		throw IllegalStateException();
	int result=drainToInternal(target, size);
	return(result);
}

// Implement the method offer. Inserts the specified element at the tail of this queue if it is possible to do so immediately without exceeding the queue's capacity, 
// returning true upon success and false if this queue is full.
template<typename T> bool ArrayBlockingQueue<T>::offer(const T& item)
{
	unique_lock<mutex> exclusiveLock(m_mutex);
	bool returnStatus = enqueue(item);
	return(returnStatus);	
}

// Implement the offer method with delay parameters.Inserts the specified element at the tail of this queue, waiting up to 
// the specified wait time for space to become available if the queue is full.
template<typename T> bool ArrayBlockingQueue<T>::offer(const T& item, const long& waitQuantity, const TimeUnit& timeUnit)
{
	bool returnStatus=false;
	auto duration = TimeUtils::waitDuration(waitQuantity, timeUnit).count();
	auto startTime = chrono::high_resolution_clock::now();
	auto endTime = startTime;
	auto durationCount = chrono::duration_cast<chrono::milliseconds>(endTime - startTime).count();	
	while(durationCount<duration)
	{
		returnStatus=offer(item);
		if(returnStatus)
			return(true);
		endTime=chrono::high_resolution_clock::now();
		durationCount = chrono::duration_cast<chrono::milliseconds>(endTime - startTime).count();
	}
	return(returnStatus);
}

// We implement the peek method. Retrieves, but does not remove, the head of this queue, or returns null if this queue is empty.
template<typename T> pair<bool,T> ArrayBlockingQueue<T>::peek()
{
	unique_lock<mutex> exclusiveLock(m_mutex);
	pair<bool, T> returnItem=make_pair(false, T());
	if(isEmpty())
		return(returnItem);
	else
		return(make_pair(true, m_queue[m_frontIdx]));
}

// We next implement the poll method. Retrieves and removes the head of this queue, or returns null if this queue is empty.
template<typename T> pair<bool,T> ArrayBlockingQueue<T>::poll()
{
	unique_lock<mutex> exclusiveLock(m_mutex);
	pair<bool,T> returnItem=make_pair(false, T());
	if(isEmpty())
		return(returnItem);
	else
		returnItem = dequeue();
	return(returnItem);
}

// We next implement the poll with timeout Method. Retrieves and removes the head of this queue, waiting up to the specified wait time if necessary for an element to become available.
template<typename T> pair<bool,T> ArrayBlockingQueue<T>::poll(const long& waitQuantity, const TimeUnit& timeUnit)
{
	unique_lock<mutex> exclusiveLock(m_mutex);
	pair<bool,T> returnItem=make_pair(false, T());
	auto duration = TimeUtils::waitDuration(waitQuantity, timeUnit).count();
	auto startTime = chrono::high_resolution_clock::now();
	auto endTime = startTime;
	auto durationCount = chrono::duration_cast<chrono::milliseconds>(endTime - startTime).count();
	while(durationCount<duration)
	{
		returnItem = dequeue();
		if(returnItem.first)
			return(returnItem);	
		endTime=chrono::high_resolution_clock::now();
		durationCount = chrono::duration_cast<chrono::milliseconds>(endTime - startTime).count();
	}
	return(returnItem);
}

// We next implement the put method.Inserts the specified element at the tail of this queue, waiting for space to become available if the queue is full.
template<typename T> void ArrayBlockingQueue<T>::put(const T& item)
{
	unique_lock<mutex> exclusiveLock(m_mutex);
	if(isFull())
		m_cond.wait(exclusiveLock, [&]() { return((m_capacity-m_size)>0); });
	enqueue(item);
}

// We next implement the remainingCapacity method.Returns the number of additional elements that this queue can ideally (in the absence of memory or resource constraints) accept without blocking.
template<typename T> size_t ArrayBlockingQueue<T>::remainingCapacity()
{
	unique_lock<mutex> exclusiveLock(m_mutex);
	return(m_capacity-m_size);
}

//Removes a single instance of the specified element from this queue, if it is present. More formally, removes an element e such that o.equals(e), 
//if this queue contains one or more such elements. Returns true if this queue contained the specified element (or equivalently, if this queue changed as a result of the call).
//Removal of interior elements in circular array based queues is an intrinsically slow and disruptive operation, so should be undertaken only in exceptional circumstances, 
//ideally only when the queue is known not to be accessible by other threads.

template<typename T> bool ArrayBlockingQueue<T>::remove(const T& item)
{
	unique_lock<mutex> exclusiveLock(m_mutex);
	// remove one instance of the item present in the queue. i.e the first occurance if any found. 
	// return true is modification was done else false.

	// case 1 : empty queue. do nothing.
	if(m_size==0)
		return(false);

	// case 2 : one item and is same as the item to be removed. simply reset the queue and return true.
	if(m_size==1&&m_queue[m_frontIdx]==item)
	{
		m_frontIdx=m_rearIdx=-1;
		m_size=0;
		return(true);
	}

	// case 3 : The queue has not wrapped around end , i.e rearIdx>=frontIdx.
	if(m_rearIdx>m_frontIdx)
	{
		int currentIdx=m_frontIdx;
		int itemPos=-1;
		while(currentIdx<=m_rearIdx)
		{				
			if(m_queue[currentIdx]==item)
			{
				itemPos=currentIdx;
				break;
			}
			++currentIdx;
		}

		if(itemPos!=-1)
		{
			currentIdx=itemPos;
			while(currentIdx<m_rearIdx)
			{
				m_queue[currentIdx]=m_queue[currentIdx+1];
				++currentIdx;
			}
			--m_rearIdx;
			--m_size;
			return(true);
		}		
	}

	// case 4 : The queue has wrapped around the end , ie rearIdx<frontIdx. This one is a bit laborious.
	if(m_rearIdx<m_frontIdx)
	{
		int currentIdx=m_frontIdx;
		int itemPos=-1;
		while(currentIdx<=static_cast<long int>(m_capacity-1))
		{
			if(m_queue[currentIdx]==item)
			{
				itemPos=currentIdx;
				break;
			}
			++currentIdx;
		}

		if(itemPos!=-1)
		{
			currentIdx=itemPos;
			while(currentIdx<static_cast<long int>(m_capacity-1))
			{
				m_queue[currentIdx]=m_queue[currentIdx+1];
				++currentIdx;
			}
		}

		if(itemPos != -1)
		{
			// We need to shift all our elements wrapped around by 1 position from end onwards.
			currentIdx=m_capacity-1;
			while(currentIdx!=m_rearIdx)
			{	
				m_queue[currentIdx]=m_queue[(currentIdx+1)%m_capacity];
				++currentIdx;
				currentIdx=currentIdx%m_capacity;
			}
			--m_rearIdx;
			if(m_rearIdx==-1)
				m_rearIdx=m_capacity-1;
			--m_size;
			return(true);
		}

		// There is a case where item to be removed is on the wrapped up side i.e between index 0 and rearIdx.
		currentIdx=0;
		itemPos=-1;
		while(currentIdx<=m_rearIdx)
		{	
			if(m_queue[currentIdx]==item)
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
			while(currentIdx<m_rearIdx)
			{
				m_queue[currentIdx]=m_queue[currentIdx+1];
				++currentIdx;
			}
			--m_rearIdx;
			if(m_rearIdx==-1)
				m_rearIdx=m_capacity-1;
			--m_size;
			return(true);
		}
	}
	return(false);
}

// We implement the size method. it returns the current size of the queue.
template<typename T> size_t ArrayBlockingQueue<T>::size()
{
	unique_lock<mutex> exclusiveLock(m_mutex);
	return(m_size);
}

// We implement the method take. Retrieves and removes the head of this queue, waiting if necessary until an element becomes available.
template<typename T> T ArrayBlockingQueue<T>::take()
{
	unique_lock<mutex> exclusiveLock(m_mutex);
	if(isEmpty())
		m_cond.wait(exclusiveLock, [&]() { return(m_size>0); });
	pair<bool,T> item = dequeue();
	return(item.second);
}

// We implement the method toArray now.Returns an array containing all of the elements in this queue, in proper sequence; the runtime type of the returned array is that of the specified array.
template<typename T> vector<T> ArrayBlockingQueue<T>::toArray()
{
	unique_lock<mutex> exclusiveLock(m_mutex);
	vector<T> returnVec;
	if(isEmpty())
		return(returnVec);
	else
	{
		returnVec.reserve(m_size);
		if(m_frontIdx<=m_rearIdx)
		{
			int currentIdx=m_frontIdx;
			while(currentIdx<=m_rearIdx)
			{
				returnVec.emplace_back(m_queue[currentIdx]);
				++currentIdx;
			}
		}
		else
		{
			int currentIdx=m_frontIdx;
			while(currentIdx<static_cast<long int>((m_capacity-1)))
			{
				returnVec.emplace_back(m_queue[currentIdx]);
				++currentIdx;
			}
			currentIdx=0;
			while(currentIdx<=m_rearIdx)
			{
				returnVec.emplace_back(m_queue[currentIdx]);
				++currentIdx;
			}
		}
	}
	return(returnVec);
}

// We implement the final method toString. Returns a string representation of this collection with various status details. This is helpful for debugging purpose.
template<typename T> string ArrayBlockingQueue<T>::toString()
{
	unique_lock<mutex> exclusiveLock(m_mutex);
	string returnString = " ==> [ ArrayBlockingQueue Name = ArrayBlockingQueue." + m_name;
	returnString += ", capacity = " + to_string(m_capacity);
	returnString += ", size = " + to_string(m_size);
	returnString += ", remainingCapacity = " + to_string((m_capacity-m_size));
	returnString += ", frontIdx = " + to_string(m_frontIdx);
	returnString += ", rearIdx = " + to_string(m_rearIdx);
	returnString += ", fair = " + to_string(m_fair);
	returnString += ". ]";
	return(returnString);	
}

// Implement the destructor that will clean the queue/array.
template<typename T> ArrayBlockingQueue<T>::~ArrayBlockingQueue()
{
	delete[] m_queue;
}

