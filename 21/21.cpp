#include <iostream>
#include <unordered_set>

using namespace std;

int main()
{
  unordered_set<int> found;
  int sum = 0;
  
  for (int i = 1; i < 10000; ++i)
  {
    if (found.find(i) != found.end()) continue;
    
    int d = 0;
    for (int j = 1; j <= i / 2; ++j)
    {
      d += i % j == 0 ? j : 0;
    }
    
    if (d != i)
    {
      int d2 = 0;
      for (int j = 1; j <= d / 2; ++j)
      {
        d2 += d % j == 0 ? j : 0;
      }
      
      if (d2 == i)
      {
        cout << "Found pair: " << i << " " << d << endl;
        sum += i + d;
        found.insert(i);
        found.insert(d);
      }
    }
  }
  
  cout << sum << endl;
  return 0;
}