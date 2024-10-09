#include <iostream>
#include <unordered_map>
#include <vector>
#include <cmath>
#include <string>

using namespace std;

int main()
{
  vector<unsigned long long> primes = { 2 };
  
  unsigned long long res = 0;
  
  // Any 8 or 9 digit pandigital number will be divisible by 3
  // Find all primes up to 7654321, then loop through primes with the pandigital checks
  // First prime that's pandigital on the backward loop is the answer
  int j = 0;
  for (unsigned long long i = 3; i <= 7654321; i += 2, j += 2)
  {
    if (j == 1000000)
    {
      j = 0;
      cout << "." << endl;
    }
    
    bool prime = true;
    for (unsigned long long p : primes)
    {
      if (i % p == 0)
      {
        prime = false;
        break;
      }
    }
    
    if (prime)
    {
      primes.emplace_back(i);
    }
  }
  
  for (int i = primes.size() - 1; i >= 0; --i)
  {
    unsigned long long n = primes[i];
    int digitCount = 0;
    bool digitPresent[10] = { 0 };
    while (n)
    {
      digitPresent[n % 10] = true;
      n /= 10;
      digitCount++;
    }
    
    bool pandigital = true;
    for (int i = 1; i <= digitCount; ++i)
    {
      if (!digitPresent[i])
      {
        pandigital = false;
        break;
      }
    }
    
    if (pandigital)
    {
      res = primes[i];
      break;
    }
  }
  
  cout << res << endl;
}