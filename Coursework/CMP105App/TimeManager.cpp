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
		std::cout << "Well done 3 stars";
		starsAchieved = 3;
	}
	else if (m_finalTime < twoStarsTime) {
		std::cout << "okay, 2 stars";
		starsAchieved = 2;
	}
	else if (m_finalTime < oneStarTime) {
		std::cout << "not bad, 1 star";
		starsAchieved = 1;
	}
	else {
		std::cout << "buddy.";
		starsAchieved = 0;
	}



	return starsAchieved;
}