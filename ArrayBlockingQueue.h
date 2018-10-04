#include<iostream>
#include<string>
#include<mutex>
#include<condition_variable>
#include<vector>
#include<utility>
#include<chrono>
#include<sstream>

#include "TimeUtils.h"

#ifndef ArrayBlockingQueue_H
#define ArrayBlockingQueue_H

template<typename T> class ArrayBlockingQueue
{
	private:
		T* m_queue;							// This is pointer pointing to the array allocated for this queue. We implement a circular buffer using this array.
		int m_frontIdx;						// This tracks the front of the queue in the array. Initially when queue is created it is set to -1.
		int m_rearIdx;						// This tracks the end of the queue in the array. Initially when the queue is created it is set to -1.
		size_t m_capacity;					// capacity is the size of the allocated array and it cant be changed after initialization.
		size_t m_size;						// We track size in a variable.
		std::mutex m_mutex;					// We need mutex to ensure thread safety since this class is ThreadSafe as per Java implementation.
		std::condition_variable m_cond;		// This is required for signalling purpose in the code when there are waiting threads either to enqueue/dequeue the elements.		
		bool m_fair;						// This tracks if the implementation is fair to waiting threads i.e FIFO implementation. reduces the performance.

		bool isEmpty();						// returns true/false to indicate if the queue is empty.
		bool isFull();						// returns if the queue is full to capacity.
		bool enqueue(const T&);				// Internal method to add item to the queue.
		std::pair<bool,T> dequeue();		// Internal method to remove item from the queue.
		int drainToInternal(std::vector<T>&, const int& =-1);	// Internal private implementation to cover both public functions of drainTo.
		std::string getName();				// This returns the unique memory address as string for toString method.
		std::string getThreadId();			// This returns the threadId of the executing thread.
		std::string m_name;
					
	public:
		ArrayBlockingQueue(const int&);		// This creates an empty queue with given capacity and fairness set to false.
		ArrayBlockingQueue(const int&, const bool& = false);	// This creates an empty queue with given capacity and fairness mode [ default false ].
		ArrayBlockingQueue(const int&, const bool&, const std::vector<T>&);	// This creates queue with given capacity, fairness mode and items are populated from input vector collection.
		
		ArrayBlockingQueue(const ArrayBlockingQueue&)=delete;
		ArrayBlockingQueue& operator=(const ArrayBlockingQueue&)=delete;
	
		bool add(const T&);					// Inserts the specified element at the tail of this queue if it is possible to do so immediately 
											//without exceeding the queue's capacity, returning true upon success and throwing an IllegalStateException if this queue is full.
		void clear();						// Atomically removes all of the elements from this queue.
		bool contains(const T&);			// Returns true if this queue contains the specified element.
		int drainTo(std::vector<T>&);		// Removes all available elements from this queue and adds them to the given collection.
		int drainTo(std::vector<T>&, const int&);	// Removes given size elements or less [ what ever is available till given size ] and adds them into the collection and returns the items added.
		std::pair<int&,size_t> iterator();  // Returns an iterator over the elements in this queue in proper sequence.
		bool offer(const T&);			// Inserts the specified element at the tail of this queue if it is possible to do so immediately without exceeding the queue's capacity, 
											// returning true upon success and false if this queue is full.
		bool offer(const T&, const long&, const TimeUnit&);	// Inserts the specified element at the tail of this queue, 
																// waiting up to the specified wait time for space to become available if the queue is full.
		T peek();							// Retrieves, but does not remove, the head of this queue, or returns null if this queue is empty.
		T poll();							// Retrieves and removes the head of this queue, or returns null if this queue is empty.
		T poll(const long&, const TimeUnit&);	// Retrieves and removes the head of this queue, waiting up to the specified wait time if necessary for an element to become available.
		void put(const T&);					// Inserts the specified element at the tail of this queue, waiting for space to become available if the queue is full.
		size_t remainingCapacity();			// Returns the number of additional elements that this queue can ideally (in the absence of memory or resource constraints) accept without blocking.
		bool remove(const T&);				// Removes a single instance of the specified element from this queue, if it is present.
		size_t size();						// Returns the number of elements in this queue.
		T take();							// Retrieves and removes the head of this queue, waiting if necessary until an element becomes available.
		std::vector<T> toArray();				// Returns an array containing all of the elements in this queue, in proper sequence.
		std::string toString();				// Returns a string representation of this collection.
};

#endif

