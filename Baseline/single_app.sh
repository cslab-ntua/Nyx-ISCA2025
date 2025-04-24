#!/bin/bash

for j in {0..20}; do

#OPTICAL FLOW
for i in 1 2 4; do
	while : ; do
		cd ./src/
        	timeout 10 ./baseline 1 $i
        	EXIT=$?
        	if [ $EXIT -eq 0 ] ; then
                	echo "success"
		        cd ../	
                	break
        	elif [ $EXIT -eq 124 ]; then
                	echo "timeout"
                	cd ../Baseline/
        	else
                	echo "Reboot FPGA and restart from j=" $j
			cd ../
                	break 5
        	fi
	done
done

#3D Rendering
for i in 1 2 4 8; do
        while : ; do
                cd ./src/
                timeout 10 ./baseline 2 $i
                EXIT=$?
                if [ $EXIT -eq 0 ] ; then
                        echo "success"
                        cd ../
                        break
                elif [ $EXIT -eq 124 ]; then
                        echo "timeout"
                        cd ../Baseline/
                else
                        echo "Reboot FPGA and restart from j=" $j
                        cd ../
                        break 5
                fi
        done
done

#Image Compression
for i in 1 2 4 8 12 16 32 64 128 256 512 1024; do
        while : ; do
                cd ./src/
                timeout 50 ./baseline 3 $i
                EXIT=$?
                if [ $EXIT -eq 0 ] ; then
                        echo "success"
                        cd ../
                        break
                elif [ $EXIT -eq 124 ]; then
                        echo "timeout"
                        cd ../Baseline/
                else
                        echo "Reboot FPGA and restart from j=" $j
                        break 5
                fi
        done
done

#Lenet
for i in 1 2 4 8 12 16 32 64 128 256 512 1024; do
        while : ; do
                cd ./src/
                timeout 50 ./baseline 4 $i
                EXIT=$?
                if [ $EXIT -eq 0 ] ; then
                        echo "success"
                        cd ../
                        break
                elif [ $EXIT -eq 124 ]; then
                        echo "timeout"
                        cd ../Baseline/
                else
                        echo "Reboot FPGA and restart from j=" $j
                        break 5
                fi
        done
done


#Digit Recognition
for i in 1 2 4 8 12 16; do
        while : ; do
                cd ./src/
                timeout 80 ./baseline 5 $i
                EXIT=$?
                if [ $EXIT -eq 0 ] ; then
                        echo "success"
                        cd ../
                        break
                elif [ $EXIT -eq 124 ]; then
                        echo "timeout"
                        cd ../Baseline/
                else
                        echo "Reboot FPGA and restart from j=" $j
                        break 5
                fi
        done
done

#Image Scaling
for i in 1 2 4 8 12 16 32 64; do
        while : ; do
                cd ./src/
                timeout 150 ./baseline 6 $i
                EXIT=$?
                if [ $EXIT -eq 0 ] ; then
                        echo "success"
                        cd ../
                        break
                elif [ $EXIT -eq 124 ]; then
                        echo "timeout"
                        cd ../Baseline/
                else
                        echo "Reboot FPGA and restart from j=" $j
                        break 5
                fi
        done
done
done
