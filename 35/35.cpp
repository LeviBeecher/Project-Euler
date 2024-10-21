#include <iostream>
#include <unordered_set>
#include <cmath>

using namespace std;

int main()
{
  unordered_set<int> primes = {2, 3, 5, 7, 11, 13, 17};
  int count = 0;
  
  for (int i = 2; i < 1000000; ++i)
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
        prime = false;
        break;
      }
    }
    
    if (prime)
    {
      primes.insert(i);
    }
  }
  
  cout << "Primes done" << endl;
  
  for (int i : primes)
  {
    int n = i;
    int d = log10f(n);
    
    if (d < 1)
    {
      ++count;
      continue;
    }
    
    bool circular = true;
    
    do 
    {
      n += (n % 10) * (pow(10, d + 1));
      n /= 10;
      
      circular = primes.find(n) != primes.end();
    }
    while (n != i && circular);
      
    count += circular;
  }

  cout << count << endl;

  return 0;
}