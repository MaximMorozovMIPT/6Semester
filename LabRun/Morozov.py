import numpy as np
import matplotlib.pyplot as plt
import os
import math

k = lambda x: 1 + np.cos(x) ** 2
q = 1 
f = lambda x: np.sin(x) ** 2

def Matrix(N, h):
    matrix = np.zeros((N, N))
    val = np.zeros(N)
    matrix[0][0] = -k(0) - h
    matrix[0][1] = k(0)
    matrix[N - 1][N - 2] = k(1)
    matrix[N - 1][N - 1] = -k(1) - h
    for i in range(1, N - 1):
        matrix[i][i - 1] = k((i - 0.5) * h) / (h ** 2)
        matrix[i][i] = -(k((i - 0.5) * h) + k((i + 0.5) * h)) / (h ** 2) - q
        matrix[i][i + 1] = k((i + 0.5) * h) / (h ** 2)
        val[i] = -f(i * h)
    val[0] = 0
    val[N - 1] = -h
    return matrix, val

def run(matrix, val):
    N = val.shape[0]
    p = np.zeros(N)
    r = np.zeros(N)
    p[0] = matrix[0][1] / matrix[0][0]
    r[0] = val[0] / matrix[0][0]
    for i in range(1, N - 1):
        p[i] = matrix[i][i + 1] / (matrix[i][i] - matrix[i][i - 1] * p[i - 1])
        r[i] = (val[i] - matrix[i][i - 1] * r[i - 1]) / (matrix[i][i] - matrix[i][i - 1] * p[i - 1])
    r[N - 1] = (val[N - 1] - matrix[N - 1][N - 2] * r[N - 2]) / (matrix[N - 1][N - 1] - matrix[N - 1][N - 2] * p[N - 2])
    return p, r

def reverse_run(p, r):
    sol = np.zeros(N)
    sol[N - 1] = r[N - 1]
    for i in range(N - 2, -1, -1):
        sol[i] = r[i] - p[i] * sol[i + 1]
    return sol
   
N = 11
h = 0.1

matrix, val = Matrix(N, h)

p, r = run(matrix, val)
solution = reverse_run(p, r)

x = []
for i in range(11):
    x.append(i*h)

N = 101
h = 0.01

matrix1, val1 = Matrix(N, h)

p1, r1 = run(matrix1, val1)
solution1 = reverse_run(p1, r1)

y = []
for i in range(101):
    y.append(i*h)

N = 1001
h = 0.001

matrix2, val2 = Matrix(N, h)

p2, r2 = run(matrix2, val2)
solution2 = reverse_run(p2, r2)

z = []
for i in range(1001):
    z.append(i*h)

N = 10001
h = 0.0001

matrix3, val3 = Matrix(N, h)

p3, r3 = run(matrix3, val3)
solution3 = reverse_run(p3, r3)

w = []
for i in range(10001):
    w.append(i*h)

fig, ax = plt.subplots(figsize=(16, 9)) 
ax.plot(x, solution, marker='x', markersize=4, label="11 dots")
ax.plot(y, solution1, label="101 dots")
ax.plot(z, solution2, label="1001 dots")
ax.plot(w, solution3, label="10001 dots")
ax.set_title('Solution')
ax.set_xlabel('x')
ax.set_ylabel('y')
ax.grid(True)
ax.legend()
fig.savefig(os.path.join("LabRun/solution.png"), dpi=100) 

printable1 = []
printable2 = []
printable3 = []
for i in range(101):
    if(i % 10 == 0):
        printable1.append(solution1[i])
for i in range(1001):
    if(i % 100 == 0):
        printable2.append(solution2[i])
for i in range(10001):
    if(i % 1000 == 0):
        printable3.append(solution3[i])

print(solution)
print(printable1)
print(printable2)
print(printable3)
