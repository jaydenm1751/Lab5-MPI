#!/usr/bin/env python3
import subprocess
import csv
from pathlib import Path

# Config
EXE = "./lab5_mpi"
INPUT_DIR = Path("input")
OUTPUT_CSV = Path("perf_results.csv")

# How many times to repeat each configuration for averaging
TRIALS = 5

# Simulation parameters (adjust to what you actually want)
THETA = 0.5
DT = 0.005

# Per-input settings: steps and max MPI ranks
CONFIGS = [
    {
        "input_file": "nb-10.txt",
        "steps": 1000,
        "max_np": 16,   # can go up to 16 for small input
    },
    {
        "input_file": "nb-100.txt",
        "steps": 1000,
        "max_np": 16,
    },
    {
        "input_file": "nb-100000.txt",
        "steps": 1000,  # maybe 100 if Codio is slow
        "max_np": 8,    # spec: at most 8 processors for large input
    },
]


def run_one(input_path, steps, np, theta, dt):
    """
    Run one mpirun configuration and return (total_time, sim_time) as floats.

    Assumes the program prints two lines at the end:
        <total_elapsed>
        <sim_elapsed>
    on rank 0.
    """
    # For performance runs, we don't care about the bodies output.
    # Use -o /dev/null to avoid disk I/O overhead.
    cmd = [
        "mpirun",
        "-np", str(np),
        EXE,
        "-i", str(input_path),
        "-o", "/dev/null",
        "-s", str(steps),
        "-t", str(theta),
        "-d", str(dt),
    ]

    result = subprocess.run(
        cmd,
        stdout=subprocess.PIPE,
        stderr=subprocess.PIPE,
        universal_newlines=True,   # same effect as text=True in Py3.6
        check=True,
    )


    # Grab the last two non-empty lines from stdout
    lines = [ln for ln in result.stdout.strip().splitlines() if ln.strip()]

    if len(lines) < 2:
        raise RuntimeError(
            f"Expected at least 2 lines of output (total, sim); got {len(lines)}.\n"
            f"Full stdout:\n{result.stdout}\nStderr:\n{result.stderr}"
        )

    total_str = lines[-2].strip()
    sim_str = lines[-1].strip()

    total_time = float(total_str)
    sim_time = float(sim_str)
    return total_time, sim_time


def main():
    # Prepare CSV and write header
    with OUTPUT_CSV.open("w", newline="") as f:
        writer = csv.writer(f)
        writer.writerow(["input", "steps", "np", "trial", "total_time", "sim_time"])

        for cfg in CONFIGS:
            input_file = cfg["input_file"]
            steps = cfg["steps"]
            max_np = cfg["max_np"]

            input_path = INPUT_DIR / input_file
            base_name = input_file.replace(".txt", "")

            print(f"=== Input: {input_file}, steps={steps}, max_np={max_np} ===")

            # Use powers of two: 1, 2, 4, 8, 16 (clamped by max_np)
            np = 1
            while np <= max_np:
                print(f"  -> np={np}")
                for trial in range(1, TRIALS + 1):
                    print(f"     trial {trial}...", end="", flush=True)
                    total_time, sim_time = run_one(input_path, steps, np, THETA, DT)
                    print(f"  total={total_time:.6f} sim={sim_time:.6f}")

                    writer.writerow([
                        base_name,
                        steps,
                        np,
                        trial,
                        f"{total_time:.6f}",
                        f"{sim_time:.6f}",
                    ])
                np *= 2

    print(f"Done. Results written to {OUTPUT_CSV}")


if __name__ == "__main__":
    main()
