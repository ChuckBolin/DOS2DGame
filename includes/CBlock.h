//CBlock.h
/*
Block consists of 6 tiles, 3 in first row and three in second row.
Block:  1  2  3
        4  5  6
        0 = all 6..used for describing blocks.
*/
#ifndef CBLOCK_H
#define CBLOCK_H

#include <vector>

struct STRUCT_TILE{//6 tiles to a block
  unsigned char ascii;
  char foreColor;
  char backColor;
};

class CBlock{
public:
  CBlock();
  void DescribeBlock(int tile, unsigned char ascii, char foreColor, char backColor);
  STRUCT_TILE GetBlockTile(int tile); //1 - 6

private:
  std::vector<STRUCT_TILE> m_block;
};

#endif