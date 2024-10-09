#include <iostream>
#include <vector>
#include <thread>
#include <utility>
#include <algorithm>

using namespace std;

const int matDim = 15;
int maxElement = 0;

int MatrixSumAux(vector<vector<int>> grid, int depth, int current, int& best)
{
  if (depth == matDim) return current;
 
  int maxPossible = maxElement * (matDim - depth) + current;
  if (maxPossible < best) return current;
  
  int indices[matDim] = { 0 };
  for (int i = 0; i < matDim; ++i) indices[i] = -1;
  int values[matDim] = { 0 };
  
  // Sort indices into grid from greatest to least
  for (int i = 0; i < matDim; ++i)
  {
    // Column already used
    if (grid[depth][i] < 0) continue;

    for (int j = 0; j < matDim; ++j)
    {
      if (grid[depth][i] > values[j])
      {
        for (int k = matDim - depth - 1; k > j; --k)
        {
          values[k] = values[k - 1];
          indices[k] = indices[k - 1];
        }
        
        values[j] = grid[depth][i];
        indices[j] = i;
        break;
      }
    }
  }
  
  int bestResult = 0;
  for (int i = 0; i < matDim - depth; ++i)
  {
    current += grid[depth][indices[i]];
    
    int j = 0;
    for (auto& v : grid)
    {
      values[j] = v[indices[i]];
      v[indices[i]] = -1;
      ++j;
    }
    
    int res = MatrixSumAux(grid, depth + 1, current, best);
    
    j = 0;
    for (auto& v : grid)
    {
      v[indices[i]] = values[j];
      ++j;
    }
    
    current -= grid[depth][indices[i]];
    
    bestResult = res > bestResult ? res : bestResult;
  }
  
  best = bestResult > best ? bestResult : best;
  return bestResult;
}

int main()
{
  // 15x15
  vector<vector<int>> grid({
    vector<int>({  7,  53, 183, 439, 863, 497, 383, 563,  79, 973, 287,  63, 343, 169, 583}),
    vector<int>({627, 343, 773, 959, 943, 767, 473, 103, 699, 303, 957, 703, 583, 639, 913}),
    vector<int>({447, 283, 463,  29,  23, 487, 463, 993, 119, 883, 327, 493, 423, 159, 743}),
    vector<int>({217, 623,   3, 399, 853, 407, 103, 983,  89, 463, 290, 516, 212, 462, 350}),
    vector<int>({960, 376, 682, 962, 300, 780, 486, 502, 912, 800, 250, 346, 172, 812, 350}),
    vector<int>({870, 456, 192, 162, 593, 473, 915,  45, 989, 873, 823, 965, 425, 329, 803}),
    vector<int>({973, 965, 905, 919, 133, 673, 665, 235, 509, 613, 673, 815, 165, 992, 326}),
    vector<int>({322, 148, 972, 962, 286, 255, 941, 541, 265, 323, 925, 281, 601,  95, 973}),
    vector<int>({445, 721,  11, 525, 473,  65, 511, 164, 138, 672,  18, 428, 154, 448, 848}),
    vector<int>({414, 456, 310, 312, 798, 104, 566, 520, 302, 248, 694, 976, 430, 392, 198}),
    vector<int>({184, 829, 373, 181, 631, 101, 969, 613, 840, 740, 778, 458, 284, 760, 390}),
    vector<int>({821, 461, 843, 513,  17, 901, 711, 993, 293, 157, 274,  94, 192, 156, 574}),
    vector<int>({ 34, 124,   4, 878, 450, 476, 712, 914, 838, 669, 875, 299, 823, 329, 699}),
    vector<int>({815, 559, 813, 459, 522, 788, 168, 586, 966, 232, 308, 833, 251, 631, 107}),
    vector<int>({813, 883, 451, 509, 615,  77, 281, 613, 459, 205, 380, 274, 302,  35, 805})
  });
  
  // 5x5
  vector<vector<int>> grid2({
    vector<int>({  7,  53, 183, 439, 863}),
    vector<int>({497, 383, 563,  79, 973}),
    vector<int>({287,  63, 343, 169, 583}),
    vector<int>({627, 343, 773, 959, 943}),
    vector<int>({767, 473, 103, 699, 303})
  });
  
  maxElement = 0;
  for (auto const& v : grid)
  {
    for (int i : v)
    {
      maxElement = maxElement < i ? i : maxElement;
    }
  }
  
  int best = 0;
  int res = MatrixSumAux(grid, 0, 0, best);
  
  cout << res << " " << best << endl;
  
  return 0;
}