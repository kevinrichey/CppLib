#include <cstdio>
#include "TestCase.h"
#include <iostream>
#include "kwrlib.h"

using namespace std;
using namespace kwr;

int main()
{
   cout 
      << "Test Cases: " 
      << TestCase::RunAll()
      << endl;

	return 0;
}

// vim: foldmethod=marker
