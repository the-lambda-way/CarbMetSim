import os
import shutil
import subprocess


# For each id:
#     run the sim with ExerciseArm30_id.txt and ParamsNormalArm30_id.txt outputting to output/outAhlborgArm30_id.txt
#     if there was an error, print a message and stop
#     compare outAhlborg58_id.txt with expected/outAhlborgArm30_id.txt.expected
#     if there was a difference, save it to outAhlborgArm30.diff, print a message, and stop


def recreate_output_folder():
    if os.path.exists("output/"):
        shutil.rmtree("output/")
    os.mkdir("output")


def run_sim(id):
    exercise_file = "ExerciseArm30_" + str(id) + ".txt"
    params_file   = "ParamsNormalArm30_" + str(id) + ".txt"
    output_file   = "output/outAhlborgArm30_" + str(id) + ".txt"
    command = ["../../../carbmetsim", "Food.txt", exercise_file, params_file, "Events30.txt", "alpha", output_file]

    return subprocess.run(command).returncode == 0


def diff_output(id):
    output_file   = "output/outAhlborgArm30_" + str(id) + ".txt"
    expected_file = "expected/outAhlborgArm30_" + str(id) + ".txt.expected"
    command = "diff -u --color " + output_file + " " + expected_file + " > outAhlborgArm30.diff"

    return subprocess.run(command, shell=True).returncode == 0


# Script
print("Testing exercise-event-AhlborgArm30...")
recreate_output_folder()

for id in range(1, 7):
    if not run_sim(id):
        print("Failed to complete at subject", id)
        break

    if not diff_output(id):
        print("Test failed at subject", id)
        print("Differences written to outAhlborgArm30.diff")
        break
else: # only if all seeds finished
    os.remove("outAhlborgArm30.diff")
    print("Success!")
