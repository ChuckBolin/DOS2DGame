#ifndef CINIREADER_H
#define CINIREADER_H

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

struct STRUCT_CONFIG_DATA{
  std::string key;
  std::string value;
};

class CINIReader{
public:
  CINIReader();
  CINIReader(std::string filename);
  ~CINIReader();

  void LoadFile(std::string filename);
  
  bool IsValid(){return m_bFileExists;}
  int GetNumberOfLines();
  std::string GetLineFromFile(int i);
  int GetNumberOfTermsFromLine(int i);
  std::string GetTerm(std::string line, int term);
  std::string GetValue(std::string key);
  bool Exists(std::string key);
  

private:
  std::string RemoveSpaces(std::string in);
  std::string RemoveComments(std::string in);
  int CountCommas(std::string in);  
  int CountEqualSigns(std::string in);

  std::string m_Filename;
  bool m_bFileExists;
  int m_NumberOfLines;
  std::vector<STRUCT_CONFIG_DATA> m_Var;
  std::vector<std::string> m_Lines;
  

};

#endif