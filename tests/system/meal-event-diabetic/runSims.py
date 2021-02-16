import os
import shutil
import subprocess


# For each seed:
#     run the sim with seed outputting to output/outDiab.seed
#     if there was an error, print a message and stop
#     compare outDiab.seed with expected/outDiab.expected.seed
#     if there was a difference, save it to outDiab.diff.seed, print a message, and stop


def recreate_output_folder():
    if os.path.exists("output/"):
        shutil.rmtree("output/")
    os.mkdir("output")


def run_sim(seed):
    output_file = "output/outDiab." + seed
    command = ["../../../carbmetsim", "Food.txt", "Exercise.txt", "ParamsDiab.txt", "Events.txt", seed, output_file]

    return subprocess.run(command).returncode == 0


def diff_output(seed):
    output_file   = "output/outDiab." + seed
    expected_file = "expected/outDiab." + seed + ".expected"
    command = "diff -u --color " + output_file + " " + expected_file + " > outDiab.diff"

    return subprocess.run(command, shell=True).returncode == 0


# Script
print("Testing meal-event-diabetic...")
recreate_output_folder()

with open('seeds') as seed_file:
    for seed in seed_file.read().splitlines():
        if not run_sim(seed):
            print("Failed to complete at seed", seed)
            break

        if not diff_output(seed):
            print("Test failed at seed", seed)
            print("Differences written to outDiab.diff")
            break
    else: # only if all seeds finished
        os.remove("outDiab.diff")
        print("Success!")
