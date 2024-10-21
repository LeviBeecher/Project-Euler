#include <iostream>
#include <iomanip>
#include <fstream>
#include <vector>
#include <string>

using namespace std;

int MaxPathSum(vector<vector<int>>& triangle)
{ 
  int s = triangle.size();
  
  for (int i = s - 2; i >= 0; --i)
  {
    for (int j = 0; j <= i; ++j)
    {
      triangle[i][j] += max(triangle[i+1][j], triangle[i+1][j+1]);
    }
  }
  
  for (auto& v : triangle)
  {
    for (int i : v)
    {
      cout << fixed << setw(4) << setfill(' ') << i << " ";
    }
    
    cout << endl;
  }
  
  return triangle[0][0];
}

int main()
{
  vector<vector<int>> triangle;
  ifstream triangleData("0067_triangle.txt");

  char buf[4096];
  while (triangleData.getline(buf, 4096))
  {
    vector<int> row;
    string numStr(buf);
    string num;
    
    for (char c : numStr)
    {
      if (c == ' ')
      {
        row.emplace_back(stoi(num));
        num.clear();
        continue;
      }
      
      num += c;
    }
    
    row.emplace_back(stoi(num));
    num.clear();
    
    triangle.emplace_back(row);
  }

  cout << MaxPathSum(triangle) << endl;
  
  return 0;
}