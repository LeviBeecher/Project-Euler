#include <iostream>

using namespace std;

int MaxPathSumBF(int triangle[15][15], int depth, int x, int sum)
{ 
  sum += triangle[depth][x];
 
  if (depth == 14) return sum;
  
  int r1 = MaxPathSumBF(triangle, depth + 1, x, sum);
  int r2 = 0;
  if (x < 15) r2 = MaxPathSumBF(triangle, depth + 1, x + 1, sum);
  
  return max(r1, r2);
}

int MaxPathSum(int triangle[15][15])
{ 
  for (int i = 13; i >= 0; --i)
  {
    for (int j = 0; j <= i; ++j)
    {
      triangle[i][j] += max(triangle[i+1][j], triangle[i+1][j+1]);
    }
  }
  
  for (int i = 0; i < 15; ++i)
  {
    for (int j = 0; j < 15; ++j)
    {
      cout << triangle[i][j] << " ";
    }
    cout << endl;
  }
  
  return 0;
}

int main()
{
  int triangle[15][15] = {
    {75, 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0  },
    {95, 64, 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0  },
    {17, 47, 82, 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0  },
    {18, 35, 87, 10, 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0  },
    {20,  4, 82, 47, 65, 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0  },
    {19,  1, 23, 75, 03, 34, 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0  },
    {88,  2, 77, 73, 07, 63, 67, 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0  },
    {99, 65,  4, 28, 06, 16, 70, 92, 0 , 0 , 0 , 0 , 0 , 0 , 0  },
    {41, 41, 26, 56, 83, 40, 80, 70, 33, 0 , 0 , 0 , 0 , 0 , 0  },
    {41, 48, 72, 33, 47, 32, 37, 16, 94, 29, 0 , 0 , 0 , 0 , 0  },
    {53, 71, 44, 65, 25, 43, 91, 52, 97, 51, 14, 0 , 0 , 0 , 0  },
    {70, 11, 33, 28, 77, 73, 17, 78, 39, 68, 17, 57, 0 , 0 , 0  },
    {91, 71, 52, 38, 17, 14, 91, 43, 58, 50, 27, 29, 48, 0 , 0  },
    {63, 66,  4, 68, 89, 53, 67, 30, 73, 16, 69, 87, 40, 31, 0  },
    { 4, 62, 98, 27, 23,  9, 70, 98, 73, 93, 38, 53, 60, 04, 23 }
  };
  
  cout << MaxPathSumBF(triangle, 0, 0, 0) << endl;
  MaxPathSum(triangle);
  
  return 0;
}