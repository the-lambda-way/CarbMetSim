import subprocess
import math
import os

def read_seeds():
    s = []
    seed_file = open('seeds', 'r')
    for seed in seed_file:
        s.append(seed[:seed.find('\n')])
    return s

def execute_sims_normal():
    seeds = read_seeds()
    for seed in seeds:
        output_file = 'output/outNormal.' + seed
        if os.path.isfile(output_file):
            os.remove(output_file)
        subprocess.call(["../../carbmetsim", "Food.txt", "Exercise.txt", "ParamsNormal.txt", "Events.txt", seed, output_file])

def execute_sims_diab():
    seeds = read_seeds()
    for seed in seeds:
        output_file = 'output/outDiab.' + seed
        if os.path.isfile(output_file):
            os.remove(output_file)
        subprocess.call(["../../carbmetsim", "Food.txt", "Exercise.txt", "ParamsDiab.txt", "Events.txt", seed, output_file])

if not os.path.exists('output/'):
    os.mkdir('output')

execute_sims_normal()
execute_sims_diab()
