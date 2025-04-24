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

#color_mine = colors(0)
#color_cublasxt = colors(2)
#color_ideal = colors(3)
#color_werk = colors1(2)

from pylab import cm
colors = cm.get_cmap('PuRd',  5)
colors_dark = cm.get_cmap('Dark2',  5)

viridis = cm.get_cmap('viridis',  4)
magma = cm.get_cmap('magma',  8)
# cp6 = list(map(lambda x: sns.desaturate(x,0.9),[colors(2), red7[2], magma(6),  viridis(3), yg7[0], gb7[4]]))
cp6 = list(map(lambda x: sns.desaturate(x,0.9),[colors(2), gb7[4], red7[2], magma(6),  viridis(3), yg7[0]]))

# Tesla-V100  colors(2)
# Tesla-A100  gb7[4] 
# AMD-EPYC-64    red7[2] 
# AMD-EPYC-24   magma(6)
# ARM-NEON    viridis(3)
# POWER9      colors_dark(1)
# Alveo-U280  yg7[0]
cp7 = list(map(lambda x: sns.desaturate(x,0.9),[colors(2), gb7[4], red7[2], magma(6), colors_dark(1), viridis(3), yg7[0]]))
cp5 = list(map(lambda x: sns.desaturate(x,0.9),[colors_dark(0), colors_dark(1), colors_dark(2), colors_dark(3), colors_dark(4)]))
#cp_all = list(map(lambda x: sns.desaturate(x,1),[colors(4), colors(2), gb7[4], red7[4], magma(6), viridis(3), gb7[1], colors_dark(8), yg7[2]]))

cp_all = list(map(lambda x: sns.desaturate(x,1),[magma(6), colors(4), gb7[4], red7[2], magma(6), viridis(3), gb7[1], colors_dark(1), yg7[0]]))

cp_base = list(map(lambda x: sns.desaturate(x,0.9),[red7[4], red7[6]]))
cp_nyx = list(map(lambda x: sns.desaturate(x,0.9),[gb7[1], gb7[4]]))

sns.set_palette(cp_all)
sns.set_palette(cp_base)
sns.set_palette(cp_nyx)
sns.color_palette()

excel_deadline = pd.read_csv('results.csv')

baseline_nonsharing = excel_deadline.loc[:,'baseline_nonsharing']
nyx_nonsharing = excel_deadline.loc[:,'nyx_nonsharing']

baseline_fcfs = excel_deadline.loc[:,'baseline_fcfs']
nyx_fcfs = excel_deadline.loc[:,'nyx_fcfs']

baseline_rr = excel_deadline.loc[:,'baseline_rr']
nyx_rr = excel_deadline.loc[:,'nyx_rr']

baseline_sjf = excel_deadline.loc[:,'baseline_sjf']
nyx_sjf = excel_deadline.loc[:,'nyx_sjf']

baseline_nimblock = excel_deadline.loc[:,'baseline_nimblock']
nyx_nimblock = excel_deadline.loc[:,'nyx_nimblock']


base_non_np = np.array(baseline_nonsharing);
base_non_perc99 = np.percentile(base_non_np, 99)
base_non_perc95 = np.percentile(base_non_np, 95)

nyx_non_np = np.array(nyx_nonsharing);
nyx_non_perc99 = np.percentile(nyx_non_np, 99)
nyx_non_perc95 = np.percentile(nyx_non_np, 95)

########

base_fcfs_np = np.array(baseline_fcfs);
base_fcfs_perc99 = np.percentile(base_fcfs_np, 99)
base_fcfs_perc95 = np.percentile(base_fcfs_np, 95)

nyx_fcfs_np = np.array(nyx_fcfs);
nyx_fcfs_perc99 = np.percentile(nyx_fcfs_np, 99)
nyx_fcfs_perc95 = np.percentile(nyx_fcfs_np, 95)

#########

base_rr_np = np.array(baseline_rr);
base_rr_perc99 = np.percentile(base_rr_np, 99)
base_rr_perc95 = np.percentile(base_rr_np, 95)

nyx_rr_np = np.array(nyx_rr);
nyx_rr_perc99 = np.percentile(nyx_rr_np, 99)
nyx_rr_perc95 = np.percentile(nyx_rr_np, 95)

#########

base_sjf_np = np.array(baseline_sjf);
base_sjf_perc99 = np.percentile(base_sjf_np, 99)
base_sjf_perc95 = np.percentile(base_sjf_np, 95)

nyx_sjf_np = np.array(nyx_sjf);
nyx_sjf_perc99 = np.percentile(nyx_sjf_np, 99)
nyx_sjf_perc95 = np.percentile(nyx_sjf_np, 95)

#########
base_nimblock_np = np.array(baseline_nimblock);
base_nimblock_perc99 = np.percentile(base_nimblock_np, 99)
base_nimblock_perc95 = np.percentile(base_nimblock_np, 95)

nyx_nimblock_np = np.array(nyx_nimblock);
nyx_nimblock_perc99 = np.percentile(nyx_nimblock_np, 99)
nyx_nimblock_perc95 = np.percentile(nyx_nimblock_np, 95)


Baseline99 = [base_non_perc99/base_non_perc99, base_non_perc99/base_fcfs_perc99, base_non_perc99/base_rr_perc99, base_non_perc99/base_sjf_perc99, base_non_perc99/base_nimblock_perc99]
Nyx99 = [base_non_perc99/nyx_non_perc99, base_non_perc99/nyx_fcfs_perc99, base_non_perc99/nyx_rr_perc99, base_non_perc99/nyx_sjf_perc99, base_non_perc99/nyx_nimblock_perc99]

Baseline95 = [base_non_perc95/base_non_perc95, base_non_perc95/base_fcfs_perc95, base_non_perc95/base_rr_perc95, base_non_perc95/base_sjf_perc95, base_non_perc95/base_nimblock_perc95]
Nyx95 = [base_non_perc95/nyx_non_perc95, base_non_perc95/nyx_fcfs_perc95, base_non_perc95/nyx_rr_perc95, base_non_perc95/nyx_sjf_perc95, base_non_perc95/nyx_nimblock_perc95]

labels = ['Non-\nSharing','First Come\nFirst Served','Round\nRobin','Shortest\nJob First', 'Nimblock']

x=np.arange(len(labels))
width = 0.25
multiplier = 0

plt.rcParams["font.family"] = "Times New Roman"
plt.rcParams['xtick.major.pad']='12'

fig, ax = plt.subplots(layout='constrained')

base_bar95 = ax.bar(x-1.5*width/2 - 0.07, Baseline95, width/2, color='#3761ab', align='center', label= 'Baseline-95')
ax.bar_label(base_bar95, label_type='edge', fmt='%.2f', fontsize = 10)
nyx_bar95 = ax.bar(x-0.5*width/2 - 0.025, Nyx95, width/2, color='#f4777f', align='center', label='Nyx-95')
ax.bar_label(nyx_bar95, label_type='edge', fmt='%.2f', fontsize = 10)

base_bar99 = ax.bar(x+0.5*width/2 + 0.025, Baseline99, width/2, color='#73a2c6', align='center', label= 'Baseline-99')
ax.bar_label(base_bar99, label_type='edge', fmt='%.2f', fontsize = 10)
nyx_bar99 = ax.bar(x+1.5*width/2 + 0.07, Nyx99, width/2, color='#be214d', align='center', label='Nyx-99')
ax.bar_label(nyx_bar99, label_type='edge', fmt='%.2f', fontsize = 10)

ax.axhline(y=1, color = '#93003a', linestyle = '-', label='Base_NonSharing')
#ax.text(1.32, 1.17, 'Baseline', va='center', fontsize=11, color='r')


handles, leg_labels = plt.gca().get_legend_handles_labels() 
order = [1, 2, 3, 4, 0]

fig.align_labels()
ax.set_ylabel('Tail Response Time Reduction', fontsize=18)
ax.set_xticks(x, labels, rotation=0, fontsize=17)
ax.legend([handles[i] for i in order], [leg_labels[i] for i in order], loc='upper center', ncols=3, fontsize=13)
ax.set_yticks(np.arange(0, 7.3, 1))
plt.yticks(fontsize=14)
#bbox_to_anchor=(0.5, 1.13), 

width = 3.487
height = width/1.618
plt.rc('figure', figsize=(width,height))

#fig.tight_layout()
plt.savefig("tail_latency.pdf", format="pdf", dpi=1000, bbox_inches="tight")
plt.show()