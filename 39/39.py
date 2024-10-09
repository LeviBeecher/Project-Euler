best = 0
bestP = 0
for i in range(1, 1001):
    count = 0
    for x in range(1, i - 2):
        for y in range(1, i - 2 - x):
            h = i - x - y
            if h**2 == x**2 + y**2:
                count += 1
    if count > best:
        best = count
        bestP = i
        print(best, bestP)

print(best, bestP)
