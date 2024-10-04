import functools

@functools.cache
def fib(n):
    if n <= 1: return n
    return fib(n-1)+fib(n-2)

if __name__ == '__main__':
    i = 2
    while (1):
        b = fib(i)
        if len(str(b)) >= 1000:
            print(i)
            break;
        i += 1
