#include <iostream>
#include <unordered_set>

using namespace std;

int main()
{
  unordered_set<int> primes = {2, 3, 5, 7, 11, 13, 17};
  
  long long unsigned sum = 0;
  
  for (int i = 2; i < 2000000; ++i)
  {
    bool prime = true;
    for (int p : primes)
    {
      if (primes.count(i) == 1)
      {
        break;
      }
      
      if (i % p == 0)
      {
        //cout << i << " " << p << endl;
        prime = false;
        break;
      }
    }
    
    if (prime)
    {
      cout << "Prime: " << i << endl;
      sum += i;
      primes.insert(i);
    }
  }
  
  cout << sum << endl;

  return 0;
}