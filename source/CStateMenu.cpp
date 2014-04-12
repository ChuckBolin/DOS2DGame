#include "..\includes\CStateMenu.h"

extern CAudioManager g_audio;

CStateMenu::CStateMenu(){

}

CStateMenu::~CStateMenu(){

}

void CStateMenu::Initialize(){
  m_choice = 0;
  m_rowRef = 5;
  m_borderCount = 0;
  m_timerBorder.Initialize();
}

CGameState* CStateMenu::Update(double timeDifference){
  m_event = GO_NO_WHERE;

  //if(g_audio.IsPlaying(2) == true)//stop intro music
  for(int i = 0; i < 6; ++i)
    g_audio.StopSoundClip(i);
  
  if(g_audio.IsPlaying(6) == false)//burns' speech
    g_audio.PlaySoundClip(6, 0);

  //check user interface
  INPUT_RECORD InRec = GetConsoleInputInfo();

  //key press events
  if(InRec.EventType == KEY_EVENT){
    if(InRec.Event.KeyEvent.uChar.AsciiChar == VK_ESCAPE)
      m_event = GO_QUIT;
    else if(InRec.Event.KeyEvent.uChar.AsciiChar == VK_RETURN){
      m_choice--;
      if(m_choice < 0)
        m_choice = 3;
    }
    else if(InRec.Event.KeyEvent.uChar.AsciiChar == VK_DOWN){
      m_choice++;
      if(m_choice > 3)
        m_choice = 0;
    }    

  }
  //mouse events
  else if(InRec.EventType == MOUSE_EVENT){
    
    if(InRec.Event.MouseEvent.dwButtonState == 0x01){
      
      m_mouseX = InRec.Event.MouseEvent.dwMousePosition.X;
      m_mouseY = InRec.Event.MouseEvent.dwMousePosition.Y;
   
      if(m_mouseY == m_rowRef)
        m_choice = 0;
      else if(m_mouseY == m_rowRef + 3)
        m_choice = 1;
      else if(m_mouseY == m_rowRef + 6)
        m_choice = 2;
      else if(m_mouseY == m_rowRef + 9)
        m_choice = 3;

      if(m_choice == 0)
        m_event = GO_PLAY;
      else if(m_choice == 1)
        m_event = GO_RULES;
      else if(m_choice == 2)
        m_event = GO_SCORE;
      else if(m_choice == 3)
        m_event = GO_QUIT;

    }
    else if(InRec.Event.MouseEvent.dwButtonState == 0x02){
    }
    else if(InRec.Event.MouseEvent.dwEventFlags == MOUSE_MOVED){
      m_mouseX = InRec.Event.MouseEvent.dwMousePosition.X;
      m_mouseY = InRec.Event.MouseEvent.dwMousePosition.Y;
      if(m_mouseY == m_rowRef)
        m_choice = 0;
      else if(m_mouseY == m_rowRef + 3)
        m_choice = 1;
      else if(m_mouseY == m_rowRef + 6)
        m_choice = 2;
      else if(m_mouseY == m_rowRef + 9)
        m_choice = 3;
    }
    else{

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

void CStateMenu::Render(Console& con){
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

  con.setColor(7);
  con.write(34, m_rowRef, "Begin Work");
  con.write(31, m_rowRef + 3, "Production Rules");
  con.write(30, m_rowRef + 6, "Highest Production");
  con.write(32, m_rowRef + 9, "Stop Production");

  con.setColor(10);

  switch(m_choice){
  case 0:
    con.write(34, m_rowRef, "Begin Work");
    break;
  case 1:
    con.write(31, m_rowRef + 3, "Production Rules");
    break;
  case 2:
    con.write(30, m_rowRef + 6, "Highest Production");
    break;
  case 3:
    con.write(32, m_rowRef + 9, "Stop Production");
    break;
  }


  con.flip();

}

Pair CStateMenu::GetBorderPosition(int a){
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


void CStateMenu::AddTransitionEvent(int event, CGameState* p_Next){
  TRANSITION_EVENT te;
  te.event=event;
  te.p_gso = p_Next;

  m_TE.push_back(te);
}