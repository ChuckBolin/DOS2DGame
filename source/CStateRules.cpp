#include "..\includes\CStateRules.h"
extern CAudioManager g_audio;

CStateRules::CStateRules(){
  m_TimerFrameRate.Initialize();
}

CStateRules::~CStateRules(){

}

void CStateRules::Initialize(){
  m_page = 0;
}

CGameState* CStateRules::Update(double timeDifference){
  m_event = GO_NO_WHERE;

  for(int i = 0; i < 4; ++i)//stop level music
    g_audio.StopSoundClip(i);

  for(int i = 5; i < 9; ++i)
    g_audio.StopSoundClip(i);

  //check user interface
  INPUT_RECORD InRec = GetConsoleInputInfo();

  //key press events
  if(InRec.EventType == KEY_EVENT){
    if(InRec.Event.KeyEvent.uChar.AsciiChar == VK_RETURN)
      m_event = GO_MENU;
    else if(InRec.Event.KeyEvent.uChar.AsciiChar == VK_SPACE && m_page == 0){//user swaps help screens
      m_page++;
      if(m_page > 1){
        m_page = 0;
        m_event = GO_MENU;

      }
    } 
  }
  //mouse events
  else if(InRec.EventType == MOUSE_EVENT){
    
    if(InRec.Event.MouseEvent.dwButtonState == 0x01){
      //con.write(1,1,InRec.Event.MouseEvent.dwMousePosition.X);
      //con.write(4,1,InRec.Event.MouseEvent.dwMousePosition.Y);
    }
    else if(InRec.Event.MouseEvent.dwButtonState == 0x02){
    }
    else if(InRec.Event.MouseEvent.dwEventFlags == MOUSE_MOVED){
      //g_mouseX = InRec.Event.MouseEvent.dwMousePosition.X;
      //g_mouseY = InRec.Event.MouseEvent.dwMousePosition.Y;
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

void CStateRules::Render(Console& con){

  static int frameCount = 0;
  con.setBackgroundColor(0);
  con.cls();
  //con.setColor(15);
 
  //display framerate to console title
  frameCount++;
  char buffer[256];
  if(m_TimerFrameRate.GetTimer(1.0)){
    ::sprintf(buffer,"%d FPS", frameCount);
    con.setTitle(buffer);
    frameCount=0;
  }

  if(m_page == 0){
    con.setColor(14);
    con.write(20, 1,"********* Production Rules *********");
    con.setColor(11);
    con.write(2, 3, "There are 4 game levels.");
    con.write(2, 4, "For each level parts in the shape of patterns move along the conveyor");
    con.write(2, 5, "from left to right.  The player 'left clicks' the assembly line to");
    con.write(2, 6, "pickup the parts.  The parts may be rotated by 'right clicking' the");
    con.write(2, 7, "mouse.  Parts that are not picked up move to the far right and become");
    con.write(2, 8, "scrap. This is not good. Scrap costs money!");
    con.setColor(10);   
    con.write(2, 10, "The goal is to move the parts from the conveyor and place them onto");
    con.write(2, 11, "correct pallets.  Pallets have color requirements such as Red, Green,");
    con.write(2, 12, "or Blue. Mixed pallets can accept parts of any color.  Level 1 has ");
    con.write(2, 13, "4 Mixed pallets. Color does not matter.  Level 4 has 3 color specific");
    con.write(2, 14, "pallets of Red, Green, and Blue. It contains 1 pallet of Mixed color");
    con.write(2, 16, "The player can place unused parts in the area labeled scrap. Scrapped");
    con.write(2, 17, "parts cost money.");
    con.setColor(12);
    con.write(2, 19, "A level is complete when the 4 pallets are loaded with the correctly");
    con.write(2, 20, "colored parts.  The game ends when all four levels are complete OR");
    con.write(2, 21, "the manufacturing operation runs out of money");
    
    con.setColor(14);
    con.write(18, 23, "Press SPACEBAR to see dollar assignments!");
  }

  else if(m_page == 1){
    con.setColor(14);
    con.write(20, 1,"********* Dollar Assignments *********");
    con.setColor(11);
    con.write(15, 3, "        Cost Per   Mixed      Red       Green     Blue");
    con.write(15, 4, "Level    Block     Pallet     Pallet    Pallet    Pallet");
    con.write(15, 5, "--------------------------------------------------------");
    con.write(15, 7, "  I     $1         $75        na        na        na    ");
    con.write(15, 9, "  II    $2         $75        $150      na        na    ");
    con.write(15, 11,"  III   $3         $75        $150      $300      na    ");
    con.write(15, 13,"  IV    $4         $75        $150      $300      $450  ");
   
    con.setColor(10);
    con.write(15, 17,"Parts consist of 1 or more blocks.  Parts added to pallets");
    con.write(15, 18,"do not incur a cost. Scrapped parts incur a cost based ");
    con.write(15, 19,"upon the level and number of blocks per part. Money is");
    con.write(15, 20,"received for completed pallets when all 4 pallets are");
    con.write(15, 21,"complete.");

    con.setColor(14);
    con.write(18, 23, "Press ENTER  to exit the Production Rules!");

  }

  con.flip();
}

void CStateRules::AddTransitionEvent(int event, CGameState* p_Next){
  TRANSITION_EVENT te;
  te.event=event;
  te.p_gso = p_Next;

  m_TE.push_back(te);
}