
# Documentation



## Installation

The [XDMA driver](https://github.com/Xilinx/dma_ip_drivers) from Xilinx is required to run the software on the host machine and execute either Nyx or the Baseline architecture on the FPGA. To install the XDMA driver, use the following commands:

    git clone https://github.com/Xilinx/dma_ip_drivers.git
    git checkout remotes/origin/2020.1
    cd XDMA/linux-kernel/xdma/
    make

The FPGA must be connected to the host machine via micro-usb. Vitis HLS or bitstream generation is not required, as all necessary bitstream files are included in this artifact. To load the bitstream file onto the FPGA, run :

    cd Nyx/
    vivado -mode tcl
    source nyx.tcl

After performing a warm reboot, verify that everything is working by running : 
    
    cd XDMA/linux-kernel/tests/
    ./load_driver.sh

A similar procedure can be followed for the Baseline architecture using the `Baseline/` directory.

To install the required dependencies and run our Python scripts,, run:
    
    cd python/
    pip install -r requirements.txt

## Experimental Workflow

To replicate the evaluation results, one would generally follow these steps:
- Run a single application to replicate Figure 6.
- Run a sequence of applications to replicate Figures 7 and 8.
- Run both to replicate Figure 9.
Modify the execution mode in `/src/event_gen.h` and select:
    
    #define QUERY // for running a sequence of applications
    #define SINGLE // for running a single application

### Relative Performance of Applications (Figure 6)

After selecting `#define SINGLE` in `/src/event_gen.h` and `#define NONSHARING` in `/src/VirtFlow_types.h` within `Nyx/` directory, execute the following command:
    
    ./single_app.sh

This will run the experiments and generate the results for Nyx (or the Baseline architecture). After collecting the performance data from both architectures, navigate to the `python/` directory and run:

    python3 relative_perf.py

This script will analyze and plot the results `(Figure 6)`. It will also generate the required `base_deadline.csv` output for parsing and plotting the Deadline Violation Rate.

The above script can be modified to adjust the batch size for each application. The event numbers for each application are as follows:

    0 : Optical Flow
    1 : 3D Rendering
    2 : Image Compression
    3 : Lenet
    4 : Digit Recognition
    5 : Image Scaling      

### Reduction in Response Time and Tail Latency (Figure 7 and 8) 

After selecting `#define QUERY` in `/src/event_gen.h` within the `Nyx/` directory, choose one of the tested scheduling algorithms by modifying the `#define` options in `src/VirtFlow_types.h`. The available options are
    
    #define NONSHARING // A non-sharing scheduler (FPGA resources are not shared)
    #define FCFS // First Come First Served
    #define RR // Round Robin
    #define SJF // Shorthest Job First
    #define Nimblock // Nimblock Scheduler

After configuring the scheduler, run the experiments and generate the response times for each incoming application in our tested sequences. As outlined in our paper, we use three different delay configurations between applications, each implemented in the corresponding script. To run the scenarios, execute: 

    ./query_1.5.sh //For running Relaxed Scenario with 1500ms delay
    ./query_0.2.sh //For running Standard Scenario with 200ms delay 
    ./query_0.05.sh //For running Stressed Scenario with 50ms delay

This will run the experiments and generate the results for Nyx (or the Baseline). Once the performance data are collected from both architectures, navigate to the `python/` directory and first run:

    python3 preprocess.py

This script processes the exported results and generates a unified `results.csv` for analysis and plotting. To plot the geometric mean reduction in response time `(Figure 7)`, execute :

    python3 geomean.py 

While for reduction in tail response time in 99th and 95th percentile `(Figure 8)`, run :

    python3 tail_latency.py

If you wish to modify the delay for incoming applications, please refer to the `query_*.sh` script files. For modifying applications, batch sizes, and priority levels in test sequences, refer to `src/event_gen.h`.

### Deadline Violation Rate (Figure 9)

To analyze and plot the Deadline Violation Rate, one must first complete the follwing two steps: 
- Measure the perfromance of each application running alone in our virtualized FPGA (Step 1). 
- Collect the response latency for each application in the tested sequences (Step 2). 
    
After collecting the results, navigate to the `python/` directory and run:

    python3 relative_perf.py

This script analyzes the data and generates the required `base_deadline.csv` output for parsing and plotting the Deadline Violation Rate. Next, process the exported results by running:

    python3 preprocess.py

Finally, to plot the Deadline Violation Rate `(Figure 9)` for each scheduler across both architectures, execute:

    python3 deadline.py

    



## Usage/Examples

In the `python/` directory, we provide useful `*.csv` examples that demonstrate how the data are organized after executing the previous steps. These examples also illustrate how our python scripts analyze and plot the imported data.


## Support

For any questions or issues, please email pmiliad@cslab.ece.ntua.gr .



