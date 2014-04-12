#include "..\includes\CStatePlay.h"

extern float g_totalValue;
extern float g_lowestScore;
extern CAudioManager g_audio;

CStatePlay::CStatePlay(){
  m_TimerFrameRate.Initialize();

  //level 1
  m_tableValue[0].blockCost = 1;
  m_tableValue[0].pallet1 = 75;
  m_tableValue[0].pallet2 = 75;
  m_tableValue[0].pallet3 = 75;
  m_tableValue[0].pallet4 = 75;

  //level 2
  m_tableValue[1].blockCost = 2;
  m_tableValue[1].pallet1 = 75;
  m_tableValue[1].pallet2 = 150;
  m_tableValue[1].pallet3 = 75;
  m_tableValue[1].pallet4 = 75;

  //level 3
  m_tableValue[2].blockCost = 3;
  m_tableValue[2].pallet1 = 75;
  m_tableValue[2].pallet2 = 150;
  m_tableValue[2].pallet3 = 300;
  m_tableValue[2].pallet4 = 75;

  //level 4
  m_tableValue[3].blockCost = 4;
  m_tableValue[3].pallet1 = 75;
  m_tableValue[3].pallet2 = 150;
  m_tableValue[3].pallet3 = 300;
  m_tableValue[3].pallet4 = 450;

  LoadPatterns();
  m_timerKey.Initialize();

  //assembly initialization
  b_runAssemblyLine = true; //turn it on
  timerAssemblyLine.Initialize();
}

CStatePlay::~CStatePlay(){

}

void CStatePlay::NewLevel(){
  m_level++;
  if(m_level > 4){
    //game over code
    m_gameOver = true;
    return;
  }
  m_bNextShapeExists = false; //means 2nd part does not exist
  m_index = 0;
  m_assemblyCount = 0;//used to move assembly line
  m_currentShape = m_pattern[GetRandomShape(m_level, RND_E, RND_M, RND_D)];//rand()% 4];
  m_currentShape.SetRefPosition(0,1);
  m_currentColor = GetRandomColor();
  m_location = LOC_ASSY; 
  m_bPickUp = false;
  m_pallet1 = 0;// 65502;
  m_pallet2 = 0;//65535;
  m_pallet3 = 0;//65535;
  m_pallet4 = 0;//65535;
  m_palletCount = 0;
  m_filled1 = false;
  m_filled2 = false;
  m_filled3 = false;
  m_filled4 = false;
  m_numScrappedPatternCount = 0;//debug use
  m_numScrappedBlockCount = 0;  //debug use
  m_levelComplete = false;

  //configure labels for pallet based upon game level
  switch(m_level){
  case 1:
    m_type1 = "Mixed";
    m_type2 = "Mixed";
    m_type3 = "Mixed";
    m_type4 = "Mixed";
    m_color1 = 0;
    m_color2 = 0;
    m_color3 = 0;
    m_color4 = 0;
    break;
  case 2:
    m_type1 = "Mixed";
    m_type2 = "Red";
    m_type3 = "Mixed";
    m_type4 = "Mixed";
    m_color1 = 0;
    m_color2 = 12;
    m_color3 = 0;
    m_color4 = 0;
    break;
  case 3:
    m_type1 = "Mixed";
    m_type2 = "Red";
    m_type3 = "Green";
    m_type4 = "Mixed";
    m_color1 = 0;
    m_color2 = 12;
    m_color3 = 10;
    m_color4 = 0;
    break;
  case 4:
    m_type1 = "Mixed";
    m_type2 = "Red";
    m_type3 = "Green";
    m_type4 = "Blue";
    m_color1 = 0;
    m_color2 = 12;
    m_color3 = 10;
    m_color4 = 11;
    break;
  }
}

void CStatePlay::NewGame(){
  m_totalValue = 40;
  m_level = 0;
  m_bYouLose = false;
  NewLevel();
}

void CStatePlay::Initialize(){

  srand((unsigned int)time(NULL));
  m_levelComplete = false;
  m_gameOver = false;
  m_bYouLose = false;
  m_firstTime = false;
}

CGameState* CStatePlay::Update(double timeDifference){
  m_event = GO_NO_WHERE;


  int p = 0; //used for snapping patterns to pallets
  int q = 0;
  //check user interface
  INPUT_RECORD InRec = GetConsoleInputInfo();
  static int nextShapeCount = 0;

  CLog *pLog = CLog::Instance();

  if(m_firstTime == false){
    m_firstTime = true;
    m_bYouLose = false;
    NewGame();
    if(g_audio.IsPlaying(5) == false)
      g_audio.PlaySoundClip(5, 0);
    return this;
  }

  if(m_bYouLose == true){
    for(int i = 1; i < 4; ++i){
      g_audio.StopSoundClip(i);
      if(g_audio.IsPlaying(0) == false)
        g_audio.PlaySoundClip(0, 0);
    }
  }


  if(m_levelComplete == true){
    if(g_audio.IsPlaying(8)==true)
      g_audio.StopSoundClip(8);
    NewLevel();
  }

  if(m_gameOver == true){
    for(int i = 1; i < 4; ++i)
      g_audio.StopSoundClip(i);

    g_totalValue = m_totalValue;
    m_event = GO_MENU;
    m_firstTime = false;
    m_gameOver = false;  

    //may be a new high score from previously played game
    if(g_totalValue > g_lowestScore){
      if(g_audio.IsPlaying(4) == false)
        g_audio.PlaySoundClip(4, 0);
      m_event = GO_SCORE;
    } 

  }

  if(InRec.EventType == KEY_EVENT){

    if(InRec.Event.KeyEvent.uChar.AsciiChar == VK_ESCAPE && m_bYouLose == false)
      m_event = GO_MENU;
    else if(InRec.Event.KeyEvent.uChar.AsciiChar == VK_RETURN && m_bYouLose == true){
      m_event = GO_MENU;
      m_firstTime = false;
    }

  }

  //game state transition code...very important
  for(unsigned int i=0; i< m_TE.size(); ++i){
    if (m_TE[i].event == m_event){
      return m_TE[i].p_gso;
    }
  }

  if(m_totalValue < 1){
    m_bYouLose = true;
  }



  //update assembly line
  if(timerAssemblyLine.GetTimer(0.05)==true && b_runAssemblyLine == true ){
    m_assemblyCount++;
    if(m_assemblyCount > 9)
      m_assemblyCount = 0;

    nextShapeCount++;//used to add 2nd part to assembly line

    if(m_bNextShapeExists == true){
      m_nextShape.SetRefPosition(m_nextShape.GetRefX() + 1, 1);
      if(m_nextShape.GetRefX() > 70){
        
        m_nextShape = m_pattern[GetRandomShape(m_level, RND_E, RND_M, RND_D)];// m_pattern[rand()% MAX_PARTS];
        m_nextShape.SetRefPosition(0,1);
        m_nextColor = GetRandomColor();
        nextShapeCount = 0;
      }
    }

    //move patterns on assembly line
    if(m_bPickUp == false){      
      m_currentShape.SetRefPosition(m_currentShape.GetRefX() + 1, 1);//move shape

      //assembly line part is now scrap
      if(m_currentShape.GetRefX() > 70 && m_bPickUp == false){//generate next part
        m_totalValue -= m_currentShape.GetPatternTileCount() * m_tableValue[m_level - 1].blockCost;
        m_numScrappedPatternCount++;
        m_numScrappedBlockCount += m_currentShape.GetPatternTileCount();
        m_currentShape = m_pattern[GetRandomShape(m_level, RND_E, RND_M, RND_D)];// m_pattern[rand()% MAX_PARTS];
        m_currentShape.SetRefPosition(0,1);
        m_currentColor = GetRandomColor();
        nextShapeCount = 0;
      }      
    }
    else{
      //add next shape 
      if(m_bNextShapeExists == false && nextShapeCount > 40){
        m_bNextShapeExists = true;
        nextShapeCount = 0;
        m_nextShape = m_pattern[GetRandomShape(m_level, RND_E, RND_M, RND_D)];
        m_nextShape.SetRefPosition(0, 1);
        m_nextColor = GetRandomColor();
      }
    }
  } //update assembly line 

  //key press events

  //mouse events
  if(InRec.EventType == MOUSE_EVENT){
    
    //left button click
    //******************************************************
    if(InRec.Event.MouseEvent.dwButtonState == 0x01){
      m_mouseX = InRec.Event.MouseEvent.dwMousePosition.X;
      m_mouseY = InRec.Event.MouseEvent.dwMousePosition.Y;

      //pick up part from assembly line..user has no parts
      if(m_bPickUp == false){  
        if(m_mouseY > 0 && m_mouseY <  10){
          if(m_mouseX > 5 && m_mouseX < 74){          
            m_currentShape.SetRefPosition(m_mouseX, m_mouseY);
            m_bPickUp = true;
          }
        }
      }

      //user has part
      else if(m_bPickUp == true){

        //try placing part on a pallet 
        if(m_location > 2){

          //pattern matches and color is true
          if(IsColorLegal() == true){
            if(IsPatternMatch()== true){
              if(m_location == LOC_PALLET1 && m_pallet1 == 65535 && m_filled1 == false){
                m_filled1 = true;
                m_totalValue += m_tableValue[m_level - 1].pallet1;
                m_palletCount++;
                if(g_audio.IsPlaying(8) == false)
                  g_audio.PlaySoundClip(8, 0);
              }
              else if(m_location == LOC_PALLET2 && m_pallet2 == 65535 && m_filled2 == false){
                m_filled2 = true;
                m_totalValue += m_tableValue[m_level - 1].pallet2;
                m_palletCount++;
                if(g_audio.IsPlaying(8) == false)
                  g_audio.PlaySoundClip(8, 0);
              }
              else if(m_location == LOC_PALLET3 && m_pallet3 == 65535 && m_filled3 == false){
                m_filled3 = true;
                m_totalValue += m_tableValue[m_level - 1].pallet3;
                m_palletCount++;
                if(g_audio.IsPlaying(8) == false)
                  g_audio.PlaySoundClip(8, 0);
              }
              else if(m_location == LOC_PALLET4 && m_pallet4 == 65535 && m_filled4 == false){
                m_filled4 = true;
                m_totalValue += m_tableValue[m_level - 1].pallet4;
                m_palletCount++;
                if(g_audio.IsPlaying(8) == false)
                  g_audio.PlaySoundClip(8, 0);
              }
              if(m_palletCount == 4)
                m_levelComplete = true;

              m_bPickUp = false;
              if(m_bNextShapeExists == false){
                pLog->Log("Next pattern exists ", m_location);
                m_currentShape = m_pattern[GetRandomShape(m_level, RND_E, RND_M, RND_D)];
                m_currentShape.SetRefPosition(0,1);
                m_currentColor = GetRandomColor();
              }
              else{
                pLog->Log("Need new pattern ", m_location);
                m_currentShape = m_nextShape;
                m_currentColor = m_nextColor;
                m_bNextShapeExists = false;
              }              
            }
          }
          else{
            pLog->Log("Oops!!");  
          }
        }
        //place pattern onto scrap and put another on the assembly line
        else if(m_location == 2 & m_bPickUp == true){
          m_totalValue -= m_currentShape.GetPatternTileCount() * m_tableValue[m_level - 1].blockCost;
          m_numScrappedPatternCount++;  //add another scapped pattern count
          m_numScrappedBlockCount += m_currentShape.GetPatternTileCount();
          m_bPickUp = false;
          if(m_bNextShapeExists == false){
            pLog->Log("Scrap new exists");
            m_currentShape = m_pattern[GetRandomShape(m_level, RND_E, RND_M, RND_D)];// m_pattern[rand()% MAX_PARTS];//rand()% 4];
            m_currentShape.SetRefPosition(0,1);
            m_currentColor = GetRandomColor();
          }
          else{
            pLog->Log("Scrap new does not exist");
            m_currentShape = m_nextShape;
            m_currentColor = m_nextColor;
            m_bNextShapeExists = false;
          }              
        }
      }
    }

    //rotate clockwise with mouse right button
    //*******************************************************
    else if(InRec.Event.MouseEvent.dwButtonState == 0x02){
      if(m_bPickUp == true){
        m_currentShape.RotateAngleCW();
        m_currentShape.SetRefPosition(m_mouseX, m_mouseY);
      }
    }
    //update mouse when mouse is moved
    //******************************************************
    else if(InRec.Event.MouseEvent.dwEventFlags == MOUSE_MOVED){
      m_mouseX = InRec.Event.MouseEvent.dwMousePosition.X;
      m_mouseY = InRec.Event.MouseEvent.dwMousePosition.Y;
      if(m_bPickUp == true){
        m_currentShape.SetRefPosition(m_mouseX, m_mouseY);
      }
      
      //determine general location
      m_location = LOC_NOWHERE;
      if(m_currentShape.GetRefY() > 12 && m_currentShape.GetRefY() < 21 && m_bPickUp == true){
        
        //pallet 1
        if(m_currentShape.GetRefX() > 20 && m_currentShape.GetRefX() < 33){
          m_location = LOC_PALLET1;
          m_currentShape.SetRefPosition(20 + GetSnapX(20, m_currentShape.GetRefX()),
                                        12 + GetSnapY(12, m_currentShape.GetRefY()));
        }

        //pallet 2
        else if(m_currentShape.GetRefX() > 33 && m_currentShape.GetRefX() < 46 && m_bPickUp == true){
          m_location = LOC_PALLET2;
          m_currentShape.SetRefPosition(33 + GetSnapX(33, m_currentShape.GetRefX()),
                                        12 + GetSnapY(12, m_currentShape.GetRefY()));
        }

        //pallet 3
        else if(m_currentShape.GetRefX() > 46 && m_currentShape.GetRefX() < 59 && m_bPickUp == true){
          m_location = LOC_PALLET3;
          m_currentShape.SetRefPosition(46 + GetSnapX(46, m_currentShape.GetRefX()),
                                        12 + GetSnapY(12, m_currentShape.GetRefY()));
        }

        //pallet 4
        else if(m_currentShape.GetRefX() > 59 && m_currentShape.GetRefX() < 72 && m_bPickUp == true){
          m_location = LOC_PALLET4;
          m_currentShape.SetRefPosition(59 + GetSnapX(59, m_currentShape.GetRefX()),
                                        12 + GetSnapY(12, m_currentShape.GetRefY()));
        }

        //scrap bin
        else if(m_currentShape.GetRefX() > 4 && m_currentShape.GetRefX() < 17 && m_bPickUp == true)
          m_location = LOC_SCRAP;
          m_currentShape.SetRefPosition(4 + GetSnapX(4, m_currentShape.GetRefX()),
                                        12 + GetSnapY(12, m_currentShape.GetRefY()));
      }
    }
    //else{
    //}
  }

  if(g_audio.IsPlaying(6) == true)//burns' speech
    g_audio.StopSoundClip(6);

  if(m_level == 1 && g_audio.IsPlaying(1) == false && g_audio.IsPlaying(5) == false && m_bYouLose == false){
    g_audio.PlaySoundClip(1, 1);
    g_audio.StopSoundClip(2);
    g_audio.StopSoundClip(3);
  }
  else if(m_level == 2 && g_audio.IsPlaying(2) == false && m_bYouLose == false){
    g_audio.PlaySoundClip(2, 1);
    g_audio.StopSoundClip(1);
    g_audio.StopSoundClip(3);
  }
  else if(m_level == 3 && g_audio.IsPlaying(3) == false && m_bYouLose == false){
    g_audio.PlaySoundClip(3, 1);
    g_audio.StopSoundClip(1);
    g_audio.StopSoundClip(2);
  }
  else if(m_level == 4 && g_audio.IsPlaying(1) == false && m_bYouLose == false){
    g_audio.PlaySoundClip(1, 1);
    g_audio.StopSoundClip(2);
    g_audio.StopSoundClip(3);
  }


  return 0;
}

void CStatePlay::Render(Console& con){

  static int frameCount = 0;
  con.setBackgroundColor(0);
  con.cls();
  con.setColor(15);

  //verify valid pattern file
  if(m_bValidPatternsFile == false){
    con.write(10, 10, "patterns.dat file does not exist!");
    con.write(10, 12, "Press ESC and correct problem!");
    con.flip();
    return;
  }

  //display framerate to console title
  frameCount++;
  char buffer[256];
  if(m_TimerFrameRate.GetTimer(1.0)){
    ::sprintf(buffer,"Game Challenge 5 (Chuck's Factory Game) %d FPS", frameCount);
    con.setTitle(buffer);
    frameCount=0;
  }

  if(m_gameOver == true){
    con.setColor(15);
    con.write(34, 13, "Game Over");
    con.flip();
    return;
  }

  if(m_bYouLose == true){
    con.setColor(15);
    con.write(26, 12, "Plant shutdown...you lose!");
    con.write(26, 14, "Press ENTER to continue...");
    con.flip();
    return;
  }

  #ifdef DEBUG_PLAY 
    con.setColor(15);
    con.write(0, 26, "Shape: ");
    con.setColor(14);
    con.write(8, 26, m_index);
    con.setColor(15);
    con.write(0, 27, "Angle: ");
    con.setColor(14);
    con.write(8, 27, m_pattern[m_index].GetAngle());
    con.write(1,23,m_shape.GetAngle());
    con.write(1,24, (long)m_shape.GetPatternValue(m_shape.GetAngle()));
    con.setColor(8);
    con.write(0,28, "Press Left and Right arrow keys to rotate");
    con.write(0,29, "Press Up and Down arrow keys to change pattern");
    con.write(0,30, "Move mouse to move pattern");
    con.write(0,31, "Press ESC to quit");

    std::ostringstream oss;
    oss << "Pattern: " << std::hex << m_pattern[m_index].GetPatternValue(m_pattern[m_index].GetAngle());
    con.write(0,32,oss.str());

    con.write(30, 49, m_mouseX);
    con.write(40, 49, m_mouseY);
  #endif

  //con.ClearScreenArray(); 

  DrawAssemblyLine(con);
  DrawPallets(con);
  DrawScrap(con);
  //draw current shape
  STRUCT_TILE block;
  int m = 0;
  int n = 0;

  //draw pallet1
  con.setColor(5);
  con.setBackgroundColor(0);
  for(int i = 15; i >= 0; --i){
    if(IsBlockExists(i, m_pallet1)== true){
      m = 3 - (i % 4);
      n = 3 - (i / 4);
      for(int j = 0; j < 6; ++j){
        if(j < 3)
          con.writeAscii(21 + (m * 3) + j, 13 + (n * 2), (int)219);
        else
          con.writeAscii(21 + (m * 3) + j - 3, 13 + (n * 2) + 1, (int)219);
      }
    }
  }

  //draw pallet2
  con.setColor(4);
  con.setBackgroundColor(0);
  for(int i = 15; i >= 0; --i){
    if(IsBlockExists(i, m_pallet2)== true){
      m = 3 - (i % 4);
      n = 3 - (i / 4);
      for(int j = 0; j < 6; ++j){
        if(j < 3)
          con.writeAscii(34 + (m * 3) + j, 13 + (n * 2), (int)219);
        else
          con.writeAscii(34 + (m * 3) + j - 3, 13 + (n * 2) + 1, (int)219);
      }
    }
  }

  //draw pallet3
  con.setColor(2);
  con.setBackgroundColor(0);
  for(int i = 15; i >= 0; --i){
    if(IsBlockExists(i, m_pallet3)== true){
      m = 3 - (i % 4);
      n = 3 - (i / 4);
      for(int j = 0; j < 6; ++j){
        if(j < 3)
          con.writeAscii(47 + (m * 3) + j, 13 + (n * 2), (int)219);
        else
          con.writeAscii(47 + (m * 3) + j - 3, 13 + (n * 2) + 1, (int)219);
      }
    }
  }

  //draw pallet4
  con.setColor(1);
  con.setBackgroundColor(0);
  for(int i = 15; i >= 0; --i){
    if(IsBlockExists(i, m_pallet4)== true){
      m = 3 - (i % 4);
      n = 3 - (i / 4);
      for(int j = 0; j < 6; ++j){
        if(j < 3)
          con.writeAscii(60 + (m * 3) + j, 13 + (n * 2), (int)219);
        else
          con.writeAscii(60 + (m * 3) + j - 3, 13 + (n * 2) + 1, (int)219);
      }
    }
  }
  
  for(int i = 15; i >= 0; --i){
    if(m_currentShape.PatternBlockExists(i)){
      m = 3 - (i % 4);
      n = 3 - (i / 4);
      for(int j = 0; j < 6; ++j){
        block = m_currentShape.GetBlockTile(i, j + 1);

        con.setColor(m_currentColor);
        con.setBackgroundColor(m_currentColor);
        
        if(j < 3)
          con.writeAscii(m_currentShape.GetRefX() + (m * 3) + j, m_currentShape.GetRefY() + (n * 2), (int)block.ascii);
        else
          con.writeAscii(m_currentShape.GetRefX() + (m * 3) + j - 3, m_currentShape.GetRefY() + (n * 2) + 1, (int)block.ascii);
      }
    }
  }

  //draw next shape if it exists
  if(m_bNextShapeExists == true){
    for(int i = 15; i >= 0; --i){
      if(m_nextShape.PatternBlockExists(i)){
        m = 3 - (i % 4);
        n = 3 - (i / 4);
        for(int j = 0; j < 6; ++j){
          block = m_nextShape.GetBlockTile(i, j + 1);
          con.setColor(m_nextColor);
          con.setBackgroundColor(m_nextColor);
          
          if(j < 3)
            con.writeAscii(m_nextShape.GetRefX() + (m * 3) + j, m_nextShape.GetRefY() + (n * 2), (int)block.ascii);
          else
            con.writeAscii(m_nextShape.GetRefX() + (m * 3) + j - 3, m_nextShape.GetRefY() + (n * 2) + 1, (int)block.ascii);
        }
      }
    }
  }

  //draw stuff that overlays other things
  DrawAssemblyLineOverlay(con);
  con.setColor(15);
  con.setBackgroundColor(0);
 
  //important game data
  con.write(25, 23, "Production Score: $");
  con.setColor(10);
  con.write(44, 23, m_totalValue);
  con.setColor(14);
  con.write(5, 23, "Level: ");
  con.setColor(10);
  con.write(13, 23, m_level);
  con.setColor(11);
  con.write(55, 23, "Press <ESC> to Quit!");

  //bug fix 
  con.setColor(11);
  con.setBackgroundColor(0);
  con.writeAscii(80, 24, 32);
  con.flip();
}

void CStatePlay::AddTransitionEvent(int event, CGameState* p_Next){
  TRANSITION_EVENT te;
  te.event=event;
  te.p_gso = p_Next;

  m_TE.push_back(te);
}

//game specific functions
void CStatePlay::DrawAssemblyLine(Console& con){

  //draws side of line
  con.writeAsciiLine(0,0,205,80);
  con.writeAsciiLine(0,9,205,80);

  con.setColor(8);
  //draws animated vertical lines...simulating motion
  for(int i = 0; i < 8; ++i){
    for(int j = 1; j < 9; ++j){
      con.writeAscii((i * 10) + m_assemblyCount, j, 197);
    }
  }
}

void CStatePlay::DrawAssemblyLineOverlay(Console& con){
  con.setColor(7);
  con.setBackgroundColor(0);

  for(int j = 0; j < 8; ++j){
    con.writeAsciiLine(0, 1 + j, 178,6);//left side
    con.writeAsciiLine(74,1 + j, 177,6);//right side
  }
}

void CStatePlay::DrawPallets(Console& con){
  int x = 20;
  int y = 12;
  con.setColor(15);

  //draw top and bottom lines
  con.writeAsciiLine(x + 1, y, 196, 51);
  con.writeAsciiLine(x + 1, y + 9, 196, 51);
  
  //draw vertical lines
  for(int j = y + 1; j < y + 9; ++j){
    con.writeAscii(x, j, 179);
    con.writeAscii(x + 13, j, 179);
    con.writeAscii(x + 26, j, 179);
    con.writeAscii(x + 39, j, 179);
    con.writeAscii(x + 52, j, 179);
  }

  //add corners and joining pieces
  con.writeAscii(x, y, 218);
  con.writeAscii(x + 13, y, 194);
  con.writeAscii(x + 26, y, 194);
  con.writeAscii(x + 39, y, 194);
  con.writeAscii(x + 52, y, 191);

  con.writeAscii(x, y + 9, 192);
  con.writeAscii(x + 13, y + 9, 193);
  con.writeAscii(x + 26, y + 9, 193);
  con.writeAscii(x + 39, y + 9, 193);
  con.writeAscii(x + 52, y + 9, 217);

  //label pallets
  con.setColor(14);
  con.write(x + 5, y - 1, m_type1);
  if(m_color2 > 0)
    con.setColor(m_color2);
  else
    con.setColor(14);
  con.write(x + 18, y - 1, m_type2);

  if(m_color3 > 0)
    con.setColor(m_color3);
  else
    con.setColor(14);
  con.write(x + 31, y - 1, m_type3);

  if(m_color4 > 0)
    con.setColor(m_color4);
  else
    con.setColor(14);
  con.write(x + 44, y - 1, m_type4);

}

void CStatePlay::DrawScrap(Console& con){
  int x = 4;
  int y = 12;
  con.setColor(4);
  con.setBackgroundColor(0);

  //draw top and bottom lines
  con.writeAsciiLine(x + 1, y, 196, 12);
  con.writeAsciiLine(x + 1, y + 9, 196, 12);
  
  //draw vertical lines
  for(int j = y + 1; j < y + 9; ++j){
    con.writeAscii(x, j, 179);
    con.writeAscii(x + 13, j, 179);
  }

  //draw corners
  con.writeAscii(x,y, 218);
  con.writeAscii(x + 13, y, 191);
  con.writeAscii(x,y + 9, 192);
  con.writeAscii(x + 13, y + 9, 217);

  //draw filler
  //con.setColor(4);
  //for(int j = y + 1; j < y + 9; ++j){
  //  con.writeAsciiLine(x + 1, j, 219, 12);
  //}

  con.setColor(14);
  con.setBackgroundColor(0);
  con.write(x  + 4, y + 4, "Scrap");
  
}

int CStatePlay::GetSnapX(int x, int offset){
  int p = offset - x;
  if(p < 4)
    p = 1;
  else if(p < 7)
    p = 4;
  else if(p < 10)
    p = 7;
  else if(p < 13)
    p = 10;

  return p;
}

int CStatePlay::GetSnapY(int y, int offset){
  int q = offset - y;

  if(q < 3)
    q = 1;
  else if(q < 5)
    q = 3;
  else if(q < 7)
    q = 5;
  else if(q < 9)
    q = 7;

  return q;
}

//1) determine current pattern
//2) shift to adjust to 4x4 pallet
//3) will pattern fit
//4) if no return false
//5) if yes update m_palletx...return true
//m_pallet1 = m_pallet1 | ((0xFFFF & pattern) >> (15 - bit));
bool CStatePlay::IsPatternMatch(){
  
  unsigned int pattern = m_currentShape.GetCurrentPattern();
  unsigned int bit = 0;
  unsigned int pallet = 0;
  unsigned int test = 0;
  int r = 0;
  int c = 0;
  int refX = m_currentShape.GetRefX();
  int refY = m_currentShape.GetRefY();
  CLog *pLog = CLog::Instance();

  if(m_location == LOC_PALLET1){
    c = m_mouseX;
    r = m_mouseY;
    bit = GetBit(r - 12, c - 20);
    pallet = m_pallet1;
    test = ((0xFFFF & pattern) >> (15 - bit));
    if((~pallet & test) == test ){
      m_pallet1 = m_pallet1 | test;
      return true;
    }
    return false;    
  }
  if(m_location == LOC_PALLET2){
    c = m_mouseX;
    r = m_mouseY;
    bit = GetBit(r - 12, c - 33);
    pallet = m_pallet2;
    test = ((0xFFFF & pattern) >> (15 - bit));
    if((~pallet & test) == test ){
      m_pallet2 = m_pallet2 | test;
      return true;
    }
    return false;    
  }
  if(m_location == LOC_PALLET3){
    c = m_mouseX;
    r = m_mouseY;
    bit = GetBit(r - 12, c - 46);
    pallet = m_pallet3;
    test = ((0xFFFF & pattern) >> (15 - bit));
    if((~pallet & test) == test ){
      m_pallet3 = m_pallet3 | test;
      return true;
    }
    return false;    
  }
  if(m_location == LOC_PALLET4){
    c = m_mouseX;
    r = m_mouseY;
    bit = GetBit(r - 12, c - 59);
    pallet = m_pallet4;
    test = ((0xFFFF & pattern) >> (15 - bit));
    if((~pallet & test) == test ){
      m_pallet4 = m_pallet4 | test;
      return true;
    }
    return false;    
  }

  return false;
}

bool CStatePlay::IsBlockExists(int block, unsigned int value){
  return (value &  (int)powf(2, (float)block));
}

//converts r,c of 4x4 pallet to a bit
int CStatePlay::GetBit(int r, int c){
  int x = 0;
  int y = 0;
  int bit = 0;

  if(c < 7)
    x = (c/4) + 1;
  else
    x = ((c-6)/4) + 3;

  if(r < 5)
    y = (r/3) ;
  else
    y = ((r - 4)/3) + 2;

  bit = 16 - x - ( y * 4);
  
  return bit;
}

int CStatePlay::GetRandomShape(int level, int nEasy, int nModerate, int nDifficult){
  //default level if out of range
  if(level < 1 || level > 4)
    level = 1;
  //default difficult if not 100    
  if((nEasy + nModerate + nDifficult) < 95|| (nEasy + nModerate + nDifficult) > 105){
    nEasy = 50;
    nModerate = 30;
    nDifficult = 20;
  }    

  int ref = 0;
  int value = rand() % 100;
  
  if(value <= nEasy)
    ref = 0;
  else if(value <= nEasy + nModerate)
    ref = 7;
  else if(value <= nEasy + nModerate + nDifficult)
    ref = 14;

  return (ref +  rand()% (3 + level));
    
}

int CStatePlay::GetRandomColor(){
 return (10 + rand() % 3); 
}

void CStatePlay::LoadPatterns(){
/*
  //initialize pattersn
  for(int i = 0; i < MAX_PARTS; ++i){
    m_pattern[i].SetPatternValue(0, 0x4E00);
    m_pattern[i].SetPatternValue(1, 0x8C80);
    m_pattern[i].SetPatternValue(2, 0xE400);
    m_pattern[i].SetPatternValue(3, 0x4C40);
  } */

  //read file
  CFileReader cfr("patterns.dat");
  std::string sValue;
  int index = 0;
  int angle = 0;
  unsigned int pattern = 0;

  CLog *pLog = CLog::Instance();

  pLog->Log("Lines in file", cfr.GetNumberOfLines());
  
  for(int i = 0; i < cfr.GetNumberOfLines(); ++i){
    
    sValue = cfr.GetTerm(cfr.GetLineFromFile(i), 1);
    index = atoi(sValue.c_str());
    
    sValue = cfr.GetTerm(cfr.GetLineFromFile(i), 2);
    angle = atoi(sValue.c_str());
    
    sValue = cfr.GetTerm(cfr.GetLineFromFile(i), 3);
    pattern = ConvertHexStringToDecimal(sValue);

    m_pattern[index].SetPatternValue(angle, pattern);
    //pLog->LogTriplet(index, angle, sValue, pattern);
  }
/*
  for(int i = 0; i < MAX_PARTS; ++i){
    pLog->LogPair(i, 0, m_pattern[i].GetPatternValue(0));
    pLog->LogPair(i, 1, m_pattern[i].GetPatternValue(1));
    pLog->LogPair(i, 2, m_pattern[i].GetPatternValue(2));
    pLog->LogPair(i, 3, m_pattern[i].GetPatternValue(3));
  }
*/
  if(cfr.IsValid() == false){
    m_bValidPatternsFile = false;
    return;
  }
  else
    m_bValidPatternsFile = true;

}

bool CStatePlay::IsColorLegal(){//int currentColor, int palletColor){
  if(m_location == LOC_PALLET1){
    if(m_color1 == 0)//any color is okay
      return true;
    else{            //return true if color matches
      if(m_color1 == m_currentColor)
        return true;
      else           //color doesn't match
        return false;
    }
  }
  else if(m_location == LOC_PALLET2){
    if(m_color2 == 0)//any color is okay
      return true;
    else{            //return true if color matches
      if(m_color2 == m_currentColor)
        return true;
      else           //color doesn't match
        return false;
    }
  }
  else if(m_location == LOC_PALLET3){
    if(m_color3 == 0)//any color is okay
      return true;
    else{            //return true if color matches
      if(m_color3 == m_currentColor)
        return true;
      else           //color doesn't match
        return false;
    }
  }
  else if(m_location == LOC_PALLET4){
    if(m_color4 == 0)//any color is okay
      return true;
    else{            //return true if color matches
      if(m_color4 == m_currentColor)
        return true;
      else           //color doesn't match
        return false;
    }
  }
  return false;
}

int CStatePlay::GetHexStringCharToDecimal(std::string ch){
  if(ch == "0")
    return 0;
  else if(ch == "1")
    return 1;
  else if(ch == "2")
    return 2;
  else if(ch == "3")
    return 3;
  else if(ch == "4")
    return 4;
  else if(ch == "5")
    return 5;
  else if(ch == "6")
    return 6;
  else if(ch == "7")
    return 7;
  else if(ch == "8")
    return 8;
  else if(ch == "9")
    return 9;
  else if(ch == "A")
    return 10;
  else if(ch == "B")
    return 11;
  else if(ch == "C")
    return 12;
  else if(ch == "D")
    return 13;
  else if(ch == "E")
    return 14;
  else if(ch == "F")
    return 15;
  else
    return 0;  
}

unsigned int CStatePlay::ConvertHexStringToDecimal(std::string sValue){
  int pos = sValue.find("0x");
  unsigned int dec = 0;
  std::string ch;
  char c;
  unsigned int multiplier;
  
  if(pos > -1)
    sValue = sValue.substr(2, sValue.size() - 2);
  else
    return 0;
    
  for(int i = sValue.size() - 1; i >= 0; --i){
    ch = sValue.substr(i, 1);
    //cout << i << ": " << ch << " : " << (unsigned int)(powf(16, (float)(sValue.size() - i - 1))) <<endl;
    multiplier = (unsigned int)GetHexStringCharToDecimal(ch);
    dec += (unsigned int)(powf(16, (float)(sValue.size() - i - 1))) * multiplier;          
  }
            
  return dec;          
}
