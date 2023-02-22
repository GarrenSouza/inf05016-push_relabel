import sys
import os
import subprocess

instances_folder = "../analysis/"

commands_list = []

create_results_file = f"touch {instances_folder}results.txt"
commands_list.append(create_results_file)

build_command = "cmake ../../ -DCMAKE_BUILD_TYPE=Release -Bbuild/Release"
commands_list.append(build_command)

run_build = "cmake --build build/Release"
commands_list.append(run_build)

for i in range(1, 7):
    command = f"time (./build/Release/push-relabel < {instances_folder}I{i}.ins > {instances_folder}I{i}.sol) >> {instances_folder}results.txt 2>&1"
    commands_list.append(command)
    command = f"./build/Release/opm --ins {instances_folder}I{i}.ins --sol {instances_folder}I{i}.sol --pbm {instances_folder}vis-{i}.pbm"
    commands_list.append(command)
    command = f"convert {instances_folder}vis-{i}.pbm {instances_folder}vis-{i}.png"
    commands_list.append(command)

for c in commands_list:
    print(f'about to run: \'{c}\'')
    p = subprocess.Popen(c, shell=True, executable='/bin/bash')
    p.wait()
