//CStateIntro.h
#ifndef CSTATEWIN_H
#define CSTATEWIN_H

#include "CGameState.h"
#include "Console.h"

class CStateWin:public CGameState{
public:
  CStateWin();
  ~CStateWin();
  void Initialize();
  CGameState* Update(double timeDifference);
  void Render(Console& con);
  void AddTransitionEvent(int event, CGameState* p_Next);

private:

};

#endif