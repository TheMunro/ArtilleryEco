#pragma once

#include <chrono>

/**
 * Helper classes that can be used to accumulate the average runtime of a function
 * TODO - Turn this into a pre-compilation macro so that we can use it in more than one function at a time
 * TODO - Templatize so that we aren't fixed to spitting out information every 1000 calls in number of nanoseconds
 * TODO - Investigate adding a callback logs averages when a PIE session ends
 * 
 */
class FunctionAverageTimerContainer
{
public:
	static FunctionAverageTimerContainer& GetInstance()
	{
		static FunctionAverageTimerContainer instance;
		return instance;
	}

	void AddTime(long long MSAdded)
	{
		TotalTime += MSAdded;
		CallCounter++;

		if (CallCounter % 1000 == 0)
		{
			UE_LOG(LogTemp, Warning, TEXT("Average time for current function nanoseconds = '%lld'"), TotalTime / CallCounter);
		}
	}

private:
	FunctionAverageTimerContainer()
	{
		CallCounter = 0;
		TotalTime = 0;
	}
	
	long long CallCounter;
	long long TotalTime;

public:
	FunctionAverageTimerContainer(FunctionAverageTimerContainer const&) = delete;
	void operator=(FunctionAverageTimerContainer const&) = delete;
};

/**
 * Class used in function we want to log for. Simply declare the class on the stack at the start of the function
 * 
 */
class CustomTimer
{
public:
	CustomTimer()
	{
		start = std::chrono::high_resolution_clock::now();
	}

	~CustomTimer()
	{
		using std::chrono::high_resolution_clock;
		using std::chrono::duration_cast;
		using std::chrono::nanoseconds;
		FunctionAverageTimerContainer::GetInstance().AddTime(duration_cast<nanoseconds>(high_resolution_clock::now() - start).count());
	}

private:
	std::chrono::time_point<std::chrono::steady_clock> start;
};