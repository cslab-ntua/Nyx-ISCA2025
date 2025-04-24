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
from scipy.stats import gmean

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

sns.set_palette(cp_all)
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
base_non_gmean = gmean(base_non_np)
print(base_non_gmean)

nyx_non_np = np.array(nyx_nonsharing);
nyx_non_gmean = gmean(nyx_non_np)

########

base_fcfs_np = np.array(baseline_fcfs);
base_fcfs_gmean = gmean(base_fcfs_np)
print(base_fcfs_gmean)

nyx_fcfs_np = np.array(nyx_fcfs);
nyx_fcfs_gmean = gmean(nyx_fcfs_np)

#########

base_rr_np = np.array(baseline_rr);
base_rr_gmean = gmean(base_rr_np)

nyx_rr_np = np.array(nyx_rr);
nyx_rr_gmean = gmean(nyx_rr_np)

#########

base_sjf_np = np.array(baseline_sjf);
base_sjf_gmean = gmean(base_sjf_np)

nyx_sjf_np = np.array(nyx_sjf);
nyx_sjf_gmean = gmean(nyx_sjf_np)

#########
base_nimblock_np = np.array(baseline_nimblock);
base_nimblock_gmean = gmean(base_nimblock_np)

nyx_nimblock_np = np.array(nyx_nimblock);
nyx_nimblock_gmean = gmean(nyx_nimblock_np)

labels = ['Non-\nSharing','First Come\nFirst Served','Round\nRobin','Shortest\nJob First', 'Nimblock']

BaselineGmean = [base_non_gmean/base_non_gmean, base_non_gmean/base_fcfs_gmean, base_non_gmean/base_rr_gmean, base_non_gmean/base_sjf_gmean, base_non_gmean/base_nimblock_gmean]
NyxGmean = [base_non_gmean/nyx_non_gmean, base_non_gmean/nyx_fcfs_gmean, base_non_gmean/nyx_rr_gmean, base_non_gmean/nyx_sjf_gmean, base_non_gmean/nyx_nimblock_gmean]


x=np.arange(len(labels))
width = 0.25
multiplier = 0

plt.rcParams["font.family"] = "Times New Roman"
plt.rcParams['xtick.major.pad']='12'


fig, ax = plt.subplots(layout='constrained')

base_bar = ax.bar(x-width/2-0.01, BaselineGmean, width, color='#3761ab', align='center', label= 'Baseline')
ax.bar_label(base_bar, label_type='edge', fmt='%.2f', fontsize = 12)
nyx_bar = ax.bar(x+width/2+0.01, NyxGmean, width, color='#f4777f', align='center', label='Nyx')
ax.bar_label(nyx_bar, label_type='edge', fmt='%.2f', fontsize = 12)

ax.axhline(y=1, color = '#93003a', linestyle = '-', label='Base_NonSharing')
#ax.text(1.3, 1.3, 'Baseline', va='center', fontsize=11, color='r')

handles, leg_labels = plt.gca().get_legend_handles_labels() 
order = [1, 2, 0]


ax.set_ylabel('Geomean Response Time Reduction', fontsize=18)
ax.set_xticks(x, labels, rotation=0, fontsize=17)
ax.legend([handles[i] for i in order], [leg_labels[i] for i in order], bbox_to_anchor=(0.5, 1.14), loc='upper center', ncols=3, fontsize=15)
ax.set_yticks(np.arange(0, 14.5, 2))
plt.yticks(fontsize=14)


width = 3.487
height = width/1.618
plt.rc('figure', figsize=(width,height))

#fig.tight_layout()
plt.savefig("geomean.pdf", format="pdf", dpi=1000, bbox_inches="tight")
plt.show()
