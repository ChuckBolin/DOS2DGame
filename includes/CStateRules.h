//CStateIntro.h
#ifndef CSTATERULES_H
#define CSTATERULES_H

#include <vector>
#include <string>
#include "CGameState.h"
#include "Console.h"
#include "Keystatus.h"
#include "CTimer.h"
#include "CAudioManager.h"

class CStateRules:public CGameState{
public:
  CStateRules();
  ~CStateRules();
  void Initialize();
  CGameState* Update(double timeDifference);
  void Render(Console& con);
  void AddTransitionEvent(int event, CGameState* p_Next);

private:
  std::vector<TRANSITION_EVENT> m_TE; //stores all transition events
  int m_event;
  CTimer m_TimerFrameRate;
  int m_page;
};

#endif