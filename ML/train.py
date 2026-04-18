import numpy as np

X = np.load("X.npy")
y = np.load("y.npy")

# Normalize
X = X / np.max(X, axis=1, keepdims=True)

classes = 5
features = 5

means = np.zeros((classes, features))
vars_ = np.zeros((classes, features))

for c in range(classes):
    X_c = X[y == c]
    means[c] = np.mean(X_c, axis=0)
    vars_[c] = np.var(X_c, axis=0) + 1e-6

np.save("means.npy", means)
np.save("vars.npy", vars_)

print("Model trained!")