#include "..\includes\CStateQuit.h"

CStateQuit::CStateQuit(){

}

CStateQuit::~CStateQuit(){

}

void CStateQuit::Initialize(){

}

CGameState* CStateQuit::Update(double timeDifference){
  m_event = GO_NO_WHERE;

  //game state transition code...very important
  for(unsigned int i=0; i< m_TE.size(); ++i){
    if (m_TE[i].event == m_event){
      return m_TE[i].p_gso;
    }
  }


  return 0;
}

void CStateQuit::Render(Console& con){
  con.setBackgroundColor(0);
  con.cls();
  con.setColor(15);
 
  for(int i = 0; i < 80; ++i){
    con.write(i, 0, "*");
    con.write(i, 24, "*");
  }
  for(int i = 1; i < 24; ++i){
    con.write(0, i, "*");
    con.write(79, i, "*");
  }

  con.setColor(14);
  con.write(31,11,"Thanks for Playing");
  con.flip();

}

void CStateQuit::AddTransitionEvent(int event, CGameState* p_Next){
  TRANSITION_EVENT te;
  te.event=event;
  te.p_gso = p_Next;

  m_TE.push_back(te);
}
