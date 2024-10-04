#include <iostream>
#include <iomanip>
#include <cmath>

using namespace std;

int main()
{
  for (float a = 1; a < 499; ++a)
  {
    for (float b = a + 1; b < 500; ++b)
    {
      if (a + b + sqrtf(a*a+b*b) == 1000)
      {
        cout << setprecision(0) << fixed;
        cout << a << " " << b << " " << sqrtf(a*a+b*b) << endl;
        cout << static_cast<int>(a * b * sqrtf(a*a+b*b)) << endl;
      }
    }
  }
  
  return 0;
}