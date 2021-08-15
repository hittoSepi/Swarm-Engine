#pragma once
#include <chrono>
#include <iomanip>


class   AtomicClock
{
public:
	using TimePoint = std::chrono::time_point < std::chrono::high_resolution_clock >;

	static TimePoint update()
	{
		auto now = getCurrentTimePoint();
		elapsedTime = now - currentTime;
		currentTime = now;
		return currentTime;
	}

	static TimePoint getCurrentTimePoint()
	{
		return std::chrono::high_resolution_clock::now();
	}

	static double calcDuration(TimePoint start, TimePoint end)
	{
		auto delta = end.time_since_epoch() - start.time_since_epoch();
		auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(delta);
		return duration.count() * 1.0e-6;
	}

	static double calcDurationInMs(TimePoint start, TimePoint end)
	{
		auto delta = end.time_since_epoch() - start.time_since_epoch();
		auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(delta);
		return (double)duration.count();
	}


	static double delta()
	{
		return elapsedTime.count();
	}

	AtomicClock() = default;
private:
	static TimePoint currentTime;
	inline static std::chrono::duration<double> elapsedTime = std::chrono::duration<double>::zero();
};


 
class   Clock
{
public:
	using TimePoint = std::chrono::time_point < std::chrono::high_resolution_clock >;

	Clock() = default;


	static void start()
	{
		startTime = AtomicClock::getCurrentTimePoint(); // Set reference time
	}

	static void update()
	{
		totalElapsedTime = AtomicClock::calcDuration(startTime, AtomicClock::getCurrentTimePoint());
	}

	static TimePoint getCurrentTimePoint()
	{
		return std::chrono::high_resolution_clock::now();
	}

	const static wchar_t* getTimestampW()
	{
		time_t rawtime;
		struct tm * timeinfo;
		wchar_t buffer [80];
		time ( &rawtime );
		
		timeinfo = localtime(&rawtime);
		std::wcsftime(buffer, 80, L"%H:%M:%S", timeinfo);
		return buffer;
	}

	static double getElapsedTime() { return totalElapsedTime; }
	
	static std::string getTimestamp()
	{
		time_t rawtime;
		struct tm * timeinfo;
		char buffer [80];
		time ( &rawtime );
		
		timeinfo = localtime(&rawtime);
		std::strftime(buffer, 80, "%H:%M:%S", timeinfo);
		std::string res = buffer;
		
		return res;
	}
	static double getDelta() { return AtomicClock::delta(); }

private:
	inline static TimePoint		startTime = TimePoint();
	inline static double		totalElapsedTime = 0.0;

};