#!/usr/bin/env python

import pandas as pd
import sys
import os


def generate_latex_table(data, vector_type, sizes):
    # Filter data for the specified vector type and sizes
    subset = data[(data['type'] == vector_type) & (data['num'].isin(sizes))]

    # Rename columns for readability
    subset = subset.rename(columns={'strat': 'Estratégia', 'num': 'Tamanho'})

    # Convert time from nanoseconds to microseconds
    subset['time'] = subset['time'] / 1e3

    # Pivot the table
    pivot_table = subset.pivot(
        index='Tamanho', columns='Estratégia', values='time'
    )

    # Convert pivot table to LaTeX format with units
    latex_table = pivot_table.map(
        lambda x: f'\\SI{{{x:.2f}}}{{\\micro\\second}}'
    ).to_latex(
        index=True,
        header=True,
        escape=False,
        column_format='|c|' + 'c|' * len(pivot_table.columns),
    )
    return latex_table


def main(arguments):
    # Load the CSV data
    data = pd.read_csv('build/main.csv')

    # Filter data for specific sizes
    sizes = [100, 1000, 10000]

    # Create output directory if it doesn't exist
    os.makedirs('build/fig', exist_ok=True)

    with open('build/fig/tables.tex', 'w') as f:
        vector_types = data['type'].unique()
        for i, vector_type in enumerate(vector_types):
            latex_table = generate_latex_table(data, vector_type, sizes)
            f.write(f'\\begin{{table}}[h!]\n\\centering\n')
            f.write(latex_table)
            f.write(
                f'\\caption{{Tempo de Execução do QuickSort com Vetores {vector_type}}}\n\\label{{tbl:{vector_type}}}\n'
            )
            f.write('\\end{table}\n\n')

            # Add a horizontal space between tables to make pairs appear side-by-side
            if (i + 1) % 2 == 0:
                f.write('\\hspace{0.5cm}\n')


if __name__ == '__main__':
    main(sys.argv[1:])