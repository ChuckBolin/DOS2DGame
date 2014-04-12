//Console.cpp
#include "..\includes\Console.h"

//constructor...initialized object
Console::Console(int width, int height)
{
	mBackColor = 0;			//black
	mForeColor = 7;			//white
	mScreenNumber = 1;	//first page to write to is hScreen1
  //fullScreen(); //removed from this game
	hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
  hScreen0 = CreateConsoleScreenBuffer(GENERIC_READ |GENERIC_WRITE,0,NULL,CONSOLE_TEXTMODE_BUFFER,NULL);
	hScreen1 = CreateConsoleScreenBuffer(GENERIC_READ |GENERIC_WRITE,0,NULL,CONSOLE_TEXTMODE_BUFFER,NULL);
	hScreen2 = CreateConsoleScreenBuffer(GENERIC_READ |GENERIC_WRITE,0,NULL,CONSOLE_TEXTMODE_BUFFER,NULL);

  SetConsoleSize(width, height);

 ::SetConsoleActiveScreenBuffer(hScreen0);
 
 CONSOLE_CURSOR_INFO cci;
 cci.bVisible = false;
 cci.dwSize= 100;

 ::SetConsoleCursorInfo(hScreen1, &cci);
 ::SetConsoleCursorInfo(hScreen2, &cci);

 CONSOLE_SCREEN_BUFFER_INFO cbsi = GetScreenBufferInfo();
 m_screenSize = cbsi.dwSize;
 m_cursorPosition = cbsi.dwCursorPosition;
 m_maximumSize.X = cbsi.dwMaximumWindowSize.X;
 m_maximumSize.Y = cbsi.dwMaximumWindowSize.Y;


}

void Console::fullScreen(void){
  typedef BOOL (WINAPI *PROCSETCONSOLEDISPLAYMODE)(HANDLE,DWORD,LPDWORD);
  typedef BOOL (WINAPI *PROCGETCONSOLEDISPLAYMODE)(LPDWORD);
  PROCSETCONSOLEDISPLAYMODE SetConsoleDisplayMode;
  PROCGETCONSOLEDISPLAYMODE GetConsoleDisplayMode;

  HMODULE hKernel32 = GetModuleHandleA("kernel32");//add A
  SetConsoleDisplayMode = (PROCSETCONSOLEDISPLAYMODE)GetProcAddress(hKernel32,"SetConsoleDisplayMode");
  GetConsoleDisplayMode = (PROCGETCONSOLEDISPLAYMODE)GetProcAddress(hKernel32,"GetConsoleDisplayMode");
  
  HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
  DWORD dwOldMode;
  SetConsoleDisplayMode(hOut,1,&dwOldMode);
}

void Console::setTitle(std::string title){
  ::SetConsoleTitle(title.c_str());
}

//sets color of background screen...applies to hScreen1 and hScreen2
void Console::setBackgroundColor(int backColor)
{
  if(backColor < 0 || backColor > 15)
		backColor = 0; //default is black

	mBackColor = backColor;
  
}

//sets forecolor of text
void Console::setColor(int foreColor)
{
  if(foreColor < 0 || foreColor > 15)
		foreColor = 7; //default is white

	mForeColor = foreColor;
}

void Console::write(int x, int y, float value){
  std::ostringstream oss;
  std::string svalue;

  oss << value;
  svalue = oss.str();
  write(x, y, svalue);
}

void Console::write(int x, int y, int value){
  std::ostringstream oss;
  std::string svalue;

  oss << value;
  svalue = oss.str();
  write(x, y, svalue);
}

void Console::write(int x, int y, double value){
  std::ostringstream oss;
  std::string svalue;

  oss << value;
  svalue = oss.str();
  write(x, y, svalue);
}

void Console::write(int x, int y, std::string sText)
{
  if(x < 0 || x >  m_maximumSize.X - 1)
		x = 0;
	if(y < 0 || y > m_maximumSize.Y - 1)
		y = 0;

	mPos.X = x;
	mPos.Y = y;
   
  CONSOLE_CURSOR_INFO cci;
  cci.bVisible = false;
  cci.dwSize= 1;

  mwColors = mBackColor * 16 + mForeColor;
  
	if(mScreenNumber == 1){
	  ::SetConsoleCursorPosition(hScreen1,mPos);
    for(int i = 0; i < (int)sText.length();i++){
      ::WriteConsoleOutputAttribute(hScreen1, &mwColors,1,mPos,&mWritten);
      mPos.X++;
    }
    mPos.X -= (int)sText.length();
    ::WriteConsoleOutputCharacter(hScreen1,sText.c_str(),sText.length(),mPos,&mWritten);
	}
	else if(mScreenNumber == 2){
    ::SetConsoleCursorPosition(hScreen2,mPos);
    for(int i = 0; i < (int)sText.length();i++){
      ::WriteConsoleOutputAttribute(hScreen2, &mwColors,1,mPos,&mWritten);
      mPos.X++;
    }
    mPos.X -= (int)sText.length();
    ::WriteConsoleOutputCharacter(hScreen2,sText.c_str(),sText.length(),mPos,&mWritten);
	}
}

//pass an ascii value to this function for special
//characters such as extended ASCII values
void Console::writeAscii(int x, int y, int ascii)
{
  std::ostringstream oss;
  std::string sText;

  if(x < 0 || x >  m_maximumSize.X - 1)
		x = 0;
	if(y < 0 || y > m_maximumSize.Y - 1)
		y = 0;
  
	mPos.X = x;
	mPos.Y = y;
  
  CONSOLE_CURSOR_INFO cci;
  cci.bVisible = false;
  cci.dwSize= 1;

  mwColors = mBackColor * 16 + mForeColor;
  
  if(ascii >= 0 && ascii <= 255){
    oss << (char)ascii;
    sText = oss.str();

	  if(mScreenNumber == 1){
	    ::SetConsoleCursorPosition(hScreen1,mPos);
      for(int i = 0; i < (int)sText.length();i++){
        ::WriteConsoleOutputAttribute(hScreen1, &mwColors,1,mPos,&mWritten);
        mPos.X++;
      }
      mPos.X -= (int)sText.length();
      ::WriteConsoleOutputCharacter(hScreen1,sText.c_str(),sText.length(),mPos,&mWritten);
      ::SetConsoleCursorInfo(hScreen1, &cci);
	  }
	  else if(mScreenNumber == 2){
      ::SetConsoleCursorPosition(hScreen2,mPos);
      for(int i = 0; i < (int)sText.length();i++){
        ::WriteConsoleOutputAttribute(hScreen2, &mwColors,1,mPos,&mWritten);
        mPos.X++;
      }
      mPos.X -= (int)sText.length();
      ::WriteConsoleOutputCharacter(hScreen2,sText.c_str(),sText.length(),mPos,&mWritten);
      ::SetConsoleCursorInfo(hScreen2, &cci);
	  }
  }
}
  
void Console::writeAsciiLine(int x, int y, int ascii, int num){
 // setColor(0);
  for(int i = 0; i < num; i++)
    writeAscii(x + i, y, ascii);
}


//positions the cursor on the screen (0..79, 0..24)
void Console::posCursor(int x, int y)
{
  if(x < 0 || x >  m_maximumSize.X - 1)
		x = 1;
	if(y < 0 || y > m_maximumSize.Y - 1)
		y = 1;
	
	mPos.X = x;
	mPos.Y = y;
	
	if(mScreenNumber == 1){
    ::SetConsoleTextAttribute(hScreen1, mBackColor * 16 + mForeColor);
    ::SetConsoleCursorPosition(hScreen1,mPos);
	}
	else if(mScreenNumber == 2){
    ::SetConsoleTextAttribute(hScreen2, mBackColor * 16 + mForeColor);
    ::SetConsoleCursorPosition(hScreen2,mPos);
	}
}

//swaps hScreen1 and hScreen2
void Console::flip()
{
	if(mScreenNumber == 1){
    ::SetConsoleActiveScreenBuffer(hScreen1);
    mScreenNumber = 2;
	}
	else if(mScreenNumber == 2){
    ::SetConsoleActiveScreenBuffer(hScreen2);
    mScreenNumber = 1;
	}
}

//clears the screen
void Console::cls()
{
  mPos.X = 0;
  mPos.Y = 0;
  DWORD cWritten; 
  WORD wColor = BACKGROUND_RED | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE; 

  if(mScreenNumber == 1){ 
    FillConsoleOutputCharacter(hScreen1,' ',80*50,mPos,&cWritten);
    FillConsoleOutputAttribute(hScreen1,mwColors,80*50,mPos,&cWritten);
  }
  if(mScreenNumber == 2){ 
    FillConsoleOutputCharacter(hScreen2,' ',80*50,mPos,&cWritten);
    FillConsoleOutputAttribute(hScreen2,mwColors,80*50,mPos,&cWritten);
  }
}

void Console::clrscr(){
  COORD coordScreen = { 0, 0 };
  DWORD cCharsWritten;
  CONSOLE_SCREEN_BUFFER_INFO csbi;
  DWORD dwConSize;
  //HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

  if(mScreenNumber == 1){ 
    GetConsoleScreenBufferInfo(hScreen1, &csbi);
    dwConSize = csbi.dwSize.X * csbi.dwSize.Y;
    FillConsoleOutputCharacter(hScreen1, TEXT(' '),
                              dwConSize,
                              coordScreen,
                              &cCharsWritten);
    GetConsoleScreenBufferInfo(hScreen1, &csbi);
    FillConsoleOutputAttribute(hScreen1,
                              csbi.wAttributes,
                              dwConSize,
                              coordScreen,
                              &cCharsWritten);
    SetConsoleCursorPosition(hScreen1, coordScreen);
  }
  if(mScreenNumber == 2){ 
    GetConsoleScreenBufferInfo(hScreen2, &csbi);
    dwConSize = csbi.dwSize.X * csbi.dwSize.Y;
    FillConsoleOutputCharacter(hScreen2, TEXT(' '),
                              dwConSize,
                              coordScreen,
                              &cCharsWritten);
    GetConsoleScreenBufferInfo(hScreen2, &csbi);
    FillConsoleOutputAttribute(hScreen2,
                              csbi.wAttributes,
                              dwConSize,
                              coordScreen,
                              &cCharsWritten);
    SetConsoleCursorPosition(hScreen2, coordScreen);
  }
}

void Console::SetConsoleSize(int width, int height){
  ::SMALL_RECT sr;
  sr.Left = 0;
  sr.Top = 0;
  sr.Right = width - 1;
  sr.Bottom = height - 1;
  COORD size;
  size.X = width;
  size.Y = height;
  ::SetConsoleWindowInfo(hScreen1, true, &sr);//physical window size
  ::SetConsoleWindowInfo(hScreen2, true, &sr);
  ::SetConsoleScreenBufferSize(hScreen1, size);//buffer (no scroll bar)
  ::SetConsoleScreenBufferSize(hScreen2, size);
  m_maximumSize.X = width;
  m_maximumSize.Y = height;

}

COORD Console::GetMaximumScreenSize(){
  return ::GetLargestConsoleWindowSize(hStdout);
}

CONSOLE_SCREEN_BUFFER_INFO Console::GetScreenBufferInfo(){
   CONSOLE_SCREEN_BUFFER_INFO csbi; 
   DWORD dwConSize;

   GetConsoleScreenBufferInfo( hScreen1, &csbi );       
   return csbi;
}

int Console::GetScreenWidth(){
  return GetSystemMetrics(SM_CXSCREEN);
}

int Console::GetScreenHeight(){
  return GetSystemMetrics(SM_CYSCREEN);
}

int Console::GetFontWidth(){
  return GetScreenWidth()/ GetMaximumScreenSize().X;
}

int Console::GetFontHeight(){
  return GetScreenHeight()/GetMaximumScreenSize().Y;
}

void Console::SetConsolePosition(int x, int y){
  HWND hWnd = ::GetConsoleWindow();
  ::MoveWindow(hWnd, x, y, GetConsoleWidth()* GetFontWidth(), GetConsoleHeight() * GetFontHeight(), true);
}

void Console::CenterConsoleWindow(){
  int left = (GetScreenWidth() - (GetConsoleWidth() * GetFontWidth()))/2;
  int top = (GetScreenHeight() - (GetConsoleHeight() * GetFontHeight()))/2;
  SetConsolePosition(left, top);
  //HWND hWnd = ::GetConsoleWindow();
  //::SetFocus(hWnd);  
}

int Console::GetConsoleWidth(){
  CONSOLE_SCREEN_BUFFER_INFO csbi = GetScreenBufferInfo();
  return csbi.dwSize.X;  
}

int Console::GetConsoleHeight(){
  CONSOLE_SCREEN_BUFFER_INFO csbi = GetScreenBufferInfo();
  return csbi.dwSize.Y;  
}

//clears array
void Console::ClearScreenArray(){
  
  for(int x = 0; x < 79; ++x){
    for(int y = 0; y < 25; ++y){
      m_screen[x][y].ascii = 32;
      m_screen[x][y].foreColor = 0;
      m_screen[x][y].backColor = 0;
    }
  }

}

void Console::SetScreenArray(char x, char y, unsigned char ascii, char foreColor, char backColor){
  if(x < 0 || x > 79)
    return;
  if(y < 0 || y > 24)
    return;
  if(ascii < 32 || ascii > 255)
    return;
  if(foreColor < 0 || foreColor > 15)
    return;
  if(backColor < 0 || backColor > 15)
    return;

  m_screen[x][y].ascii = ascii;
  m_screen[x][y].foreColor = foreColor;
  m_screen[x][y].backColor = backColor;

}

void Console::DrawScreenArray(){
  COORD pos;
  for(int x = 0; x < 79; ++x){
    for(int y = 0; y < 25; ++y){
      //writeAscii(x, y, (int)m_screen[x][y].ascii);
      
      mwColors =  m_screen[x][y].backColor * 16 + m_screen[x][y].foreColor;
      pos.X = x;
      pos.Y = y;
	    if(mScreenNumber == 1){
	      ::SetConsoleCursorPosition(hScreen1, pos);
        ::WriteConsoleOutputAttribute(hScreen1, &mwColors,1, pos,&mWritten);
        ::WriteConsoleOutputCharacter(hScreen1, (LPCSTR)m_screen[x][y].ascii, 1, pos, &mWritten);//(LPCSTR) m_screen[x][y].ascii
	    }
	    else if(mScreenNumber == 2){
	      ::SetConsoleCursorPosition(hScreen2, pos);
        ::WriteConsoleOutputAttribute(hScreen2, &mwColors,1, pos,&mWritten);
        ::WriteConsoleOutputCharacter(hScreen2, (LPCSTR)m_screen[x][y].ascii, 1, pos, &mWritten);//m_screen[x][y].ascii,
	    }
      
    }
  }
}
/*
  if(ascii >= 0 && ascii <= 255){
    oss << (char)ascii;
    sText = oss.str();

	  if(mScreenNumber == 1){
	    ::SetConsoleCursorPosition(hScreen1,mPos);
      for(int i = 0; i < (int)sText.length();i++){
        ::WriteConsoleOutputAttribute(hScreen1, &mwColors,1,mPos,&mWritten);
        mPos.X++;
      }
      mPos.X -= (int)sText.length();
      ::WriteConsoleOutputCharacter(hScreen1,sText.c_str(),sText.length(),mPos,&mWritten);
      ::SetConsoleCursorInfo(hScreen1, &cci);
	  }
	  else if(mScreenNumber == 2){
      ::SetConsoleCursorPosition(hScreen2,mPos);
      for(int i = 0; i < (int)sText.length();i++){
        ::WriteConsoleOutputAttribute(hScreen2, &mwColors,1,mPos,&mWritten);
        mPos.X++;
      }
      mPos.X -= (int)sText.length();
      ::WriteConsoleOutputCharacter(hScreen2,sText.c_str(),sText.length(),mPos,&mWritten);
      ::SetConsoleCursorInfo(hScreen2, &cci);
	  }
  }
*/
