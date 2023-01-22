import sys, subprocess

bin_folder = "./bin"
graph_generator_bin = "graph-generator"
push_relabel_bin = "push-relabel"
graph_files_folder = "./graphs"

# for mesh, random level and random 2-level

u = [512, 1024, 2048, 4096, 8192]
v = [16, 32, 64, 128, 256]
w = [2, 4, 8, 16]

r = 1000

commands = []

for k in range(1, 4):
    for i in v:
        for j in v:
            graph_file_name = f"g_{k}_{i}_{j}_{r}.gr"
            command = f"{bin_folder}/{graph_generator_bin} {k} {i} {j} {r} {graph_files_folder}/{graph_file_name}"
            commands.append(command)

# for matching
k = 4
for i in v:
    for j in w:
        graph_file_name = f"g_{k}_{i}_{j}_{r}.gr"
        command = f"{bin_folder}/{graph_generator_bin} {4} {i} {j} {r} {graph_files_folder}/{graph_file_name}"
        commands.append(command)

# for square mesh
k = 5
for i in v:
    for j in v:
        j = j - 1
        graph_file_name = f"g_{k}_{i}_{j}_{r}.gr"
        command = f"{bin_folder}/{graph_generator_bin} {k} {i} {j} {r} {graph_files_folder}/{graph_file_name}"
        commands.append(command)

# for basic line,  exp line, dexp line
for l in range(6, 9):
    for i in v:
        for j in v:
            for k in w:
                graph_file_name = f"g_{l}_{i}_{j}_{k}{r}.gr"
                command = f"{bin_folder}/{graph_generator_bin} {l} {i} {j} {k} {r} {graph_files_folder}/{graph_file_name}"
                commands.append(command)


# dinic bad
k = 9
for j in u:
    graph_file_name = f"g_{k}_{j}.gr"
    command = f"{bin_folder}/{graph_generator_bin} {k} {j} {graph_files_folder}/{graph_file_name}"
    commands.append(command)


# gold bad
k = 10
for j in u:
    graph_file_name = f"g_{k}_{j}.gr"
    command = f"{bin_folder}/{graph_generator_bin} {k} {j} {graph_files_folder}/{graph_file_name}"
    commands.append(command)

for c in commands:
    print(f"echo \"about to run '{c}'...\"")
    print(c)
