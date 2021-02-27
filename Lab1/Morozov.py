import numpy as np
import matplotlib.pyplot as plt
import os

approximation_order = 1 # 1 or 4
num_of_stages = 4
num_of_eq = 2 # x' = y; y' = x ** 2 - 1
h = 0.001

f = open('text1.txt', 'w')

# system x' = y; y' = x ** 2 - 1
def equations(sol_n):
    f.write(str(sol_n[1]) + ' ' + str(sol_n[0] * sol_n[0] - 1) + '\n')
    return np.array([sol_n[1], sol_n[0] * sol_n[0] - 1])
 
# Not using column c in table cause it's equal to a_table[i][i-1] i = 1,2,3
def butcher_table(approximation_order):
    if approximation_order == 1:
        a_table = np.zeros((num_of_stages, num_of_stages))
        a_table[1][0] = a_table[2][1] = a_table[3][2] = 0.5
        b_row = np.array([0.25, 0.25, 0.25, 0.25])
        return (a_table, b_row)
    elif approximation_order == 4:
        a_table = np.zeros((num_of_stages, num_of_stages))
        a_table[1][0] = a_table[2][1] = 0.5
        a_table[3][2] = 1.0
        b_row = np.array([1.0 / 6.0, 1.0 / 3.0, 1.0 / 3.0, 1.0 / 6.0])
        return (a_table, b_row)

def runge_kutta_positive(sol_n, h):
    a_table, b_row = butcher_table(approximation_order)
    k_matrix = np.zeros((num_of_stages, num_of_eq))
    # Counting additional coefs k in runge kutta method
    for i in range(num_of_stages):
        tmp = a_table[i][0] * k_matrix[0]
        if i > 0:
            for j in range(1, i):
                tmp += a_table[i][j] * k_matrix[j]
        k_matrix[i] = equations(sol_n + h * tmp)
    # Counting next point knowing all at current point
    return sol_n + h * np.dot(b_row, k_matrix)

#____________________________
# For start value x > 1
#____________________________

points_0 = []
sol_values_0 = ([], [])

# dx/dtau = ... ; dy/dtau = ...
tau = 0.0

# begin condition
sol_n = np.array([1.001 , 0])
h = 0.001
while tau <= 7:
    points_0.append(tau)
    sol_n = runge_kutta_positive(sol_n, h)
    tau += h
    for i in range(2):
        sol_values_0[i].append(sol_n[i])

#____________________________
points_1 = []
sol_values_1 = ([], [])

# dx/dtau = ... ; dy/dtau = ...
tau = 0.0

# begin condition
sol_n = np.array([1.001 , 0])
h = -0.001
while tau >= -7:
    points_1.append(tau)
    sol_n = runge_kutta_positive(sol_n, h)
    tau += h
    for i in range(2):
        sol_values_1[i].append(sol_n[i])


fig, ax = plt.subplots(figsize=(16, 9)) 
ax.plot(points_0, sol_values_0[0])
ax.plot(points_1, sol_values_1[0])
ax.set_title('Solution for x')
ax.set_xlabel('tau')
ax.set_ylabel('x')
ax.grid(True)
fig.savefig(os.path.join("Lab1/x_more_1_1.png"), dpi=100) 

fig, ax = plt.subplots(figsize=(16, 9)) 
ax.plot(points_0, sol_values_0[1])
ax.plot(points_1, sol_values_1[1])
ax.set_title('Solution for y')
ax.set_xlabel('tau')
ax.set_ylabel('y')
ax.grid(True)
fig.savefig(os.path.join("Lab1/x_more_1_2.png"), dpi=100) 

fig, ax = plt.subplots(figsize=(16, 9)) 
ax.plot(sol_values_0[0], sol_values_0[1])
ax.plot(sol_values_1[0], sol_values_1[1])
ax.set_title('Phase diagramm')
ax.set_xlabel('x')
ax.set_ylabel('y')
ax.grid(True)
fig.savefig(os.path.join("Lab1/x_more_1_3.png"), dpi=100) 



#____________________________
# For start value x < 1
#____________________________

points_0 = []
sol_values_0 = ([], [])

# dx/dtau = ... ; dy/dtau = ...
tau = 0.0

# begin condition
sol_n = np.array([-1.999 , 0])
h = 0.001
while tau <= 10:
    points_0.append(tau)
    sol_n = runge_kutta_positive(sol_n, h)
    tau += h
    for i in range(2):
        sol_values_0[i].append(sol_n[i])

#____________________________
points_1 = []
sol_values_1 = ([], [])

# dx/dtau = ... ; dy/dtau = ...
tau = 0.0

# begin condition
sol_n = np.array([-1.999 , 0])
h = -0.001
while tau >= -10:
    points_1.append(tau)
    sol_n = runge_kutta_positive(sol_n, h)
    tau += h
    for i in range(2):
        sol_values_1[i].append(sol_n[i])


fig, ax = plt.subplots(figsize=(16, 9)) 
ax.plot(points_0, sol_values_0[0])
ax.plot(points_1, sol_values_1[0])
ax.set_title('Solution for x')
ax.set_xlabel('tau')
ax.set_ylabel('x')
ax.grid(True)
fig.savefig(os.path.join("Lab1/x_less_1_1.png"), dpi=100) 

fig, ax = plt.subplots(figsize=(16, 9)) 
ax.plot(points_0, sol_values_0[1])
ax.plot(points_1, sol_values_1[1])
ax.set_title('Solution for y')
ax.set_xlabel('tau')
ax.set_ylabel('y')
ax.grid(True)
fig.savefig(os.path.join("Lab1/x_less_1_2.png"), dpi=100) 

fig, ax = plt.subplots(figsize=(16, 9)) 
ax.plot(sol_values_0[0], sol_values_0[1])
ax.plot(sol_values_1[0], sol_values_1[1])
ax.set_title('Phase diagramm')
ax.set_xlabel('x')
ax.set_ylabel('y')
ax.grid(True)
fig.savefig(os.path.join("Lab1/x_less_1_3.png"), dpi=100) 
