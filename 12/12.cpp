#include <iostream>

using namespace std;

int main()
{
  unsigned long long tri = 0;
  
  for (int i = 1; i; ++i)
  {
    tri += i;
    
    int divisors = 0;
    for (unsigned long long j = 1; j <= tri; ++j)
    {
      if (tri % j == 0) ++divisors;
    }
    
    if (divisors > 500)
    {
      break;
    }
  }
  
  cout << tri << endl;
}