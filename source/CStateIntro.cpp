#include "..\includes\CStateIntro.h"
extern CAudioManager g_audio;

CStateIntro::CStateIntro(){
  m_TimerFrameRate.Initialize();
}

CStateIntro::~CStateIntro(){

}

void CStateIntro::Initialize(){
  m_borderCount = 0;
  m_timerBorder.Initialize();
}

CGameState* CStateIntro::Update(double timeDifference){
  m_event = GO_NO_WHERE;

  if(g_audio.IsPlaying(2) == false)
    g_audio.PlaySoundClip(2, 1);


  //process this code for the very first Update(...) call
  static bool bFirstTime = false;
  if(bFirstTime == false){
    bFirstTime = true;
  }

  //check user interface
  INPUT_RECORD InRec = GetConsoleInputInfo();

  //key press events
  if(InRec.EventType == KEY_EVENT){
    m_event = GO_MENU;//press any key
  }

  //mouse events
  else if(InRec.EventType == MOUSE_EVENT){
    //left mouse 
    if(InRec.Event.MouseEvent.dwButtonState == 0x01){
    }
    //right mouse
    else if(InRec.Event.MouseEvent.dwButtonState == 0x02){
    }
    //mouse move
    else if(InRec.Event.MouseEvent.dwEventFlags == MOUSE_MOVED){
      m_mouseX = InRec.Event.MouseEvent.dwMousePosition.X;
      m_mouseY = InRec.Event.MouseEvent.dwMousePosition.Y;
    }
  }
  
  //animated border
  if(m_timerBorder.GetTimer(.05)== true){
    m_borderCount++;
    if(m_borderCount > 205)
      m_borderCount = 0;
  }

  //game state transition code...very important
  for(unsigned int i=0; i< m_TE.size(); ++i){
    if (m_TE[i].event == m_event){
      return m_TE[i].p_gso;
    }
  }

  return 0;
}

void CStateIntro::Render(Console& con){

  static int frameCount = 0;

  con.setColor(14);
  con.setBackgroundColor(0);
  con.cls();
  
  //draw border
  Pair pt;
  int color = 9;
  for(int i = 0; i < 208; i += 1){
    con.setColor(color);
    color++;
    if(color > 15)
      color = 9;
    pt = GetBorderPosition(m_borderCount + i);
    con.writeAscii(pt.x, pt.y, 42);
  }

  //draw info
  con.setColor(15);
  con.write(36, 5, "Chuck's");
  con.setColor(12);
  con.write(34, 7, "Factory Game");

  con.setColor(10);
  con.write(26, 10, "http:\\\\www.gameinstitute.com");
  con.write(31, 12, "Game Challenge #5");
  con.setColor(7);
  con.write(31, 14, "February 23, 2008");

  con.setColor(11);
  con.write(24, 20, "<<< PRESS ANY KEY TO CONTINUE >>>");
  con.flip();
}

Pair CStateIntro::GetBorderPosition(int a){
  Pair temp;
  
  if(a > 207)
    a = a - 207;

  temp.x = 0;
  temp.y = 0;

  if(a >= 0 && a <= 79){
    temp.y = 0;
    temp.x = a;
  }
  else if(a >= 80 && a <= 103){
    temp.y = (a - 80) + 1;
    temp.x = 79;
  }
  else if(a >= 104 && a <= 183){
    temp.y = 24;
    temp.x = 183 - a;
  }
  else if(a >= 184 && a <= 207){
    temp.y = 24 - (a - 183 );
    temp.x = 0;
  }

  return temp;
}

void CStateIntro::AddTransitionEvent(int event, CGameState* p_Next){
  TRANSITION_EVENT te;
  te.event=event;
  te.p_gso = p_Next;

  m_TE.push_back(te);
}