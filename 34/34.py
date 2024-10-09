import math

res = 0
for i in range(3, 1000000):
    s = str(i)
    sum = 0;
    for c in s:
        sum += math.factorial(int(c))
    if sum == i:
        res += i
        print(i)

print(res)
