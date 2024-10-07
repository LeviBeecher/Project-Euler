#include <iostream>

using namespace std;

int main()
{
  long long target = 2000000;
  int bestDistance = 0x7FFFFFFF;
  int bestArea = 0;
  int bestX = 0;
  int bestY = 0;
  
  for (int dimX = 1; dimX <= 60; ++dimX)
  {
    for (int dimY = 1; dimY <= 1000; ++dimY)
    {
      long long res = 0;
      
      for (int i = 0; i < dimX; ++i)
      {
        for (int j = 0; j < dimY; ++j)
        {
          res += (dimX - i) * (dimY - j);
        }
      }
      
      int distance = res - target;
      if (abs(distance) < bestDistance)
      {
        bestDistance = abs(distance);
        bestArea = dimX * dimY;
        bestX = dimX;
        bestY = dimY;
      }
      
      if (distance > 0) break;
    }
  }
  
  cout << bestX << " " << bestY << "   " << bestArea << endl;
}