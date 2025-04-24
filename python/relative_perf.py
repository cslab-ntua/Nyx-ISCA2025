#!/usr/bin/env python

import warnings
warnings.filterwarnings('ignore')


import pandas as pd
from scipy.stats.mstats import hmean
pd.options.mode.chained_assignment = None  # default='warn'
from pandas.api.types import CategoricalDtype
import math
import matplotlib as mpl
#mpl.use('pdf')
import matplotlib.pyplot as plt
import matplotlib.font_manager
import numpy as np
from collections import OrderedDict
import matplotlib
import seaborn as sns
from matplotlib.cbook import boxplot_stats

from IPython.display import display, HTML

import subprocess


from matplotlib.cbook import boxplot_stats
from matplotlib.ticker import MaxNLocator


from matplotlib.lines import Line2D
import matplotlib.patches as mpatches

def process(csv_file):
    # Read CSV without headers
    df = pd.read_csv(csv_file, header=None)

    # Remove rows that contain non-numeric values
    df = df[pd.to_numeric(df[1], errors = 'coerce').notna()]

    # Convert the third column to numeric, errors='coerce' will turn invalid entries into NaN
    df[1] = pd.to_numeric(df[1], errors='coerce')
    df[2] = pd.to_numeric(df[2], errors='coerce')

    # Combine the first two columns with an underscore and drop the original columns
    df["App_Batch"] = df[0].astype(str) + '_' + df[1].astype(str)

    grouped = df.groupby('App_Batch').apply(
        lambda x: pd.Series({
            'Mean': x[2].sort_values().iloc[:int(0.75 * len(x))].mean(),
            'Application': x[0].iloc[0],  # Retain the first column value from any row in the group
            'Batch_Size': x[1].iloc[0]   # Retain the second column value from any row in the group
        })
    ).reset_index()
    return grouped


def post_process(grouped1, grouped2):
    # Create a dictionary of mean values from grouped2 where Batch Size == 1
    mean_batch_1 = grouped2[grouped2['Batch_Size'] == 1].set_index('Application')['Mean'].to_dict()
    def compute_post_process(row):
        app_name = row['Application']
        if app_name in mean_batch_1 and mean_batch_1[app_name] !=0:
            return (mean_batch_1[app_name] * row['Batch_Size']) / row['Mean']
        else:
            return None  # Avoid division by zero

    grouped1['post_process'] = grouped1.apply(compute_post_process, axis=1)
    return grouped1


def plot_results(grouped1, grouped2):

    #Red
    red1= sns.color_palette("Reds_d",1)
    red2= sns.color_palette("Reds_d",2)
    red3= sns.color_palette("Reds_d",3)
    red4= sns.color_palette("Reds_d",4)
    red5= sns.color_palette("Reds_d",5)
    red6= sns.color_palette("Reds_d",6)
    red7= sns.color_palette("Reds_d",7)
    red8= sns.color_palette("Reds_d",8)

    #YellowGreen
    yg1= sns.color_palette("YlGn_d",1)
    yg2= sns.color_palette("YlGn_d",2)
    yg3= sns.color_palette("YlGn_d",3)
    yg4= sns.color_palette("YlGn_d",4)
    yg5= sns.color_palette("YlGn_d",5)
    yg7= sns.color_palette("YlGn_d",7)

    #GreenBlue
    gb1= sns.color_palette("GnBu_d",1)
    gb2= sns.color_palette("GnBu_d",2)
    gb3= sns.color_palette("GnBu_d",3)
    gb4= sns.color_palette("GnBu_d",4)
    gb5= sns.color_palette("GnBu_d",5)
    gb6= sns.color_palette("GnBu_d",6)
    gb7= sns.color_palette("GnBu_d",7)
    gb8= sns.color_palette("GnBu_d",8)

    cp2 = list(map(lambda x: sns.desaturate(x,0.9),[red7[2],gb7[4]]))
    cp2v1 = list(map(lambda x: sns.desaturate(x,0.9),[red7[2],yg7[0]]))
    cp3 = list(map(lambda x: sns.desaturate(x,0.9),[yg7[0],gb7[4],red7[2]]))
    #cp4 = list(map(lambda x: sns.desaturate(x,0.9),red1+gb2+yg1))
    cp2_2 = list(map(lambda x: sns.desaturate(x,0.9),[red7[0],red7[3],gb7[4],gb7[6]]))
    cp_total_spectrum = list(map(lambda x: sns.desaturate(x,0.9),gb7 + yg7 + red7))

    from pylab import cm
    colors = cm.get_cmap('PuRd',  5)
    colors_dark = cm.get_cmap('Dark2',  5)

    viridis = cm.get_cmap('viridis',  4)
    magma = cm.get_cmap('magma',  8)
    cp6 = list(map(lambda x: sns.desaturate(x,0.9),[colors(2), gb7[4], red7[2], magma(6),  viridis(3), yg7[0]]))
    cp7 = list(map(lambda x: sns.desaturate(x,0.9),[colors(2), gb7[4], red7[2], magma(6), colors_dark(1), viridis(3), yg7[0]]))
    cp5 = list(map(lambda x: sns.desaturate(x,0.9),[colors_dark(0), colors_dark(1), colors_dark(2), colors_dark(3), colors_dark(4)]))
    cp_all = list(map(lambda x: sns.desaturate(x,1),[magma(6), colors(4), gb7[4], red7[2], magma(6), viridis(3), gb7[1], colors_dark(1), yg7[0]]))

    sns.set_palette(cp_all)
    sns.color_palette()

    # For each unique application in grouped1 and grouped2, create a separate plot
    unique_apps = set(grouped1['Application']).union(set(grouped2['Application']))

    plt.rcParams["font.family"] = "Times New Roman"
    plt.rcParams['xtick.major.pad']='12'
    fig, axs = plt.subplots(3, 2)
    #axes = axes.flatten()

    group1 = grouped1[grouped1['Application'] == "Optical Flow"].sort_values('Batch_Size')
    group2 = grouped2[grouped2['Application'] == "Optical Flow"].sort_values('Batch_Size')
    l1 = axs[0,0].plot(group1['Batch_Size'], group1['post_process'], linestyle='--', linewidth=0.7, marker='^',  markersize=4.5, color='#3761ab', label='Baseline')
    l2 = axs[0,0].plot(group2['Batch_Size'], group2['post_process'], linestyle='--', linewidth=0.7, marker='s',  markersize=4.5, color='#f4777f', label='Nyx')
    labels = ["Baseline Architecture", "Nyx Architecture"] 

    axs[0, 0].set_title('Relative Performance - OF', fontsize=12, loc='left')
    axs[0, 0].set_yticks(np.arange(0, 10.5, 2))
    axs[0, 0].set_ylim(ymin=0, ymax=10)
    axs[0, 0].set_xlabel('Batch Size', fontsize=12)
    axs[0, 0].plot(group1['Batch_Size'], group1['post_process'], linestyle='--', linewidth=0.5, marker='^', markersize=3, color='#3761ab', label='Baseline')
    axs[0, 0].plot(group2['Batch_Size'], group2['post_process'], linestyle='--', linewidth=0.5, marker='s',  markersize=3, color='#f4777f', label='Nyx')
    axs[0, 0].xaxis.set_major_locator(MaxNLocator(integer=True))
    axs[0, 0].grid(axis='y', linewidth=0.1)

    group1 = grouped1[grouped1['Application'] == "Lenet"].sort_values('Batch_Size')
    group2 = grouped2[grouped2['Application'] == "Lenet"].sort_values('Batch_Size')
    axs[0, 1].set_title('Relative Performance - LN', fontsize=12, loc='left')
    axs[0, 1].set_yticks(np.arange(0, 70.1, 14))
    axs[0, 1].set_ylim(ymin=0, ymax=70.1)
    axs[0, 1].set_xlabel('Batch Size', fontsize=12)
    axs[0, 1].plot(group1['Batch_Size'], group1['post_process'],linestyle='--', linewidth=0.5, marker='^', markersize=3, color='#3761ab', label='Baseline')
    axs[0, 1].plot(group2['Batch_Size'], group2['post_process'], linestyle='--', linewidth=0.5, marker='s',  markersize=3, color='#f4777f', label='Nyx')
    axs[0, 1].xaxis.set_major_locator(MaxNLocator(integer=True))
    axs[0, 1].grid(axis='y', linewidth=0.1)

    group1 = grouped1[grouped1['Application'] == "3D Rendering"].sort_values('Batch_Size')
    group2 = grouped2[grouped2['Application'] == "3D Rendering"].sort_values('Batch_Size')
    axs[1, 0].set_title('Relative Performance - 3DR', fontsize=12, loc='left')
    axs[1, 0].set_yticks(np.arange(0, 16.5, 4))
    axs[1, 0].set_ylim(ymin=0, ymax=16.5)
    axs[1, 0].set_xlabel('Batch Size', fontsize=12)
    axs[1, 0].plot(group1['Batch_Size'], group1['post_process'], linestyle='--', linewidth=0.5, marker='^', markersize=3, color='#3761ab', label='Baseline')
    axs[1, 0].plot(group2['Batch_Size'], group2['post_process'], linestyle='--', linewidth=0.5, marker='s',  markersize=3, color='#f4777f', label='Nyx')
    axs[1, 0].xaxis.set_major_locator(MaxNLocator(integer=True))
    axs[1, 0].grid(axis='y', linewidth=0.1)

    group1 = grouped1[grouped1['Application'] == "Image Compression"].sort_values('Batch_Size')
    group2 = grouped2[grouped2['Application'] == "Image Compression"].sort_values('Batch_Size')
    axs[1, 1].set_title('Relative Performance - IMGC', fontsize=12, loc='left')
    axs[1, 1].set_yticks(np.arange(0, 90.1, 18))
    axs[1, 1].set_ylim(ymin=0, ymax=90)
    axs[1, 1].set_xlabel('Batch Size', fontsize=12)
    axs[1, 1].plot(group1['Batch_Size'], group1['post_process'], linestyle='--', linewidth=0.5, marker='^', markersize=3, color='#3761ab', label='Baseline')
    axs[1, 1].plot(group2['Batch_Size'], group2['post_process'], linestyle='--', linewidth=0.5, marker='s',  markersize=3, color='#f4777f', label='Nyx')
    axs[1, 1].xaxis.set_major_locator(MaxNLocator(integer=True))
    axs[1, 1].grid(axis='y', linewidth=0.1)

    group1 = grouped1[grouped1['Application'] == "Digit Recognition"].sort_values('Batch_Size')
    group2 = grouped2[grouped2['Application'] == "Digit Recognition"].sort_values('Batch_Size')
    axs[2, 0].set_title('Relative Performance - DR', fontsize=12, loc='left')
    axs[2, 0].set_yticks(np.arange(0, 1.11, 0.05))
    axs[2, 0].set_ylim(ymin=0.9, ymax=1.1)
    axs[2, 0].set_xlabel('Batch Size', fontsize=12)
    axs[2, 0].plot(group1['Batch_Size'], group1['post_process'], linestyle='--', linewidth=0.5, marker='^', markersize=3, color='#3761ab', label='Baseline')
    axs[2, 0].plot(group2['Batch_Size'], group2['post_process'], linestyle='--', linewidth=0.5, marker='s',  markersize=3, color='#f4777f', label='Nyx')
    axs[2, 0].xaxis.set_major_locator(MaxNLocator(integer=True))
    axs[2, 0].grid(axis='y', linewidth=0.1)

    group1 = grouped1[grouped1['Application'] == "Image Scaling"].sort_values('Batch_Size')
    group2 = grouped2[grouped2['Application'] == "Image Scaling"].sort_values('Batch_Size')
    axs[2, 1].set_title('Relative Performance - IMGS', fontsize=12, loc='left')
    axs[2, 1].set_yticks(np.arange(0, 2.1, 0.4))
    axs[2, 1].set_ylim(ymin=0.4, ymax=2)
    axs[2, 1].set_xlabel('Batch Size', fontsize=12)
    axs[2, 1].plot(group1['Batch_Size'], group1['post_process'], linestyle='--', linewidth=0.5, marker='^', markersize=3, color='#3761ab', label='Baseline')
    axs[2, 1].plot(group2['Batch_Size'], group2['post_process'], linestyle='--', linewidth=0.5, marker='s',  markersize=3, color='#f4777f', label='Nyx)')
    axs[2, 1].xaxis.set_major_locator(MaxNLocator(integer=True))
    axs[2, 1].grid(axis='y', linewidth=0.1)


    width = 3.487
    height = 1.15*width/1.618
    plt.rc('figure', figsize=(width,height))

    fig.tight_layout()
    fig.legend([l1, l2], labels=labels, fontsize='12',
               loc="lower center",  bbox_to_anchor=(0.5, -0.12))

    plt.savefig("relative_perf.pdf", format="pdf", dpi=1000, bbox_inches="tight")
    plt.show()

def export_results(grouped, filename):

    # Select only the required columns
    export_df = grouped[['App_Batch', 'Mean']]
    
    # Save to CSV
    export_df.to_csv(filename, index=False)
    
    print(f"Results exported to {filename}")

# Example usage
if __name__ == "__main__":
    input_nyx = "nyx_app.csv"  # Change this to your input file
    input_baseline = "baseline_app.csv"
    nyx = process(input_nyx)
    baseline = process(input_baseline)

    nyx = post_process(nyx,baseline)
    baseline = post_process(baseline, baseline)
    plot_results(baseline,nyx)
    #print(nyx)
    #print(baseline)
    export_results(baseline, "baseline_deadline.csv")
    #print(baseline)
