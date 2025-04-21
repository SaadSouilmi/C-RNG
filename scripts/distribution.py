from concurrent.futures import ThreadPoolExecutor
from argparse import ArgumentParser, Namespace

import pandas as pd 
import numpy as np 
import matplotlib.pyplot as plt 

def process_lines(lines):
    return [float(line.strip()) for line in lines if line.strip()]


def plot_discrete_distribution(sample: np.ndarray) -> None:
    fig, (ax1, ax2) = plt.subplots(1, 2, figsize=(15, 5))

    value_counts = pd.Series(sample).value_counts(normalize=True).sort_index()

    value_counts.plot(kind="bar", edgecolor="k", ax=ax1)
    ax1.set_title("Empirical Distributions")

    value_counts.cumsum().plot(kind="bar", edgecolor="k", ax=ax2)
    ax2.set_title("Empirical CDF")
        
    plt.show()
    plt.close()

def plot_continuous_distribution(sample: np.ndarray) -> None:
    fig, (ax1, ax2) = plt.subplots(1, 2, figsize=(15, 5))

    ax1.hist(sample, density=True, bins=80)
    ax1.set_title(f"mean = {sample.mean()}, std = {sample.std(ddof=1)}")
    
    ax2.plot(sample, np.arange(1, len(sample) +1 )/len(sample))
    ax2.set_title("Empirical CDF")

    plt.show()
    plt.close()


def parse_args() -> Namespace:
    parser = ArgumentParser()
    parser.add_argument("-f", "--file", type=str, required=True, help="file containing the sample values")
    parser.add_argument("--discrete", action="store_true", help="if provided plots a discrete distribution")
    return parser.parse_args()


def main() -> None:
    args = parse_args()

    with open(args.file, "r") as file:
        lines = file.readlines()
    sample = np.sort(process_lines(lines))
    if args.discrete:
        plot_discrete_distribution(sample)
    else:
        plot_continuous_distribution(sample)


if __name__ == "__main__":
    main()
