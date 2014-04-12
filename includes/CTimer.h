//CTimer.h
//Need to link to winmm.lib because of timeGetTime()
#ifndef C_TIMER_H
#define C_TIMER_H

#include <ctime>
#include <windows.h>
#include <iostream>

class CTimer{
public:
  CTimer::CTimer();
  CTimer::~CTimer();
  bool SecondTimer();
  bool GetTimer(double timeDiff);
  bool Initialize();
  __int64 GetFrequency();
  double GetTimeDifference();

private:
	__int64 cntsPerSec;
  __int64 lastTime;
  __int64 currTime; 
  double timeScale;
  __int64 tempTime;
	double timeElapsed;
  double secondsElapsed;
  double deltaTime;
  bool counterAvailable;
};

#endif