//CPattern.cpp
#include "..\includes\CPattern.h"

CPattern::CPattern(){
  timerRotate.Initialize();
  CBlock temp;
  temp.DescribeBlock(0, 177, 3,0);

  for(int i = 0; i < 16; ++i){
    m_pattern.push_back(temp);
  }
  m_angle = 0;
}

//void CPattern::void SetPatternColor(int color){}

void CPattern::SetPatternValue(int angle, unsigned long value){
  if(angle < 0 || angle > 3)
    return;

    m_value[angle] = value;
}

unsigned long CPattern::GetPatternValue(int angle){
  if(angle < 0 || angle > 3)
   return 0;
  else
    return m_value[angle];
}

int CPattern::GetPatternTileCount(){
  unsigned int pattern = GetPatternValue(m_angle);
  unsigned int word = 0;
  int count = 0;

  bool first = false;
  word = 1;
  for(int i = 0; i < 16; ++i){
    if(first == false)
      first = true;
    else  
      word *= 2;
     
    if((bool)(word & pattern)== true)
      count ++;
  }

  return count;
}

void CPattern::SetRefPosition(int x, int y){
  m_refX = x;
  m_refY = y;
}

void CPattern::DescribeBlock(int block, int tile, unsigned char ascii, char foreColor, char backColor){
  if(block < 0 || block > 15)
    return;
  if(tile < 0 || tile > 6)
    return;  
  if(ascii < 32 || ascii > 255)
    return;
  if(foreColor < 0 || foreColor > 15)
    return;
  if(backColor < 0 || backColor > 15)
    return;
  
  CBlock temp;
  temp.DescribeBlock(tile, ascii, foreColor, backColor);
  m_pattern[block] = temp;
}

STRUCT_TILE CPattern::GetBlockTile(int block, int tile){
  STRUCT_TILE temp;
 
  if(block < 0 || block > 15)
    return temp;
  if(tile < 0 || tile > 6)
    return temp;

  return m_pattern[block].GetBlockTile(tile);
}

void CPattern::SetAngle(int angle){
  if(angle < 0 || angle > 3)
    return;
  m_angle = angle;
}

void CPattern::RotateAngleCW(){
  if(timerRotate.GetTimer(0.2)==true){
    m_angle++;
    if(m_angle > 3)
      m_angle = 0;
  }
}

void CPattern::RotateAngleCCW(){
  if(timerRotate.GetTimer(0.2) == true){
    m_angle--;
    if(m_angle < 0)
      m_angle = 3;
  }
}

//examines 16 bit int and checks to see if block is true
bool CPattern::PatternBlockExists(int block){
  unsigned int value = m_value[m_angle];

  return (value &  (int)powf(2, (float)block));
}

//return true if x,y is inside pattern
bool CPattern::IsInsidePattern(int x, int y){
  return true;

}

unsigned long CPattern::GetCurrentPattern(){
  return GetPatternValue(m_angle);
}