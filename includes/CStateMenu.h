//CStateIntro.h
#ifndef CSTATEMENU_H
#define CSTATEMENU_H

#include <vector>
#include <string>
#include "CGameState.h"
#include "Console.h"
#include "Keystatus.h"
#include "CTimer.h"
#include "CStateIntro.h"
#include "CAudioManager.h"

/*
struct Pair{
  int x;
  int y;
};*/

class CStateMenu:public CGameState{
public:
  CStateMenu();
  ~CStateMenu();
  void Initialize();
  CGameState* Update(double timeDifference);
  void Render(Console& con);
  void AddTransitionEvent(int event, CGameState* p_Next);
  Pair GetBorderPosition(int a);

private:
  std::vector<TRANSITION_EVENT> m_TE; //stores all transition events
  int m_event;
  int m_choice;
  int m_rowRef;
  int m_mouseX;
  int m_mouseY;
  int m_borderCount;
  CTimer m_timerBorder;
};

#endif