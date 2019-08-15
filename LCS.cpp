/*
	LCS.cpp
	COP4531
	Project 5
	String Matching
	Shweta Tiwar LoVoi
	7/19/2019
*/
#include <iostream>
#include <matrix.h>
#include <cstring>
#include <bitvect.h>
#include <bitvect.cpp>
#include <genalg.h>
#include <list.h>

size_t LCS(const char* s, size_t m, const char* t, size_t n, fsu::BitVector& bvs, fsu::BitVector& bvt);
void   LCS(const char* s, size_t m, const char* t, size_t n, fsu::Matrix<size_t>& L);
void alignment(char * s, char * t, fsu::BitVector bvs, fsu::BitVector bvt, fsu::List<char>& SL, fsu::List<char>& TL,
fsu::List<char>& ML );
int main (int argc, char* argv[])
{
	if(argc < 3)
	{
		std::cout << "\n  LCS uses dynamic programming to find the longest common subsequence (LCS) of two sequences s and t.\n"
		<< "    Input is at the command line and consists of two ascii strings.\n"
		<< "    Output shows:\n"
		<< "      Length of the LCS\n"
		<< "      subscripts of the LCS in s\n"
		<< "       bitcode of the LCS in s\n"
		<< "       LCS highlighted in s\n"
		<< "      subscripts of the LCS in t\n"
		<< "       bitcode of the LCS in t\n"
		<< "       LCS highlighted in t\n"
		<< "      alignment of the two strings with LCS highlighted\n"
		<< "  The two strings must be entered as command-line arguments\n"
		<< "  Please report errors or suggestions to lacher@cs.fsu.edu\n\n";
		return EXIT_FAILURE;
	}
	//save s and t char strings
	char * s = argv[1];
	char * t = argv[2];
	//get the length of the char strings
	size_t sizeS = strlen(s);
	size_t sizeT = strlen(t);
	//bitvector to represent bitvector
	fsu::BitVector bvs(sizeS);
	fsu::BitVector bvt(sizeT);
	//get the LCS
	size_t sizeL = LCS(s ,sizeS ,t ,sizeT, bvs, bvt);
	//LCS numbers
	fsu::List<size_t> listS;
	fsu::List<size_t> listT;
	//save the optimal alignment:
	fsu::List<char> sList;
	fsu::List<char> tList;
	fsu::List<char> mList;
	//LCS digits as a list for printing
	for(size_t i = 0; i < sizeS; i++)
	  {
		if(bvs[i] == 1)
		listS.PushBack(i);
	  }
	for(size_t i = 0; i < sizeT; i++)
	  {
		if(bvt[i] == 1)
		listT.PushBack(i);
	  }
	//get the optimal alignment
	alignment(s, t, bvs, bvt, sList, tList, mList);
	//print the stats
	std::cout << "\n  Length of LCS:  " << sizeL << '\n';	
	std::cout << "  LCS in s: { ";
	listS.Display(std::cout, ' ');
	std::cout << "}\n";
	std::cout << "   bitcode: ";
	for(size_t i = 0; i < sizeS; i++)
		std::cout << bvs[i];
	std::cout << "\n        s = " << s;
	std::cout << "\n  LCS in t: { ";
	listT.Display(std::cout, ' ');
	std::cout << "}";
	std::cout << "\n   bitcode: ";
	for(size_t i = 0; i < sizeT; i++)
		std::cout << bvt[i];
	std::cout << "\n        t = " << t 
	<< "\n  optimal alignment:\n"
	<< "   s = ";
	sList.Dump(std::cout);
	std::cout << "       ";
	mList.Dump(std::cout);
	std::cout << "   t = ";
	tList.Dump(std::cout);
	std::cout << '\n';
}

size_t LCS(const char* s, size_t sizeS, const char* t, size_t sizeT, fsu::BitVector& bvs, fsu::BitVector& bvt)
{
  bvs.Unset(); 
  bvt.Unset();
  fsu::Matrix<size_t> L((sizeS + 1),(sizeT + 1),0);
  LCS(s, sizeS, t, sizeT, L);
  for (size_t i = sizeS, j = sizeT; i > 0 && j > 0; ) 
  {
    if (s[i-1] == t[j-1])
    {
      bvs.Set(i-1); 
	  bvt.Set(j-1); // indicates s[i-1] == t[j-1] is in the LCS
      --i; --j;
    }
    else if (L[i][j] == L[i-1][j])
      --i;
    else // L[i,j] == L[i,j-1]
      --j;
  }
  return L[sizeS][sizeT];
}

void LCS(const char* s, size_t sizeS, const char* t, size_t sizeT, fsu::Matrix<size_t>& L)
{
  for (size_t i = 1; i <= sizeS; ++i) 
  {
    for (size_t j = 1; j <= sizeT; ++j)
    {
      if (s[i-1] == t[j-1])
        L[i][j] = 1 + L[i-1][j-1];
      else
    	L[i][j] = fsu::Max( L[i-1][j] , L[i][j-1] );
    }
  }
}

//get the optimal alignment
void alignment(char * s, char * t, fsu::BitVector bvs, fsu::BitVector bvt, fsu::List<char>& SL, fsu::List<char>& TL,
fsu::List<char>& ML )
{
	//bvs size
	size_t bs = 0;
	//bvt size
	size_t bt = 0;
	//counter for string s and t
	size_t sSize = 0;
	size_t tSize = 0;
	//for(size_t k = 0; k < size; k++)
	while(sSize != strlen(s) || tSize != strlen(t))
	{
		//if both vectors have 1 then add to all of the lists
		if(bvs[bs] == 1 && bvt[bt] == 1)
		{	
			SL.PushBack(s[sSize]);
			TL.PushBack(t[tSize]);
			ML.PushBack('|');
			++sSize;
			++tSize;
			++bs;
			++bt;
		} 
		//check if both vectors have 0
		else if(bvs[bs] == 0 && bvt[bt] == 0)
		{
			SL.PushBack(s[sSize]);
			TL.PushBack(t[tSize]);
			ML.PushBack(' ');
			++sSize;
			++tSize;
			++bs;
			++bt;
		}
		//if bvs has 0 and bvt has 1 add char to s and add - to t
		else if(bvs[bs] == 0 && bvt[bt] == 1)
		{
			SL.PushBack(s[sSize]);
			TL.PushBack('-');
			ML.PushBack(' ');
			++sSize;
			++bs;
		}
		//if bvt has 0 and bvs has 1 add - to s and char to t
		else if(bvs[bs] == 1 && bvt[bt] == 0)
		{
			SL.PushBack('-');
			TL.PushBack(t[tSize]);
			ML.PushBack(' ');
			++tSize;
			++bt;
		}
		/* 
		else if((bvs[bs] == 1 || bvs[bs] == 0) && tSize > strlen(t))
		{
			SL.PushBack(s[sSize]);
			++bs;
			++sSize;
			++tSize;
		}
		else if((bvt[bt] == 1 || bvt[bt] == 0) && sSize > strlen(s))
		{
			SL.PushBack(t[tSize]);
			++tSize;
			++sSize;
			++bt;
		}
		*/
	}
}