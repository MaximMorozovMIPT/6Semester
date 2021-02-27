import numpy as np
import matplotlib.pyplot as plt
import os

epsilon = 10 ** 3
num_of_stages = 4
num_of_equations = 4
end_time = 0.1
h = 0.000001

# Return equasion array
def equasion(sol_n):
    a = epsilon * ((sol_n[2] - sol_n[3]) - np.tan(np.pi * sol_n[0] / 2) - sol_n[1])
    b = epsilon * ((sol_n[3] - sol_n[2]) - np.tan(np.pi * sol_n[1] / 2) - sol_n[0])
    c = sol_n[0]
    d = sol_n[1]
    return np.array([a, b, c, d])

# Start conditions
def begin_values():
    return np.array([0,0,2,0])

# Ok go next
def butcher_table():
    a_table = np.zeros((num_of_stages, num_of_stages))
    a_table[0][0] = a_table[1][1] = a_table[2][2] = a_table[3][3] = 0.5
    a_table[1][0] = 1.0 / 6
    a_table[2][1] = a_table[3][2] = 0.5
    a_table[2][0] = -0.5
    a_table[3][0] = 1.5
    a_table[3][1] = -1.5
    b_row = np.array([1.5, -1.5, 0.5, 0.5])
    c_column = np.array([0.5, 2.0 / 3, 0.5, 1])
    return a_table, b_row, c_column

a_table, b_row, c_column = butcher_table()

# Derivative matrix from equasions
def equasion_matrix(sol_n):
    eq_matr = np.zeros((num_of_equations, num_of_equations))
    eq_matr[0][0] = ((-1) / (np.cos(np.pi * sol_n[0] / 2) ** 2) * np.pi / 2) * epsilon
    eq_matr[0][1] = (-1) * epsilon
    eq_matr[0][2] = (-1) * epsilon
    eq_matr[0][3] = 1 * epsilon
    eq_matr[1][0] = (-1) * epsilon
    eq_matr[1][1] = ((-1) / (np.cos(np.pi * sol_n[1] / 2) ** 2) * np.pi / 2) * epsilon
    eq_matr[1][2] = 1 * epsilon
    eq_matr[1][3] = (-1) * epsilon
    eq_matr[2][0] = 1
    eq_matr[3][1] = 1
    return eq_matr

# Newton method for system solution
def solve_newton(a, sol_n, h, alpha, equasion):
    maxiter = 15
    eps = np.float64(1.0e-5)
    dim = a.shape[0]
    initial = np.zeros(dim, dtype=np.float64)
    res = a.dot(equasion(sol_n))
    counter = 0
    while np.linalg.norm(res - initial) >= eps and counter <= maxiter:
        counter += 1
        initial = res
        res = initial - a.dot(initial - equasion(sol_n + h * alpha * initial))
    return res

# Implicit method runge kutta (very very long computating)
def runge_kutta(sol_n, h):
    k = np.zeros((num_of_stages, num_of_equations))

    k_matrix = np.linalg.inv(np.identity(num_of_equations) - h * a_table[0][0] * equasion_matrix(sol_n))
    for i in range(num_of_stages):
        k[i] = solve_newton(k_matrix, sol_n + h * sum(a_table[i][j] * k[j] for j in range(i)), h , a_table[i][i], equasion)

    tmp = sol_n + h * np.dot(b_row, k)
    return tmp


tau = []
sol_values = ([], [], [], [])
x = np.float64(0.0)
y = begin_values()
while x <= end_time:
    y_tmp = runge_kutta(y, h)
    y = y_tmp
    tau.append(x)
    x += h
    for i in range(num_of_equations):
        sol_values[i].append(y[i])

fig, ax = plt.subplots(figsize=(16, 9)) 

for i in range(num_of_equations):
    ax.plot(tau, sol_values[i])

ax.set_title("runge rutta", fontsize=20)
ax.set_xlabel("tau", fontsize=16)
ax.set_ylabel("x1, x2, y2, y2", fontsize=16)
ax.grid(True)
fig.savefig(os.path.join("LabDU2/solv.png"), dpi=100) 
