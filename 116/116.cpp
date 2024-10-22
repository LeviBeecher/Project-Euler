#include <iostream>
#include <unordered_map>

using namespace std;

unordered_map<unsigned long long, unsigned long long> cache;

unsigned long long mSizeBlocksInNSpaces(int fn, unsigned long long n, unsigned long long _n, int m)
{
  if (fn == 1)
  {
    return n - (m - 1);
  }
  
  auto findRes = cache.find(m*100000+fn*1000+n);
  if (findRes != cache.end())
  {
    return findRes->second;
  }
  
  unsigned long long res = 0;
  
  // m is size of block
  for (int k = (fn - 1) * m; k <= n - m; ++k)
  {
    res += mSizeBlocksInNSpaces(fn - 1, k, _n, m);
  }
  
  if (n == _n)
  {
    res += mSizeBlocksInNSpaces(fn - 1, n, _n, m);
  }
  
  cache[m*100000+fn*1000+n] = res;
  return res;
}

int main(int argc, char* argv[])
{
  int n = argc == 2 ? atoi(argv[1]) : 5;
  int r = 2;
  int g = 3;
  int b = 4;
  cout << mSizeBlocksInNSpaces(n/2, n, n, r) << endl;
  cout << mSizeBlocksInNSpaces(n/2, n, n, g) << endl;
  cout << mSizeBlocksInNSpaces(n/2, n, n, b) << endl;
  cout << mSizeBlocksInNSpaces(n/2, n, n, r) + mSizeBlocksInNSpaces(n/2, n, n, g) + mSizeBlocksInNSpaces(n/2, n, n, b) << endl;
  
  return 0;
}