import os
import shutil
import subprocess


# For each id:
#     run the sim with Exercise58_id.txt and ParamsNormal58_id.txt outputting to output/outAhlborg58_id.txt
#     if there was an error, print a message and stop
#     compare outAhlborg58_id.txt with expected/outAhlborg58_id.txt.expected
#     if there was a difference, save it to outAhlborg58.diff, print a message, and stop


def recreate_output_folder():
    if os.path.exists("output/"):
        shutil.rmtree("output/")
    os.mkdir("output")


def run_sim(id):
    exercise_file = "Exercise58_" + str(id) + ".txt"
    params_file   = "ParamsNormal58_" + str(id) + ".txt"
    output_file   = "output/outAhlborg58_" + str(id) + ".txt"
    command = ["../../../carbmetsim", "Food.txt", exercise_file, params_file, "Events58.txt", "alpha", output_file]

    return subprocess.run(command).returncode == 0


def diff_output(id):
    output_file   = "output/outAhlborg58_" + str(id) + ".txt"
    expected_file = "expected/outAhlborg58_" + str(id) + ".txt.expected"
    command = "diff -u --color " + output_file + " " + expected_file + " > outAhlborg58.diff"

    return subprocess.run(command, shell=True).returncode == 0


# Script
print("Testing exercise-event-Ahlborg58...")
recreate_output_folder()

for id in range(1, 7):
    if not run_sim(id):
        print("Failed to complete at subject", id)
        break

    if not diff_output(id):
        print("Test failed at subject", id)
        print("Differences written to outAhlborg58.diff")
        break
else: # only if all seeds finished
    os.remove("outAhlborg58.diff")
    print("Success!")
