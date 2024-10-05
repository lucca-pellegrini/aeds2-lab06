import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns
import sys
from matplotlib import rcParams

# Set up LaTeX and Palatino font for matplotlib
def setup_plot_style():
    rcParams['text.usetex'] = True
    rcParams['font.family'] = 'serif'
    rcParams['font.serif'] = ['Palatino']
    sns.set_theme(style='whitegrid')


def plot_data(data, title, filename):
    plt.figure(figsize=(10, 6))
    sns.lineplot(
        data=data,
        x='num',
        y=data['time'] / 1e3,
        hue='strat',
        marker='',
        linewidth=1,
    )
    plt.title(title)
    plt.xlabel('Array Size (num)')
    plt.ylabel('Execution Time (ms)')
    plt.legend(title='Pivot Strategy')
    plt.yscale('log')  # Log scale for better visibility of differences
    plt.tight_layout()
    plt.savefig(filename, dpi=600, format='svg')
    # plt.show()


def plot_all_data(data):
    vector_types = data['type'].unique()
    for vector_type in vector_types:
        subset = data[data['type'] == vector_type]
        title = (
            f'Performance of Quicksort Strategies for {vector_type} Vectors'
        )
        filename = f'build/fig/{vector_type}_all_data.svg'
        plot_data(subset, title, filename)


def plot_specific_sizes(data, sizes):
    filtered_data = data[data['num'].isin(sizes)]
    vector_types = filtered_data['type'].unique()
    for vector_type in vector_types:
        subset = filtered_data[filtered_data['type'] == vector_type]
        title = f'Performance of Quicksort Strategies for {vector_type} Vectors (Specific Sizes)'
        filename = f'build/fig/{vector_type}_specific_sizes.svg'
        plot_data(subset, title, filename)


def main(arguments):
    # Load the CSV data
    data = pd.read_csv('build/main.csv')

    # Determine which plots to create
    plot_full = 'full' in arguments or len(arguments) == 0
    plot_selected = 'selected' in arguments or len(arguments) == 0

    # Set up plot style
    setup_plot_style()

    # Plot data as requested
    if plot_full:
        plot_all_data(data)
    if plot_selected:
        plot_specific_sizes(data, [100, 1000, 10000])


if __name__ == '__main__':
    main(sys.argv[1:])