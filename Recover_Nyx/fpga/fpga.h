/*===============================================================*/
/*                                                               */
/*                    fpga.h                      */
/*                                                               */
/*      Functions to interact with the FPGA device.     */
/*                                                               */
/*===============================================================*/

#include "dma_functions.h"
#include <ctime>
#include <ratio>
#include <chrono>
using namespace std::chrono;


void Program_Device(char* binFile, int PRR_Slot);
void Execute_Task(int UserSlot);
void Finish_Task(int PRR_Slot);
void FPGA_write(void* data, uint64_t start_address, uint64_t bytes);
void FPGA_read(void* data, uint64_t start_address, uint64_t bytes);
uint8_t bitExtracted(uint32_t number, int k, int p);