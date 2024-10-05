#!/usr/bin/env python

import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns
import sys
from matplotlib import rcParams

# Set up LaTeX and Palatino font for matplotlib
def setup_plot_style():
    rcParams['text.usetex'] = True
    rcParams['font.family'] = 'serif'
    rcParams['font.serif'] = ['mathpazo']
    sns.set_theme(style='whitegrid')


def plot_data(data, title, filename):
    print(f'Plotting {filename}')
    data = data[data['num'] >= 100]

    fig, axes = plt.subplots(1, 2, figsize=(10, 4))

    for ax, yscale in zip(axes, ['linear', 'log']):
        sns.scatterplot(
            data=data,
            x='num',
            y=data['time'] / 1e1,
            hue='strat',
            s=0.5,  # Set marker size
            ax=ax,
            edgecolor='none'  # Remove edge color
        )
        ax.set_title(f'Escala {yscale.capitalize()}')
        ax.set_xlabel('Tamanho do Arranjo (num)')
        ax.set_ylabel('Tempo de Execução (ns)')
        ax.set_yscale(yscale)
        handles, labels = ax.get_legend_handles_labels()
        ax.legend(handles=handles, labels=labels, title='Estratégia', markerscale=10)

    fig.suptitle(title, fontsize=16)
    plt.tight_layout(rect=(0, 0, 1, 0.95))  # Use a tuple instead of a list
    plt.savefig(filename, dpi=1200, format='pdf')
    # plt.show()


def plot_all_data(data):
    vector_types = data['type'].unique()
    for vector_type in vector_types:
        subset = data[data['type'] == vector_type]
        title = f'Desempenho das Estratégias de Escolha de Pivô com Vetores {vector_type}'
        filename = f'build/fig/{vector_type}_all_data.pdf'
        plot_data(subset, title, filename)


def plot_specific_sizes(data, sizes):
    filtered_data = data[data['num'].isin(sizes)]
    vector_types = filtered_data['type'].unique()
    for vector_type in vector_types:
        subset = filtered_data[filtered_data['type'] == vector_type]
        title = f'Desempenho das Estratégias de Escolha de Pivô com Vetores {vector_type} (Pontos Selecionados)'
        filename = f'build/fig/{vector_type}_specific_sizes.pdf'
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