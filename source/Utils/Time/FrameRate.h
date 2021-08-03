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
		oldTime = std::chrono::high_resolution_clock::now();
		static int fps; fps++;
		
		if (std::chrono::duration_cast<std::chrono::seconds>(std::chrono::high_resolution_clock::now() - oldTime) >= std::chrono::seconds{ 1 })
		{
			oldTime = std::chrono::high_resolution_clock::now();
		
			fps = 0;
		}
	}

	int GetFps()
	{
		return fps;
	}

private:

	std::chrono::time_point<std::chrono::steady_clock> oldTime = std::chrono::high_resolution_clock::now();
	int			fps;
	TimePoint	start, end;
};
