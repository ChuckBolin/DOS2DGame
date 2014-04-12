//CPattern.h

#ifndef CPATTERN_H
#define CPATTERN_H

#include <vector>
#include <cmath>
#include "CBlock.h"
#include "CTimer.h"

class CPattern{
public:
  CPattern(); 
  void SetPatternValue(int angle, unsigned long value);
  unsigned long GetPatternValue(int angle);
  int GetPatternTileCount();
  void DescribeBlock(int block, int tile, unsigned char ascii, char foreColor, char backColor);
  STRUCT_TILE GetBlockTile(int block, int tile); //1 - 6
  void SetAngle(int angle);
  int GetAngle()const{return m_angle;}
  void RotateAngleCW();
  void RotateAngleCCW();
  bool PatternBlockExists(int block);
  void SetRefPosition(int x, int y);
  int GetRefX(){return m_refX;}
  int GetRefY(){return m_refY;}
  bool IsInsidePattern(int x, int y);
  unsigned long GetCurrentPattern();
 // void SetPatternColor(int color); //fore and background color

private:
  std::vector<CBlock> m_pattern;
  unsigned int m_value[4]; //stores 0, 1, 2, 3
  int m_angle;
  CTimer timerRotate;
  int m_refX;
  int m_refY;

};

#endif;