//CStateIntro.h
#ifndef CSTATEOVER_H
#define CSTATEOVER_H

#include "CGameState.h"
#include "Console.h"

class CStateOver:public CGameState{
public:
  CStateOver();
  ~CStateOver();
  void Initialize();
  CGameState* Update(double timeDifference);
  void Render(Console& con);
  void AddTransitionEvent(int event, CGameState* p_Next);

private:

};

#endif