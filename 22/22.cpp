#include <algorithm>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

int main()
{
  unsigned long long res = 0;
  
  ifstream namesFile("0022_names.txt");
  
  vector<string> names;
  
  auto alphabeticalOrderSort = [](const string& s1, const string& s2)
  {
    int len = min(s1.size(), s2.size());
    for (int i = 0; i < len; ++i)
    {
      if (s1[i] < s2[i]) return true;
      else if (s1[i] > s2[i]) return false;
    }
    
    return s1.size() < s2.size();
  };
  
  char c;
  string name;
  while (namesFile.get(c))
  {
    if (c == ',')
    {
      names.emplace_back(name);
      name.clear();
      continue;
    }
    
    if (c != '"')
    {
      name += c;
    }
  }
  
  names.emplace_back(name);
  name.clear();
  
  sort(names.begin(), names.end(), alphabeticalOrderSort);
  
  int i = 1;
  for (const string& s : names)
  {
    unsigned long long score = 0;
    for (char _c : s) score += _c - 'A' + 1;
    score *= i;
    
    res += score;
    ++i;
  }
 
  cout << res << endl;
  
  return 0;
}