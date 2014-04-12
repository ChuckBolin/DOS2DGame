//CStateIntro.h
#ifndef CSTATEPLAY_H
#define CSTATEPLAY_H

#include <iostream>
#include <vector>
#include <string>
#include "CGameState.h"
#include "Console.h"
#include "Keystatus.h"
#include "CTimer.h"
#include "CBlock.h"
#include "CPattern.h"
#include "CLog.h"
#include "CFileReader.h"
#include "CAudioManager.h"

//#define DEBUG_PLAY

const int LOC_NOWHERE = 0;
const int LOC_ASSY = 1;
const int LOC_SCRAP = 2;
const int LOC_PALLET1 = 3;
const int LOC_PALLET2 = 4;
const int LOC_PALLET3 = 5;
const int LOC_PALLET4 = 6;
const int MAX_PARTS = 21;
const int RND_E = 90;
const int RND_M = 5;
const int RND_D = 5;

struct STRUCT_CALC{
  float blockCost;
  float mixedPalletValue;
  float redPalletValue;
  float greenPalletValue;
  float bluePalletValue;
  float pallet1;
  float pallet2;
  float pallet3;
  float pallet4;
};

class CStatePlay:public CGameState{
public:
  CStatePlay();
  ~CStatePlay();
  void Initialize();
  CGameState* Update(double timeDifference);
  void Render(Console& con);
  void AddTransitionEvent(int event, CGameState* p_Next);

  //game play methods
  void DrawAssemblyLine(Console& con);
  void DrawAssemblyLineOverlay(Console& con);
  void DrawPallets(Console& con);
  void DrawScrap(Console& con);
  int GetSnapX(int x, int offset);
  int GetSnapY(int y, int offset);
  bool IsPatternMatch();
  bool IsBlockExists(int block, unsigned int value);
  int GetBit(int r, int c);
  int GetRandomShape(int level, int nEasy, int nModerate, int nDifficult);
  void LoadPatterns();
  int GetRandomColor();
  bool IsColorLegal();//int currentColor, int palletColor); 
  int CStatePlay::GetHexStringCharToDecimal(std::string ch);
  unsigned int CStatePlay::ConvertHexStringToDecimal(std::string sValue);

private:
  std::vector<TRANSITION_EVENT> m_TE; //stores all transition events
  int m_event;
  CTimer m_TimerFrameRate;  
  CPattern m_currentShape;//
  CPattern m_nextShape;
  int m_currentColor;
  int m_nextColor;  
  bool m_firstTime;

  bool m_bNextShapeExists;
  CPattern m_pattern[MAX_PARTS];
  int m_mouseX;
  int m_mouseY;
  int m_index;
  CTimer m_timerKey;
//  void LoadPatterns();

  //assembly line stuff
  int m_assemblyCount;
  bool b_runAssemblyLine;
  CTimer timerAssemblyLine;

  //level information
  int m_level;

  //displays text for color of pallet
  std::string m_type1;
  std::string m_type2;
  std::string m_type3;
  std::string m_type4;

  //stores allowable color
  int m_color1;
  int m_color2;
  int m_color3;
  int m_color4;

  //parts coming down assembly line
  unsigned int m_currentPart;
  unsigned int m_nextPart;
  unsigned int m_pallet1;
  unsigned int m_pallet2;
  unsigned int m_pallet3;
  unsigned int m_pallet4;
  int m_palletCount;

  bool m_bPickUp; //true if mouse can pick it up
  bool m_bValidPatternsFile;
  int m_location; //defines general area

  //scrap stuff
  int m_numScrappedPatternCount;
  int m_numScrappedBlockCount;
  
  //stores values for calculating
  STRUCT_CALC m_tableValue[4];
  bool m_filled1;
  bool m_filled2;
  bool m_filled3;
  bool m_filled4;
  bool m_levelComplete;
  bool m_gameOver;

  //key value
  float m_totalValue;
  bool m_bYouLose;

  void NewLevel();
  void NewGame();
};

#endif