#include <iostream>
#include <fstream>
#include <string>
#include <thread>

using namespace std;

bool IsNumeric(char c) { return c >= '0' && c <= '9'; }

long long StrToInt(const string& s)
{
  int strSize = s.size() - 1;
  long long j = 1;
  long long res = 0;
  for (int i = strSize; i >= 0; --i)
  {
    res += (s[i] - 48) * j;
    j *= 10;
  }
  
  return res;
}

int main()
{
  ifstream fp("0059_cipher.txt");
  string message;
  
  string num;
  char c;

  while (fp.get(c))
  {
    if (IsNumeric(c))
    {
      num += c;
    }
    
    else
    {
      message += StrToInt(num);
      num.clear();
    }
  }
  
  if (!num.empty())
  {
    message += StrToInt(num);
    num.clear();
  }
  
  string bestMessage;
  int bestCount = 0;
  
  int messageLength = message.size();
  for (char i = 'a'; i <= 'z'; ++i)
  {
    for (char j = 'a'; j <= 'z'; ++j)
    {
      for (char k = 'a'; k <= 'z'; ++k)
      {
        string key = "";
        key += i; key += j; key += k;
        
        string decryptedMessage;
        for (int l = 0; l < messageLength; ++l)
        {
          decryptedMessage += message[l] ^ key[l % 3];
        }
        
        int count = 0;
        for (int l = 0; l < messageLength - 3; ++l)
        {
          if (decryptedMessage[l] == 't' && decryptedMessage[l + 1] == 'h' && decryptedMessage[l + 2] == 'e') ++count;
        }
        
        if (count > bestCount)
        {
          bestCount = count;
          bestMessage = decryptedMessage;
        }
      }
    }
  }
  
  cout << bestMessage << endl;
  
  int sum = 0;
  for (char c2 : bestMessage) sum += c2;
  
  cout << sum << endl;
  
  return 0;
}