#include <string>
#include <vector>


class CGame{
public:
  CGame();
  void SetPosition(int x, int y);
  void SetLevelBoundaries(int left, int right, int top, int bottom);
  int GetPositionX();
  int GetPositionY();
  void MoveLeft(int step = 1);
  void MoveRight(int step = 1);
  void MoveUp(int step = 1);
  void MoveDown(int step = 1);
   
private:
  int m_positionX; //X,Y position of game, it's center of focus
  int m_positionY;
  int m_leftLevelLimit; //defines world boundaries
  int m_rightLevelLimit;
  int m_topLevelLimit;
  int m_bottomLevelLimit;

};