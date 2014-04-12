//CGameState.h
#ifndef CGAMESTATE_H
#define CGAMESTATE_H

class CGameState;
class Console;

//constants for game state transition events
const int GO_NO_WHERE = -1;
const int GO_INTRO = 0;
const int GO_MENU = 1;
const int GO_RULES = 2;
const int GO_SCORE = 3;
const int GO_PAUSE = 4;
const int GO_PLAY = 5;
const int GO_QUIT = 100;

struct TRANSITION_EVENT{
  int event;
  CGameState* p_gso;
};

class CGameState{
public:
  //CGameState();
  //~CGameState();
  virtual void Initialize() = 0;
  virtual CGameState* Update(double timeDifference) = 0;
  virtual void Render(Console& con) = 0;
  virtual void AddTransitionEvent(int event, CGameState* p_Next) = 0;
private:

};

#endif
