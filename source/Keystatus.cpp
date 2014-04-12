//Keystatus.cpp
#include "..\includes\Keystatus.h"

bool keyDown(int key)
{
  if(GetAsyncKeyState(key) == 0) //key is not pressed, return false
    return false;
  else                           //key IS pressed, return true 
    return true;
}

INPUT_RECORD GetConsoleInputInfo(){
  INPUT_RECORD InRec;
  DWORD NumRead;
  DWORD EventCount;
  HANDLE hIn = GetStdHandle(STD_INPUT_HANDLE);

  GetNumberOfConsoleInputEvents(hIn, &EventCount);
  ReadConsoleInput(hIn,&InRec,1,&NumRead);
  return InRec;
}