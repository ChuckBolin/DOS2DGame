//CBlock.cpp
#include "..\includes\CBlock.h"
/*
struct STRUCT_TILE{//6 tiles to a block
  char ascii;
  char foreColor;
  char backColor;
};*/

//initialize the block with six white solid ascii characters (219) on black background
CBlock::CBlock(){
  STRUCT_TILE temp;
  temp.ascii = 219;
  temp.foreColor = 7;
  temp.backColor = 0;

  for(int i = 0; i< 6; ++i)
    m_block.push_back(temp);
}

/*
Block:  1  2  3
        4  5  6
*/
void CBlock::DescribeBlock(int tile, unsigned char ascii, char foreColor, char backColor){
  //illegal values
  if(tile < 0 || tile > 6)
    return;  
  if(ascii < 32 || ascii > 255)
    return;
  if(foreColor < 0 || foreColor > 15)
    return;
  if(backColor < 0 || backColor > 15)
    return;

  //set specific tile (1 - 6)
  if(tile > 0){
    m_block[tile - 1].ascii = ascii;
    m_block[tile - 1].foreColor = foreColor;
    m_block[tile - 1].backColor = backColor;
  }
  //set all tiles
  else if(tile == 0){
    for(int i = 0; i < 6;++i){
      m_block[i].ascii = ascii;
      m_block[i].foreColor = foreColor;
      m_block[i].backColor = backColor;
    }
  }
}

STRUCT_TILE CBlock::GetBlockTile(int tile){
  STRUCT_TILE temp;
  temp.ascii = 219;
  temp.foreColor = 7;
  temp.backColor = 0;

  if(tile < 0 || tile > 6)
    return temp;
  else
    return m_block[tile - 1];
}

