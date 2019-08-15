/*
	ED.cpp
	COP4531
	Project 5
	String Matching
	Shweta Tiwar LoVoi
	7/19/2019
*/
#include <iostream>
#include <cstring>
#include <matrix.h>
#include <list.h>

size_t LCS(const char* s, size_t m, const char* t, size_t n, fsu::Matrix<size_t>& L, size_t lev, fsu::Matrix<char>& P);
size_t countDigit(size_t n);
void transcript(fsu::Matrix<char> P, fsu::Matrix<size_t> L, fsu::List<char>& L1, size_t m, size_t n);
//sends old string, List with transcript, and the new string which will be printed
void alignment(char * oldStr, fsu::List<char> L1, fsu::List<char>& newStr);
//to get the middle line
void alignment1(fsu::List<char> L1, fsu::List<char>& newStr);
int main (int argc, char* argv[])
{
  if(argc < 3)
    {
       std::cout << "\n  ED uses dynamic programming to find the edit "
       << "distance (ED) between two sequences s and t.\n"
       << "    Input is at the command line and consists of:\n"
       << "      1,2: ascii strings             (required)\n"
       << "        3: value for substition cost (optional - default = 2 = Levenshtein)\n"
       << "        4: matrix dump code          "
       << "(optional - default = 0): 0 = none, 1 = distance, 2 = parent, 3 = both\n"
       << "    Output shows:\n"
       << "      the edit distance\n"
       << "      the string s\n"
       << "      edit transcript for s -> t\n"
       << "      edit transcript for t -> s\n"
       << "      the string t\n"
       << "      optimal alignment of the two strings\n"
       << "  The two REQUIRED strings two OPTIONAL inputs must be entered as command line arguments\n"
       << "  Please report errors or suggestions to lacher@cs.fsu.edu\n\n";   
    }
  char * s = argv[1];
  char * t = argv[2];
  size_t lev = 2;
  size_t levPrint = 0;
  if(argc == 4 || argc == 5)
    {
      if(atoi(argv[3]) >= 2)
      {
        lev = 2;
        levPrint = atoi(argv[3]);
      }
      else
      {
        lev = 1;   
        levPrint = atoi(argv[3]);
      }
    }
  else 
    lev = 2;
  size_t dump = 0;
  if(argc == 5)
    dump = atoi(argv[4]);
  size_t sizeS = strlen(s);
  size_t sizeT = strlen(t);
  //save the words for optimal alignment
  fsu::List<char> newS;
  fsu::List<char> newT;
  fsu::List<char> A; 
  //distance matrix
  fsu::Matrix<size_t> L((sizeS + 1),(sizeT + 1),0);
  //parent matrix
  fsu::Matrix<char> P((sizeS + 1),(sizeT + 1),'-');
  //format size_t to print matrix correctly
  size_t sizeL = LCS(s ,sizeS ,t ,sizeT, L, lev, P);
  //list of transcript s -> t
  fsu::List<char> L1;
  //go over the matrix to construct transcript
  transcript(P, L, L1, sizeS, sizeT);
  //list of transcript for t -> s
  fsu::List<char> L2 = L1;
  //change the I to D
  fsu::List<char>::Iterator j = L2.Begin();
  for(; j != L2.End(); j++)
  {
    if(*j == 'D')
      *j = 'I';
    else if(*j == 'I')
      *j = 'D';
  }
  //get the optimal alighment
  alignment(s, L1, newS);
  alignment(t, L2, newT);
  //to get the alignment line
  alignment1(L1, A);
  //if 1 = distance 
  if(dump == 1)
  {
    std::cout << "Distance Matrix:\n";
    L.Dump(std::cout, sizeL + 1);
  }
  if(dump == 2)
  {
    std::cout << "Parent Matrix:\n";
    P.Dump(std::cout,2);
  }
  if(dump == 3)
  {
    std::cout << "Distance Matrix:\n";
    L.Dump(std::cout, sizeL + 1);
    std::cout << "Parent Matrix:\n";
    P.Dump(std::cout,2);
  }
  std::cout << "\n      Edit Distance: " << L[sizeS][sizeT];
  if(levPrint == 2)
    std::cout << " // Levenshtein - substitution cost = 2\n";
  else if(argc == 3)
    std::cout << " // Levenshtein - substitution cost = 2\n";
  else
    std::cout << " // substitution cost = " << levPrint << std::endl; 
  std::cout << "                  s: " << s
    << "\n   s > t transcript: ";
  L1.Dump(std::cout);
  std::cout << "   t > s transcript: ";
  L2.Dump(std::cout);
  std::cout << "                  t: " << t
    << "\n  optimal alignment:\n"
    << "   s = ";
  newS.Dump(std::cout);
  std::cout << "       ";
  A.Dump(std::cout);  
  std::cout << "   t = ";
  newT.Dump(std::cout);

}


size_t LCS(const char* s, size_t sizeS, const char* t, size_t sizeT, fsu::Matrix<size_t>& L, size_t lev, fsu::Matrix<char>& P)
{
  size_t size = 0;
  //set up the blank matrix with initial values: Horizontal
  for (size_t i = 1; i <= sizeT; ++i) 
  {
    P[0][i] = 'L';
    L[0][i] = L[0][i - 1] + 1;
    //get the size of number of digits in the matrix for distance dump format
    size_t newSize = countDigit(L[0][1]);
    if(newSize > size)  
      size = newSize;
  }
  //vertical
  for (size_t j = 1; j <= sizeS; ++j)
  {
    P[j][0] = 'U';
    L[j][0] = L[j-1][0] + 1;
    //get the size of number of digits in the matrix for distance dump format
    size_t newSize = countDigit(L[j][0]);
      if(newSize > size)  
      size = newSize;
  }
  //calculate ED 
  for (size_t i = 1; i <= sizeS; ++i) 
  {
    for (size_t j = 1; j <= sizeT; ++j)
    {
      //if char match then get horizontal matrix entry
      if(s[i -1] == t[j -1])
      {
        L[i][j] = L[i-1][j-1];
        P[i][j] = 'D';
        size_t newSize = countDigit(L[i][j]);
        if(newSize > size)  
          size = newSize;
      }
      //if char do not match get the minimum and add 1
      //this is a diagonal jump
      else
      {
        size_t levMin = L[i-1][j-1];
        size_t min = L[i][j-1];
        if(L[i-1][j-1] < min)
          min = L[i-1][j-1];
        if(L[i-1][j] < min)
          min = L[i-1][j];
        //if minimum is on the diagonal set D in P
        if(min == L[i-1][j-1])
          P[i][j] = 'D';
        //if minimum is Up set U
        else if(min == L[i-1][j])
          P[i][j] = 'U';
        //if minimun if Left set L
        else if(min == L[i][j-1])
          P[i][j] = 'L';
        if(min == levMin)
          L[i][j] = min + lev;
        else
          L[i][j] = min + 1;
        //get the number of digits to set the format of dump for distance matrix
        size_t newSize = countDigit(L[i][j]);
        if(newSize > size)  
          size = newSize;
      }
    }
  }
  //return the number of digits to set the dump
  return size;
} 
//to calculate the transctipt s > t
void transcript(fsu::Matrix<char> P, fsu::Matrix<size_t> L, fsu::List<char>& L1, size_t sizeS, size_t sizeT)
{
  //backtrace the parent matrix. This will give the reverse order since
  //we are going from bottom up
  size_t j = sizeT;
  for(size_t i = sizeS; i > 0 || j > 0;)
  {
    if(P[i][j] == 'D')
    {
      if(L[i-1][j-1] == L[i][j])
        L1.PushBack('M');
      else
        L1.PushBack('S');
      j--;
      i--;
    }
    else if(P[i][j] == 'L')
    {
      L1.PushBack('I');
      j--;
    }
    else if(P[i][j] == 'U')
    {
      i--;
      L1.PushBack('D');
    }
  }
  //reverse the list to get the correct order
  L1.Reverse();
}
//return the max numner of digits in matrix
size_t countDigit(size_t n) 
{ 
  size_t count = 0; 
  while (n != 0) { 
    n = n / 10; 
    ++count; 
  } 
  return count; 
}

void alignment(char * oldStr, fsu::List<char> L, fsu::List<char>& newStr)
{
  //distance list iterator
  fsu::List<char>::Iterator i = L.Begin();
  size_t j = 0;
  //for loop for the larger of the 2 strings
  if(L.Size() < strlen(oldStr))
  {
    for(; j < strlen(oldStr); j++)
    {
      //if the char in L is 'S' or D or M keep the oldStr char the same
      if( *i == 'S' || *i == 'D' || *i == 'M')
      {
        newStr.PushBack(oldStr[j]);  
        i++;
      }
      //if the char in L is 'I' then oldStr char gets -
      else if(*i == 'I')
      {
        newStr.PushBack('-');
        i++;
      }
      else
        newStr.PushBack(oldStr[j]);
    }
  }
  else
  {
    for(; i != L.End(); i++)
    {
      if(*i == 'S' || *i == 'D' || *i == 'M')
      {
        newStr.PushBack(oldStr[j]);  
        j++;
      }
      //if the char in L is 'I' then oldStr char gets -
      else if(*i == 'I')
        newStr.PushBack('-');
      else
        newStr.PushBack(oldStr[j]);
    }
  }
}

void alignment1(fsu::List<char> L, fsu::List<char>& newStr)
{
  //if the char in transcript is 'M then add | to the alignment list
  fsu::List<char>::Iterator i = L.Begin(); 
  for(; i != L.End(); i++)
  {
    if(*i == 'M')
      newStr.PushBack('|');
    else
      newStr.PushBack(' ');
  }
}