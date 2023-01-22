import subprocess, os

graph_files_folder = "./graphs"
max_flow_bin = "./bin/push-relabel"
data_folder = "./data"

graphs = os.listdir(graph_files_folder)

for g in graphs:
    output_file = g.replace(".gr", ".csv")
    command = f"{max_flow_bin} < {graph_files_folder}/{g} > {data_folder}/{output_file}"
    print(f"echo \"about to run '{command}'...\"")
    print(command)

