//CStateIntro.h
#ifndef CSTATEQUIT_H
#define CSTATEQUIT_H

#include <vector>
#include <string>
#include "CGameState.h"
#include "Console.h"

class CStateQuit:public CGameState{
public:
  CStateQuit();
  ~CStateQuit();
  void Initialize();
  CGameState* Update(double timeDifference);
  void Render(Console& con);
  void AddTransitionEvent(int event, CGameState* p_Next);

private:
  std::vector<TRANSITION_EVENT> m_TE; //stores all transition events
  int m_event;
};

#endif