#include <iostream>
#include <string>

using namespace std;

string numbers[1001];

int NumberLetterCount(int i)
{
  if (i <= 20) return numbers[i].size();
  else if (i < 100) return numbers[i % 10].size() + numbers[10*(i / 10)].size();
  else if (i < 1000) return numbers[100*(i / 100)].size() + (i%100==0?0:3) + (i-100*(i / 100)>20?numbers[10*((i % 100) / 10)].size() + numbers[i%10].size() : numbers[i-100*(i / 100)].size()); 
  else return numbers[1000].size();
}

int main()
{
  int sum = 0;
  
  numbers[0] = "";
  numbers[1] = "one";
  numbers[2] = "two";
  numbers[3] = "three";
  numbers[4] = "four";
  numbers[5] = "five";
  numbers[6] = "six";
  numbers[7] = "seven";
  numbers[8] = "eight";
  numbers[9] = "nine";
  numbers[10] = "ten";
  numbers[11] = "eleven";
  numbers[12] = "twelve";
  numbers[13] = "thirteen";
  numbers[14] = "fourteen";
  numbers[15] = "fifteen";
  numbers[16] = "sixteen";
  numbers[17] = "seventeen";
  numbers[18] = "eighteen";
  numbers[19] = "nineteen";
  numbers[20] = "twenty";
  numbers[30] = "thirty";
  numbers[40] = "forty";
  numbers[50] = "fifty";
  numbers[60] = "sixty";
  numbers[70] = "seventy";
  numbers[80] = "eighty";
  numbers[90] = "ninety";
  numbers[100] = "onehundred";
  numbers[200] = "twohundred";
  numbers[300] = "threehundred";
  numbers[400] = "fourhundred";
  numbers[500] = "fivehundred";
  numbers[600] = "sixhundred";
  numbers[700] = "sevenhundred";
  numbers[800] = "eighthundred";
  numbers[900] = "ninehundred";
  numbers[1000] = "onethousand";
  
  for (int i = 1; i <= 1000; ++i)
  {
    sum += NumberLetterCount(i);
  }

  cout << sum << endl;
}