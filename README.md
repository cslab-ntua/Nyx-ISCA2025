
# Nyx: Virtualizing Dataflow Execution on shared FPGAs (ISCA'25 Artifact)



[![DOI](https://zenodo.org/badge/DOI/10.5281/zenodo.15274814.svg)](https://doi.org/10.5281/zenodo.15274814)



This repository contains the prototype implementation of Nyx for the Xilinx Alveo U250 FPGA. It includes all required bitstreams and software to run the scheduler and replicate the experiments for the artifact evaluation of our paper. We also plan to extend this work to support additional FPGA devices in the future.

Due to space constraints on the GitHub repository, please download the bitstream files from [Zenodo](https://doi.org/10.5281/zenodo.15274814).

## Authors

- Panagiotis Miliadis (National Technical University of Athens, Greece)
- Dimitris Theodoropoulos (National Technical University of Athens)
- Nectarios Koziris (National Technical University of Athens, Greece)
- Dionisios Pnevmatikatos (National Technical University of Athens, Greece) 



## Paper Citation

TBA

## Hardware Dependencies

- Nyx requires a Xilinx Alveo U250 Data Center FPGA, programmed through micro-USB, to run the provided source code, scripts, and bitstreams. 
- The benchmarks have a maximum memory footprint of 3 GiB. If Vivado, Vitis HLS, or other required software dependencies are not installed, additional memory is needed.
- For the evaluation of our paper, we used a 3.2 GHz Intel Core i7-8700 CPU with 32 GB of DDR memory.

## Software Dependencies

- The [XDMA driver](https://github.com/Xilinx/dma_ip_drivers) from Xilinx is required to run the software on the host machine.
- [OpenCV4](https://docs.opencv.org/4.7.0/d7/d9f/tutorial_linux_install.html) is needed for a subset of our benchmarks. 
- Vivado 2022.2 is required to load the bitstream file onto the FPGA device. Other versions have not been tested. 

## Directory Structure
- `Nyx/` Scripts, source code, and bitstreams for running the Nyx architecture.
- `Baseline/` Scripts, source code, and bitstreams for running the Baseline architecture.
- `HLS_apps/` HLS code for benchmarks and datasets.
- `Recover_Nyx/` Scripts, source code, and bitstreams for recovering Nyx.
- `python/` Python scripts for preprocessing, analyzing, and plotting the results as presented in the evaluation section of the paper.
## Documentation

For details on the experiment workflow, please refer to the [Documentation](artifact_workflow.md)


## License

[Apache License 2.0](http://www.apache.org/licenses/LICENSE-2.0)


## Support

For any questions or issues, please email pmiliad@cslab.ece.ntua.gr .

