import numpy as np
import matplotlib.pyplot as plt
import os
import math

def equasion(y):
    return math.exp(y)

def equasion_2(y):
    return -1 * math.exp(y)

def shoot(alpha, equasion):
    curr_x = 0
    curr_sol = 1
    x = []
    sol = []
    x.append(0)
    sol.append(1)
    h = 0.001
    while curr_x < 1:
        curr_x += h
        curr_sol += alpha * h
        alpha += equasion(curr_sol) * h
        x.append(curr_x)
        sol.append(curr_sol)
    return x, sol

alpha_high = 0
x1, sol1 = shoot(alpha_high, equasion)
alpha_low = -2
x2, sol2 = shoot(alpha_low, equasion)

boundary_condition = 2
presicion = 0.01
while 1:
    alpha = (alpha_high + alpha_low) / 2
    x, sol = shoot(alpha, equasion)
    if (abs(sol[-1] - boundary_condition) < presicion):
        break
    if sol[-1] > boundary_condition:
        alpha_high = alpha
    elif  sol[-1] < boundary_condition:
        alpha_low = alpha
    else:
        print("BONG")
        break

fig, ax = plt.subplots(figsize=(16, 9)) 
ax.plot(x1, sol1, label = "begin high solution")
ax.plot(x2, sol2, label = "begin low solution")
ax.plot(x, sol, label = "Computed solution")
ax.set_title('Solution for y(1) = 2')
ax.set_xlabel('x')
ax.set_ylabel('y')
ax.grid(True)
ax.legend()
fig.savefig(os.path.join("LabShooting/b_eq_2.png"), dpi=100) 


alpha_high = 0
x1, sol1 = shoot(alpha_high, equasion)
alpha_low = -2
x2, sol2 = shoot(alpha_low, equasion)

boundary_condition = 1
presicion = 0.01
while 1:
    alpha = (alpha_high + alpha_low) / 2
    x, sol = shoot(alpha, equasion)
    if (abs(sol[-1] - boundary_condition) < presicion):
        break
    if sol[-1] > boundary_condition:
        alpha_high = alpha
    elif  sol[-1] < boundary_condition:
        alpha_low = alpha
    else:
        print("BONG")
        break

fig, ax = plt.subplots(figsize=(16, 9)) 
ax.plot(x1, sol1, label = "begin high solution")
ax.plot(x2, sol2, label = "begin low solution")
ax.plot(x, sol, label = "Computed solution")
ax.set_title('Solution for y(1) = 1')
ax.set_xlabel('x')
ax.set_ylabel('y')
ax.grid(True)
ax.legend()
fig.savefig(os.path.join("LabShooting/b_eq_1.png"), dpi=100) 


alpha_high = 4.5
x1, sol1 = shoot(alpha_high, equasion_2)
alpha_low = 0
x2, sol2 = shoot(alpha_low, equasion_2)

boundary_condition = 1
presicion = 0.01
while 1:
    alpha = (alpha_high + alpha_low) / 2
    x, sol = shoot(alpha, equasion_2)
    if (abs(sol[-1] - boundary_condition) < presicion):
        break
    if sol[-1] > boundary_condition:
        alpha_high = alpha
    elif  sol[-1] < boundary_condition:
        alpha_low = alpha
    else:
        print("BONG")
        break

fig, ax = plt.subplots(figsize=(16, 9)) 
ax.plot(x1, sol1, label = "begin high solution")
ax.plot(x2, sol2, label = "begin low solution")
ax.plot(x, sol, label = "Computed solution")
ax.set_title('Solution for y(1) = 1')
ax.set_xlabel('x')
ax.set_ylabel('y')
ax.grid(True)
ax.legend()
fig.savefig(os.path.join("LabShooting/sec_eq.png"), dpi=100) 


