import numpy as np

means = np.load("means.npy")
vars_ = np.load("vars.npy")

labels = ['A','B','C','D','E']

with open("model_params.h", "w") as f:
    f.write("#ifndef MODEL_PARAMS_H\n#define MODEL_PARAMS_H\n\n")

    f.write("static float means[5][5] = {\n")
    for row in means:
        f.write("{" + ",".join(f"{v:.4f}" for v in row) + "},\n")
    f.write("};\n\n")

    f.write("static float vars[5][5] = {\n")
    for row in vars_:
        f.write("{" + ",".join(f"{v:.4f}" for v in row) + "},\n")
    f.write("};\n\n")

    f.write("static char labels[5] = {'A','B','C','D','E'};\n\n")

    f.write("#endif\n")

print("model_params.h generated!")