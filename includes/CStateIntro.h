//CStateIntro.h
#ifndef CSTATEINTRO_H
#define CSTATEINTRO_H

#include <vector>
#include <string>
#include "CGameState.h"
#include "Console.h"
#include "Keystatus.h"
#include "CTimer.h"
#include "CAudioManager.h"

struct Pair{
  int x;
  int y;
};

class CStateIntro:public CGameState{
public:
  CStateIntro();
  ~CStateIntro();
  void Initialize();
  CGameState* Update(double timeDifference);
  void Render(Console& con);
  void AddTransitionEvent(int event, CGameState* p_Next);
  Pair GetBorderPosition(int a);

private:
  std::vector<TRANSITION_EVENT> m_TE; //stores all transition events
  int m_event;
  CTimer m_TimerFrameRate;
  int m_mouseX;
  int m_mouseY;
  int m_borderCount;
  CTimer m_timerBorder;
};

#endif