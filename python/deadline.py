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

sns.set_palette(cp_all)
sns.color_palette()

excel_deadline = pd.read_csv('results.csv')
base_df = pd.read_csv('baseline_deadline.csv')

tags = excel_deadline.loc[:, 'App_Batch']

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

base_dict = base_df.set_index('App_Batch')['Mean'].to_dict()

base_non_deadline = np.zeros(2001)
nyx_non_deadline = np.zeros(2001)

base_fcfs_deadline = np.zeros(2001)
nyx_fcfs_deadline = np.zeros(2001)

base_rr_deadline = np.zeros(2001)
nyx_rr_deadline = np.zeros(2001)

base_sjf_deadline = np.zeros(2001)
nyx_sjf_deadline = np.zeros(2001)

base_nimblock_deadline = np.zeros(2001)
nyx_nimblock_deadline = np.zeros(2001)

j=0

for i in np.arange(0.0, 20.001, 0.01, dtype=np.float32):
    for index,row in excel_deadline.iterrows():
        value = base_dict.get(tags.iloc[index])
        deadline = value + i*value
        if deadline<baseline_nonsharing.iloc[index]:
            base_non_deadline[j]+=1

        if deadline<nyx_nonsharing.iloc[index]:
            nyx_non_deadline[j]+=1
        
        if deadline<baseline_fcfs.iloc[index]:
            base_fcfs_deadline[j]+=1

        if deadline<nyx_fcfs.iloc[index]:
            nyx_fcfs_deadline[j]+=1

        if deadline<baseline_rr.iloc[index]:
            base_rr_deadline[j]+=1
        
        if deadline<nyx_rr.iloc[index]:
            nyx_rr_deadline[j]+=1

        if deadline<baseline_sjf.iloc[index]:
            base_sjf_deadline[j]+=1
        
        if deadline<nyx_sjf.iloc[index]:
            nyx_sjf_deadline[j]+=1 

        if deadline<baseline_nimblock.iloc[index]:
            base_nimblock_deadline[j]+=1
        
        if deadline<nyx_nimblock.iloc[index]:
            nyx_nimblock_deadline[j]+=1 
    j+=1      


x= np.arange(0.0, 20.001, 0.01, dtype=np.float32)

fig, axs = plt.subplots(layout='constrained')

new_base_non = [(base/200)*100 for base in base_non_deadline]
new_nyx_non = [(nyx/200)*100 for nyx in nyx_non_deadline]

new_base_fcfs = [(base/200)*100 for base in base_fcfs_deadline]
new_nyx_fcfs = [(nyx/200)*100 for nyx in nyx_fcfs_deadline]

new_base_rr = [(base/200)*100 for base in base_rr_deadline]
new_nyx_rr = [(nyx/200)*100 for nyx in nyx_rr_deadline]

new_base_sjf = [(base/200)*100 for base in base_sjf_deadline]
new_nyx_sjf = [(nyx/200)*100 for nyx in nyx_sjf_deadline]

new_base_nimblock = [(base/200)*100 for base in base_nimblock_deadline]
new_nyx_nimblock = [(nyx/200)*100 for nyx in nyx_nimblock_deadline]

axs.plot(x, new_base_non,linestyle='-', linewidth=1, color='#00429d', label='Base_NonSharing')
axs.plot(x, new_base_fcfs,linestyle='-', linewidth=1, color='#3a64ad', label='Base_FCFS')
axs.plot(x, new_base_rr,linestyle='-', linewidth=1, color='#5b87bc', label='Base_RR')
axs.plot(x, new_base_sjf,linestyle='-', linewidth=1, color='#7bacca', label='Base_SJF')
axs.plot(x, new_base_nimblock,linestyle='-', linewidth=1, color='#9fd0d6', label='Base_Nimblock')

axs.plot(x, new_nyx_non, linestyle='-', linewidth=1, color='#ff9c98', label='Nyx_NonSharing')
axs.plot(x, new_nyx_fcfs, linestyle='-', linewidth=1, color='#f88387', label='Nyx_FCFS')
axs.plot(x, new_nyx_rr, linestyle='-', linewidth=1, color='#e25469', label='Nyx_RR')
axs.plot(x, new_nyx_sjf, linestyle='-', linewidth=1, color='#c1254f', label='Nyx_SJF')
axs.plot(x, new_nyx_nimblock, linestyle='-', linewidth=1, color='#93003a', label='Nyx_Nimblock')

axs.axhline(y=10, color = '#50C878', linestyle = '--')


width = 3.487
height = width/1.618
plt.rc('figure', figsize=(width,height))

#axs.set_title('Deadline Violation Rate', fontsize=14)
axs.set_ylim(ymin=0, ymax=100)
axs.set_ylabel('Violation Rate (%)', fontsize=18)
axs.set_xlabel('Deadline Factor', fontsize=16)
#axs.set_xticks(x, rotation=0, fontsize=16)
axs.legend(loc='upper right', ncols=2, fontsize=11.5)
#axs.set_yticks(np.arange(0, 100.1, 1), fontsize=14)

plt.yticks(fontsize=14)
plt.xticks(fontsize=12)

fig.tight_layout()
#fig.legend([l1, l2], labels=labels, fontsize="10",
 #          loc="lower center",  bbox_to_anchor=(0.5, -0.1))




plt.savefig("deadline.pdf", format="pdf", dpi=1000, bbox_inches="tight")
plt.show()
