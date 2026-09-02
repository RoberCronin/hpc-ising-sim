import os
import subprocess
import shutil

COMMITS_TO_BENCHMARK = [
    "main",
    "8c63d698594744d943f7f10770de4f62ed056eb0",
]


def automate_historical_sweep():
    original_branch = (
        subprocess.check_output(["git", "rev-parse", "--abbrev-ref", "HEAD"])
        .decode("utf-8")
        .strip()
    )

    print(f"Starting sweep. Current git location saved: {original_branch}")
    status_output = (
        subprocess.check_output(["git", "status", "--porcelain"])
        .decode("utf-8")
        .strip()
    )
    has_dirty_changes = len(status_output) > 0

    try:
        if has_dirty_changes:
            print("\nBenchmarking active workspace (uncommitted modifications)...")
            print("\tRecompiling C++ bindings for active workspace...")
            if os.path.exists("libising/build"):
                shutil.rmtree("libising/build")

            subprocess.run(
                ["devenv", "--quiet", "shell", "--", "benchmark"],
                check=True,
            )

            subprocess.run(
                ["git", "stash", "push", "-m", "generate_historical_data"],
                check=True,
            )

        for commit in COMMITS_TO_BENCHMARK:
            print(f"\nBenchmarking commit: {commit}")

            subprocess.run(
                ["git", "checkout", commit], check=True, stdout=subprocess.DEVNULL
            )

            print("\tRecompiling C++ bindings for this version...")
            if os.path.exists("libising/build"):
                shutil.rmtree("libising/build")

            # use devenv to make sure the environment matches the legacy commit
            subprocess.run(
                ["devenv", "--quiet", "shell", "--", "benchmark"],
                check=True,
            )

    finally:
        print(f"\nCleaning up. Returning workspace to branch: {original_branch}")
        subprocess.run(
            ["git", "checkout", original_branch], check=True, stdout=subprocess.DEVNULL
        )
        if has_dirty_changes:
            subprocess.run(["git", "stash", "pop"], check=True)


if __name__ == "__main__":
    automate_historical_sweep()
