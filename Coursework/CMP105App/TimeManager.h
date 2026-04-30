#pragma once
#include <fstream>
#include <iomanip>


class TimeManager
{
public:

	void addTime(float dt);
	float getCurrentTime();
	void setFinalTime();
	void resetTime();
	int checkClearTime(float oneStarTime, float twoStarsTime, float threeStarsTime);

private:

	float m_currentTime = 0.f;
	float m_finalTime;
};

