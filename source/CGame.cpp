#include "..\includes\CGame.h"

CGame::CGame(){

}

void CGame::SetPosition(int x, int y){
  m_positionX = 10;//2560;  //default to center of map
  m_positionY = 10;//2560;

  if(x < m_leftLevelLimit || x > m_rightLevelLimit)
    return;
  if(y < m_topLevelLimit || y > m_bottomLevelLimit)
    return;
  
  m_positionX = x;
  m_positionY = y;
}

void CGame::SetLevelBoundaries(int left, int right, int top, int bottom){
  m_leftLevelLimit = left;
  m_rightLevelLimit = right;
  m_topLevelLimit = top;
  m_bottomLevelLimit = bottom;
}

int CGame::GetPositionX(){
  return m_positionX;
}

int CGame::GetPositionY(){
  return m_positionY;
}

void CGame::MoveLeft(int step){
  m_positionX -= step;

  if(m_positionX < m_leftLevelLimit)
    m_positionX = m_leftLevelLimit;
}

void CGame::MoveRight(int step){
  m_positionX += step;

  if(m_positionX > m_rightLevelLimit)
    m_positionX = m_rightLevelLimit;
}

void CGame::MoveUp(int step){
  m_positionY -= step;

  if(m_positionY < m_topLevelLimit)
    m_positionY = m_topLevelLimit;
}

void CGame::MoveDown(int step){
  m_positionY += step;

  if(m_positionY > m_bottomLevelLimit)
    m_positionY = m_bottomLevelLimit;
}
