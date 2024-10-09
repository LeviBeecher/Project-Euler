#include <iostream>

using namespace std;

int main()
{
  for (unsigned long long i = 1000000000; i < 1415000000; i += 10)
  {
    unsigned long long sqr = i * i;
    
    bool found = true;
    unsigned long long expected = 0;
    while (sqr)
    {
      if (sqr % 10 != expected)
      {
        found = false;
        break;
      }
      
      expected = expected == 0 ? 9 : expected - 1;
      
      sqr /= 100;
    }
    
    if (found)
    {
      cout << i << endl;
      break;
    }
  }
}