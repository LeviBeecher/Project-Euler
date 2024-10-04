import math
import sys

sys.set_int_max_str_digits(10000)

sum = int(0)
for n in range(1,2023):
    for k in range(0,1+(n-1)//2):
        sum += 9**(k+1) * math.comb(n,k)
print(sum%1000000007)
