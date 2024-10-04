#include <iostream>
#include <cstring>
#include <unordered_map>

using namespace std;

int main()
{
  unordered_map<unsigned long long, unsigned long long> lengths;
  
  unsigned long long res = 0;
  unsigned res2 = 1;
  for (unsigned i = 1; i < 1000000; ++i)
  {
    unsigned n = i;
    unsigned long long length = 1;
    while (n != 1)
    {
      n = n % 2 == 0 ? n / 2 : 3 * n + 1;
      ++length;
      
      // Length has been computed before
      if (lengths.find(n) != lengths.end())
      {
        length += lengths[n];
        break;
      }
    }
    
    // Add length to precomputed lengths
    if (lengths.find(n) == lengths.end())
    {
      lengths[i] = length;
    }
    
    if (length > res) 
    {
      res = length;
      res2 = i;
    }
  }
  
  cout << res << " " << res2 << endl;
  
  return 0;
}