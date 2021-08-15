#pragma once

class Framerate
{
public:
	using TimePoint = std::chrono::time_point < std::chrono::high_resolution_clock >;

	Framerate()
	{
		start = Clock::getCurrentTimePoint();
		fps = 0;
	}

	void Update()
	{
		fps++;
		
		if (std::chrono::duration_cast<std::chrono::seconds>(std::chrono::high_resolution_clock::now() - oldTime) >= std::chrono::seconds{ 1 })
		{
			oldTime = std::chrono::high_resolution_clock::now();
			lastFps = fps;
			fps = 0;
		}
	}

	int GetFps()
	{
		if (std::chrono::duration_cast<std::chrono::seconds>(std::chrono::high_resolution_clock::now() - checkTime) >= std::chrono::seconds{ 1 }) {
			checkTime = std::chrono::high_resolution_clock::now();
			return lastFps;
		}
		return -1;
	}

private:

	std::chrono::time_point<std::chrono::steady_clock> oldTime = std::chrono::high_resolution_clock::now();
	std::chrono::time_point<std::chrono::steady_clock> checkTime = std::chrono::high_resolution_clock::now();

	
	int fps = 0;
	int lastFps = 0;
	TimePoint	start, end;
};
