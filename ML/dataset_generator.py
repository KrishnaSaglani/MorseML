import numpy as np

def generate(letter):
    if letter == 'A': return [100,100,300,0,0]
    if letter == 'B': return [300,100,100,100,100]
    if letter == 'C': return [300,100,300,100,0]
    if letter == 'D': return [300,100,100,0,0]
    if letter == 'E': return [100,0,0,0,0]

X, y = [], []
labels = ['A','B','C','D','E']

for i, l in enumerate(labels):
    for _ in range(300):
        base = np.array(generate(l))
        noise = np.random.normal(0, 15, 5)
        X.append(base + noise)
        y.append(i)

np.save("X.npy", np.array(X))
np.save("y.npy", np.array(y))