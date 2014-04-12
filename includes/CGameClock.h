/*
Filename: CGameClock Author: Chuck Bolin  Date: December 23, 2007
Purpose: Manges game clock. The game runs from Jan. 1880 to Jan 1885
         60 seconds real time = 1 month game time

Implementation:
  CGameClock g_GameClock;
  
  //setup game clock
  g_GameClock.SetMonthYear(0,0); //January 1880
  g_GameClock.Start();

  //loop
  g_GameClock.Update();

  cout << g_GameClock.GetMonth() << "  "
       << g_GameClock.GetYear()  << "  "
       << g_GameClock.GetCounter() << endl;;
    
*/

#ifndef CGAMECLOCK_H
#define CGAMECLOCK_H

#include "CTimer.h"
#include <iostream>

const int C_TIMER_STOPPED = 0;
const int C_TIMER_NORMAL = 1;
const int C_TIMER_FAST2 = 2;
//const int C_TIMER_FAST5 = 5;
const int C_TIMER_FAST15 = 15;
const int C_TIMER_FAST30 = 30;
const int C_TIMER_FAST60 = 60;

class CGameClock{
public:
  CGameClock();
  void Pause();
  void Resume();
  void Reset();
  void Start();
  void SetMonthYear(int month, int year);
  void Update();
  std::string GetMonth();
  std::string GetYear();
  bool IsOver();
  void SetTimeScale(int ts);
  int GetCounter();
  std::string GetTimeScaleSymbol(){return m_timeScaleSymbol;}
  int GetTimeScale(){return m_timeScale;}

private:
  CTimer m_timer;   //1 second updates
  int m_counter;    //0 to 3600
  int m_monthIndex; //0 to 11
  int m_yearIndex;  //0 to 6
  bool m_bPaused;   //true if paused
  bool m_bOver;     //true of time has been reached
  std::string m_month[12]; //stores names of months
  std::string m_year[12];  //stores names of years
  int m_timeScale; 
  std::string m_timeScaleSymbol;
};

#endif



