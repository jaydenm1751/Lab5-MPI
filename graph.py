#!/usr/bin/env python3
import sys
import csv
from collections import defaultdict, OrderedDict

import matplotlib
matplotlib.use("Agg")  # no GUI needed
import matplotlib.pyplot as plt


def load_data(csv_path):
    """
    Returns:
      data[input_name][np] = list of sim_times (one per trial)
    """
    data = defaultdict(lambda: defaultdict(list))

    with open(csv_path, "r") as f:
        reader = csv.DictReader(f)
        for row in reader:
            input_name = row["input"]
            np = int(row["np"])
            sim_time = float(row["sim_time"])
            data[input_name][np].append(sim_time)

    # convert lists -> average
    avg_data = {}
    for input_name, np_dict in data.items():
        avg_data[input_name] = {}
        for np, times in np_dict.items():
            avg_data[input_name][np] = sum(times) / len(times)

    return avg_data


def plot_sim_time(avg_data, out_path="simtime_vs_np.png"):
    """
    One plot: sim time vs np, with one curve per input size.
    """
    plt.figure(figsize=(8, 5))

    for input_name, np_dict in sorted(avg_data.items()):
        # sort by np
        nps = sorted(np_dict.keys())
        times = [np_dict[np] for np in nps]
        label = f"{input_name} particles"
        plt.plot(nps, times, marker="o", label=label)

    plt.xlabel("Number of Processes")
    plt.ylabel("Average Simulation Time (s)")
    plt.title("Simulation Time vs Number of Processes")
    plt.grid(True, linestyle="--", alpha=0.4)
    plt.legend()
    plt.tight_layout()
    plt.savefig(out_path, dpi=200)
    plt.close()


def plot_speedup(avg_data, out_path="speedup_vs_np.png"):
    """
    One plot: speedup vs np, with one curve per input size.
    Speedup(np) = T(1) / T(np) where T(1) is the np=1 average sim_time.
    """
    plt.figure(figsize=(8, 5))

    for input_name, np_dict in sorted(avg_data.items()):
        # need np=1 as baseline; skip if missing
        if 1 not in np_dict:
            continue

        baseline = np_dict[1]
        nps = sorted(np_dict.keys())
        speedups = [baseline / np_dict[np] for np in nps]

        label = f"{input_name} particles"
        plt.plot(nps, speedups, marker="o", label=label)

    plt.xlabel("Number of Processes")
    plt.ylabel("Speedup (T1 / Tnp)")
    plt.title("Speedup vs Number of Processes")
    plt.grid(True, linestyle="--", alpha=0.4)
    plt.legend()
    plt.tight_layout()
    plt.savefig(out_path, dpi=200)
    plt.close()


def main():
    if len(sys.argv) < 2:
        print("Usage: python3 graph.py perf_results.csv")
        sys.exit(1)

    csv_path = sys.argv[1]
    avg_data = load_data(csv_path)

    plot_sim_time(avg_data, "simtime_vs_np_1.png")
    plot_speedup(avg_data, "speedup_vs_np_1.png")
    print("Wrote simtime_vs_np.png and speedup_vs_np.png")


if __name__ == "__main__":
    main()
