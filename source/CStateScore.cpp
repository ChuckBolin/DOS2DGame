#include "..\includes\CStateScore.h"

extern float g_lowestScore;
extern float g_totalValue;
extern std::string g_playerName;
extern CAudioManager g_audio;

CStateScore::CStateScore(){
  m_TimerFrameRate.Initialize();
}

CStateScore::~CStateScore(){

}

void CStateScore::Initialize(){
  m_score.LoadScore("highscore.txt", 10);
  g_lowestScore = m_score.getLowScore();
}

CGameState* CStateScore::Update(double timeDifference){
  m_event = GO_NO_WHERE;

  for(int i = 0; i < 4; ++i)
    g_audio.StopSoundClip(i);

  for(int i = 5; i < 9; ++i)
    g_audio.StopSoundClip(i);

  //check user interface
  INPUT_RECORD InRec = GetConsoleInputInfo();

  if(g_totalValue > g_lowestScore){
    m_score.addScore(g_playerName, (int)g_totalValue);
    g_totalValue = 0; //reset total score
    g_lowestScore = m_score.getLowScore();
  }

  //key press events
  if(InRec.EventType == KEY_EVENT){//no need to get user name
    m_event = GO_MENU;
  }
  //mouse events
  else if(InRec.EventType == MOUSE_EVENT){
    
    if(InRec.Event.MouseEvent.dwButtonState == 0x01){
    }
    else if(InRec.Event.MouseEvent.dwButtonState == 0x02){
    }
    else if(InRec.Event.MouseEvent.dwEventFlags == MOUSE_MOVED){
    }
    else{

    }
  }
  
  //game state transition code...very important
  for(unsigned int i=0; i< m_TE.size(); ++i){
    if (m_TE[i].event == m_event){
      return m_TE[i].p_gso;
    }
  }


  return 0;
}

void CStateScore::Render(Console& con){

  static int frameCount = 0;
  con.setBackgroundColor(1);
  con.cls();
  con.setColor(14);

  //display framerate to console title
  frameCount++;
  char buffer[256];
  if(m_TimerFrameRate.GetTimer(1.0)){
    ::sprintf(buffer,"%d FPS", frameCount);
    con.setTitle(buffer);
    frameCount=0;
  }
  
  con.write(20, 1,"********* Top Ten Producers *********");
  int num = m_score.size();  
  if(m_score.size() > 10)
    num = 10;
  else
    num = m_score.size();

  for(int i = 0; i < num; ++i){
    con.setColor(15);
    con.write(20, 3 + (i * 2), m_score.getName(i));
    con.setColor(10);
    con.write(52, 3 + (i * 2), m_score.getScore(i));
  }
  
  con.setColor(15);
  con.write(26, 24, "Press any key to continue!");

  con.flip();
}

void CStateScore::AddTransitionEvent(int event, CGameState* p_Next){
  TRANSITION_EVENT te;
  te.event=event;
  te.p_gso = p_Next;

  m_TE.push_back(te);
}