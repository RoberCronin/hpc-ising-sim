import os
import subprocess
import shutil

COMMITS_TO_BENCHMARK = [
    "main",
]


def automate_historical_sweep():
    original_branch = (
        subprocess.check_output(["git", "rev-parse", "--abbrev-ref", "HEAD"])
        .decode("utf-8")
        .strip()
    )

    print(f"Starting sweep. Current git location saved: {original_branch}")

    try:
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


if __name__ == "__main__":
    automate_historical_sweep()
