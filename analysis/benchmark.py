import os
import json
import timeit
import subprocess
from datetime import datetime, timezone
import numpy as np


def run_performance_benchmark(save_latest=False):
    LATTICE_SIZE = 512
    TEMPERATURE = 2.269
    ITERATIONS = 10_000_000
    NUM_TRIALS = 5

    try:
        git_proc = subprocess.run(
            ["git", "rev-parse", "--short", "HEAD"],
            capture_output=True,
            text=True,
            check=True,
        )
        commit_hash = git_proc.stdout.strip()
    except Exception:
        commit_hash = "unknown"

    setup_code = (
        f"import libising; engine = libising.IsingEngine({LATTICE_SIZE}, {TEMPERATURE})"
    )
    stmt_code = f"engine.step_metropolis({ITERATIONS})"

    print("\tExecuting warmup cycle...")
    timeit.timeit(stmt=stmt_code, setup=setup_code, number=1)

    print(f"\tRunning {NUM_TRIALS} trials...")
    raw_times = timeit.repeat(
        stmt=stmt_code,
        setup=setup_code,
        repeat=NUM_TRIALS,
        number=1,
    )

    mean_time = float(np.mean(raw_times))
    median_time = float(np.median(raw_times))
    std_dev = float(np.std(raw_times))
    min_time = float(np.min(raw_times))
    max_time = float(np.max(raw_times))

    benchmark_data = {
        "metadata": {
            "timestamp": datetime.now(timezone.utc).strftime("%Y-%m-%d %H:%M:%S UTC"),
            "commit_hash": commit_hash,
            "lattice_size": LATTICE_SIZE,
            "temperature": TEMPERATURE,
            "iterations": ITERATIONS,
            "num_trials": NUM_TRIALS,
        },
        "raw_metrics": raw_times,
        "statistics": {
            "mean": mean_time,
            "median": median_time,
            "std_dev": std_dev,
            "min_time": min_time,
            "max_time": max_time,
        },
    }

    os.makedirs(".benchmarks", exist_ok=True)

    # save results
    timestamp_slug = datetime.now(timezone.utc).strftime("%Y%m%d_%H%M%S")
    history_file = f".benchmarks/{timestamp_slug}_{commit_hash}.json"
    latest_file = "analysis/benchmark_results.json"

    with open(history_file, "w") as f:
        json.dump(benchmark_data, f, indent=4)

    if save_latest:
        with open(latest_file, "w") as f:
            json.dump(benchmark_data, f, indent=4)

    print(f"\tMean {mean_time:.4f}s")
    if save_latest:
        print(f"\tData saved to: {history_file} and {latest_file}")
    else:
        print(f"\tData saved to: {history_file}")


if __name__ == "__main__":
    run_performance_benchmark(save_latest=False)
