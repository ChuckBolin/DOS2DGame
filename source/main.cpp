//main.cpp
#include <iostream>
#include <string>
#include <cstdlib>
#include <windows.h>
#include "..\includes\CLog.h"
#include "..\includes\Keystatus.h"
#include "..\includes\Console.h"
#include "..\includes\CTimer.h"
#include "..\includes\CINIReader.h"
#include "..\includes\CGameState.h"
#include "..\includes\CStateIntro.h"
#include "..\includes\CStateMenu.h"
#include "..\includes\CStatePlay.h"
#include "..\includes\CStateQuit.h"
#include "..\includes\CStateRules.h"
#include "..\includes\CStateScore.h"
#include "..\includes\CStateWin.h"
#include "..\includes\CStateOver.h"
#include "..\includes\CStateIntro.h"
#include "..\includes\CAudioManager.h"

using namespace std;

//manages game states
CGameState* g_pStateIntro = new CStateIntro;
CGameState* g_pStateMenu = new CStateMenu;
CGameState* g_pStateQuit = new CStateQuit;
CGameState* g_pStateRules = new CStateRules;
CGameState* g_pStateScore = new CStateScore;
CGameState* g_pStatePlay = new CStatePlay;
CGameState* g_pCurrent = 0;
CGameState* g_pNext = 0;

//global variables for high/low score
float g_totalValue = 0;
float g_lowestScore = 0;
std::string g_playerName;
CAudioManager g_audio;

//******************************************************************************
void Initialize(){                      //  I N I T I A L I Z E
//******************************************************************************

  //define transition events for game states
  g_pStateIntro->AddTransitionEvent(GO_MENU, g_pStateMenu);
  
  g_pStateMenu->AddTransitionEvent(GO_QUIT, g_pStateQuit);
  g_pStateMenu->AddTransitionEvent(GO_RULES, g_pStateRules);
  g_pStateMenu->AddTransitionEvent(GO_SCORE, g_pStateScore);
  g_pStateMenu->AddTransitionEvent(GO_PLAY, g_pStatePlay);
  
  g_pStateRules->AddTransitionEvent(GO_MENU, g_pStateMenu);
  
  g_pStateScore->AddTransitionEvent(GO_MENU, g_pStateMenu);
  
  g_pStatePlay->AddTransitionEvent(GO_MENU, g_pStateMenu);
  g_pStatePlay->AddTransitionEvent(GO_SCORE, g_pStateScore);//if new high score
 
  //perform any required game state initialization
  g_pStateIntro->Initialize();
  g_pStateMenu->Initialize();
  g_pStateQuit->Initialize();
  g_pStateRules->Initialize();
  g_pStateScore->Initialize();
  g_pStatePlay->Initialize();

  //start game
  g_pCurrent = g_pStateIntro;//g_pStatePlay;// 

  //initialize sound if audio initializes correctly
  if(g_audio.IsValidSound() == true){
    g_audio.LoadSample(0, "sounds\\burns_fired.wav");
    g_audio.LoadSample(1, "sounds\\Kids_Music_1.wav");
    g_audio.LoadSample(2, "sounds\\Kids_Music_2.wav");
    g_audio.LoadSample(3, "sounds\\Kids_Music_3.wav");
    g_audio.LoadSample(4, "sounds\\victory_fanfare.mp3");
    g_audio.LoadSample(5, "sounds\\burns_started.wav");
    g_audio.LoadSample(6, "sounds\\burns_succeed.wav");
    g_audio.LoadSample(7, "sounds\\truckhorn.wav");
    g_audio.LoadSample(8, "sounds\\trucks021.wav");
  }
}

//********************************************************************
// main()
//********************************************************************
int main(){

  //instantiate log object and log
  CLog *pLog = CLog::Instance();
  pLog->Log("*********************************************************");
  pLog->Log("Program starting..................");
  pLog->SetDelimiter(':');
  pLog->LogDate();
  pLog->LogTime();
  pLog->Log("************************ *********************************");
   
  cout << "Enter name: ";
  cin >> g_playerName;
  //system("cls");

  //initialize console
  Console con(80, 25);//50);
  con.SetConsolePosition(0,0);
  con.CenterConsoleWindow();
  con.cls();

  Initialize();
  
  CONSOLE_SCREEN_BUFFER_INFO csbi = con.GetScreenBufferInfo();
  pLog->LogPair("Screen Buffer Size",csbi.dwSize.X, csbi.dwSize.Y);
  pLog->LogPair("Window Size",csbi.dwMaximumWindowSize.X, csbi.dwMaximumWindowSize.Y);
  pLog->LogPair("Left-Top",csbi.srWindow.Left, csbi.srWindow.Top);
  pLog->LogPair("Right-Bottom",csbi.srWindow.Right, csbi.srWindow.Bottom);
  pLog->LogPair("Maximum Window Size", csbi.dwMaximumWindowSize.X, csbi.dwMaximumWindowSize.Y);
  COORD maxSize = con.GetMaximumScreenSize();
  pLog->LogPair("Max Size",maxSize.X, maxSize.Y);

  //game timer
  CTimer loopTimer;
  CTimer renderTimer;
  loopTimer.Initialize();
  renderTimer.Initialize();
  double timeDelta = 0;

  char buffer[] = "Chuck's Factory Game";
  con.setTitle(buffer);

  //main game loop
  //********************************************
  bool quit = false;    
  while(!quit){ 
    timeDelta = loopTimer.GetTimeDifference();
    g_pNext = g_pCurrent->Update(timeDelta);
	  if(NULL != g_pNext)
	  {
      g_pCurrent = g_pNext;
    }
     
    if(g_pCurrent == g_pStateQuit )
      quit = true;

  	// Render our current game object
    //if(renderTimer.GetTimer(0.1)== true)
      g_pCurrent->Render(con);
  }
  //********************************************
  //end game loop

  //free resources
  delete g_pStateIntro;
  delete g_pStateMenu;
  delete g_pStateQuit;
  delete g_pStateRules;
  delete g_pStateScore;
  delete g_pStatePlay;

  for(int i = 0; i < 9; ++i)
    g_audio.StopSoundClip(i);

  system("cls");
  
  //normal termination of game
  pLog->Log("*********************************************************");
  pLog->Log("Program terminating normally..................");
  pLog->SetDelimiter(':');
  pLog->LogTime();
  pLog->Log("*********************************************************");
  
  return 0;
}



 