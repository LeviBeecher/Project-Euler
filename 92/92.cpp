#include <iostream>
#include <unordered_map>
#include <vector>
#include <chrono>

using namespace std;

int main()
{
  // Number / Arrives at 89
  unordered_map<int, bool> results;
  
  results[1] = false;
  results[89] = true;
  
  int count = 0;
  for (int i = 1; i < 10000000; ++i)
  {
    if (results.find(i) != results.end())
    {
      count += results[i];
      continue;
    }
    
    int n = i;
    vector<int> chain;
    while (n != 1 && n != 89)
    {
      chain.emplace_back(n);
      
      int temp = n;
      n = 0;
      while (temp)
      {
        n += (temp % 10) * (temp % 10);
        temp /= 10;
      }
    }
    
    bool res = n == 89;
    for (int j : chain)
    {
      results[j] = res;
    }
    
    count += res;
  }
  
  cout << count << endl;
  
  return 0;
}