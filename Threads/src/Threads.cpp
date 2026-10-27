#include <iostream>
#include <mutex>
#include <thread>

int counter = 0;			// one int, visible to both threads
std::mutex counterMutex;	// the lock that guards counter

void addMany()
{
	for (int i = 0; i < 1000; ++i)
	{
		// lock_guard takes the mutex here and releases it
		// when hold goes out of scope at the end of the block
		std::lock_guard<std::mutex> hold(counterMutex);

		// only the thread that holds the mutex reaches this line;
		// the other waits at the lock_guard above
		++counter;
	}
}

int main()
{
	std::thread first(addMany);		// starts a thread running addMany
	std::thread second(addMany);	// starts a second thread on the same function

	first.join();	// main waits until first has finished
	second.join();	// main waits until second has finished

	// both loops are done, so counter is no longer being written
	std::cout << counter << '\n';
	return 0;
}
