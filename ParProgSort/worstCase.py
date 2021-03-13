import math

def worstCaseArrayOfSize(n):
    if n == 1:
        return [1]
    else:
        top = worstCaseArrayOfSize(int(math.floor(float(n) / 2)))
        bottom = worstCaseArrayOfSize(int(math.ceil(float(n) / 2)))
        a = list(map(lambda x: x * 2, top))
        b = list(map(lambda x: x * 2 - 1, bottom))
        return a + b

with open('your_file.txt', 'w') as f:
    for item in worstCaseArrayOfSize(30000000):
        f.write("%s " % item)
