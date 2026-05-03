#include "TimeManager.h"
#include <iostream>


void TimeManager::addTime(float dt) { //called in update to add dt to the timer
	m_currentTime += dt;
}

float TimeManager::getCurrentTime() {
	return m_currentTime;
}

void TimeManager::setFinalTime() {
	m_finalTime = m_currentTime;
}

void TimeManager::resetTime() {
	m_currentTime = 0.f;
}

int TimeManager::checkClearTime(float oneStarTime, float twoStarsTime, float threeStarsTime) {
	int starsAchieved;

	if (m_finalTime < threeStarsTime) {
		starsAchieved = 3;
	}
	else if (m_finalTime < twoStarsTime) {
		starsAchieved = 2;
	}
	else if (m_finalTime < oneStarTime) {
		starsAchieved = 1;
	}
	else {
		starsAchieved = 0;
	}



	return starsAchieved;
}