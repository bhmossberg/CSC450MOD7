/**
 * @file CSC450_Mod7_CT_Mossberg.cpp
 * @brief Two counter threads: one counts up to 20, then the other counts down to 0.
 *
 * The up-counter publishes a flag under a mutex and notifies a condition
 * variable. The down-counter waits on that flag, so it does not run its
 * loop until the up-counter has stored 20. main joins both threads.
 *
 * @author Benjamin Mossberg
 * @date 2026-10-11
 * @version 1.0
 */

#include <condition_variable>
#include <iostream>
#include <mutex>
#include <string>
#include <thread>

namespace
{
const int kLimit = 20;

std::mutex counterMutex;
std::condition_variable countReached;
bool upCounterDone = false;

void countUp()
{
	const std::string label = "count-up";
	for (int value = 0; value <= kLimit; ++value)
	{
		std::lock_guard<std::mutex> lock(counterMutex);
		std::cout << label << ": " << value << '\n';
		if (value == kLimit)
		{
			upCounterDone = true;
			countReached.notify_one();
		}
	}
}

void countDown()
{
	const std::string label = "count-down";
	std::unique_lock<std::mutex> lock(counterMutex);
	countReached.wait(lock, [] { return upCounterDone; });
	for (int value = kLimit; value >= 0; --value)
		std::cout << label << ": " << value << '\n';
}
} // namespace

/**
 * @brief Starts the up-counter and the down-counter, then waits for both.
 *
 * The down-counter thread is created immediately, but its loop does not
 * begin until countUp stores the done flag. Joining both threads keeps
 * main from returning while either counter is still running.
 *
 * @return 0 after both threads have been joined.
 */
int main()
{
	std::thread upThread(countUp);
	std::thread downThread(countDown);
	upThread.join();
	downThread.join();
	return 0;
}
