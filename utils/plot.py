#!/usr/bin/env python

import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns
import sys
import argparse
from matplotlib import rcParams

# Set up LaTeX and Palatino font for matplotlib
def setup_plot_style():
    rcParams['text.usetex'] = True
    rcParams['font.family'] = 'serif'
    rcParams['font.serif'] = ['Palatino']
    sns.set_theme(style='whitegrid')


def plot_data(data, title, filename, yscale):
    plt.figure(figsize=(10, 6))
    sns.lineplot(
        data=data,
        x='num',
        y=data['time'] / 1e9,
        hue='strat',
        marker='',
        linewidth=1,
    )
    plt.title(title)
    plt.xlabel('Tamanho do Arranjo (num)')
    plt.ylabel('Tempo de Execução (s)')
    plt.legend(title='Pivô')
    plt.yscale(yscale)  # Set the y-axis scale
    plt.tight_layout()
    plt.savefig(filename, dpi=900, format='svg')
    # plt.show()


def plot_all_data(data, yscale):
    vector_types = data['type'].unique()
    for vector_type in vector_types:
        subset = data[data['type'] == vector_type]
        title = f'Performance das Estratégias de Escolha de Pivô com Vetores {vector_type} (Escala {yscale.capitalize()})'
        filename = f'build/fig/{vector_type}_all_data_{yscale}.svg'
        plot_data(subset, title, filename, yscale)


def plot_specific_sizes(data, sizes, yscale):
    filtered_data = data[data['num'].isin(sizes)]
    vector_types = filtered_data['type'].unique()
    for vector_type in vector_types:
        subset = filtered_data[filtered_data['type'] == vector_type]
        title = f'Performance das Estratégias de Escolha de Pivô com Vetores {vector_type} (Pontos Selecionados, Escala {yscale.capitalize()})'
        filename = f'build/fig/{vector_type}_specific_sizes_{yscale}.svg'
        plot_data(subset, title, filename, yscale)


def main(arguments):
    parser = argparse.ArgumentParser(
        description='Plot quicksort performance data.'
    )
    parser.add_argument(
        '--linear', action='store_true', help='Use linear scale for y-axis'
    )
    parser.add_argument(
        '--log', action='store_true', help='Use logarithmic scale for y-axis'
    )
    args = parser.parse_args(arguments)

    # Load the CSV data
    data = pd.read_csv('build/main.csv')

    # Determine which plots to create
    plot_full = 'full' in arguments or len(arguments) == 0
    plot_selected = 'selected' in arguments or len(arguments) == 0

    # Set up plot style
    setup_plot_style()

    # Plot data as requested
    if args.linear or not (args.linear or args.log):
        if plot_full:
            plot_all_data(data, 'linear')
        if plot_selected:
            plot_specific_sizes(data, [100, 1000, 10000], 'linear')

    if args.log or not (args.linear or args.log):
        if plot_full:
            plot_all_data(data, 'log')
        if plot_selected:
            plot_specific_sizes(data, [100, 1000, 10000], 'log')


if __name__ == '__main__':
    main(sys.argv[1:])