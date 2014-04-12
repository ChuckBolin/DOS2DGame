//CStateIntro.h
#ifndef CSTATESCORE_H
#define CSTATESCORE_H

#include <vector>
#include <string>
#include "CGameState.h"
#include "Console.h"
#include "Keystatus.h"
#include "CTimer.h"
#include "CHighScore.h"

class CStateScore:public CGameState{
public:
  CStateScore();
  ~CStateScore();
  void Initialize();
  CGameState* Update(double timeDifference);
  void Render(Console& con);
  void AddTransitionEvent(int event, CGameState* p_Next);

private:
  std::vector<TRANSITION_EVENT> m_TE; //stores all transition events
  int m_event;
  CTimer m_TimerFrameRate;
  CHighScore m_score;
};

#endif