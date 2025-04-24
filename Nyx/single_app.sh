#!/bin/bash

for j in {0..20}; do

#OPTICAL FLOW
var=0
for i in 1 2 4; do
	var=0
	while : ; do
		var=$((var+1));
		cd ./src/
        	timeout 10 ./nyx 1 $i
        	EXIT=$?
        	if [ $EXIT -eq 0 ] ; then
                	echo "success"
		        cd ../	
                	break
        	elif [ $EXIT -eq 124 ]; then
                	echo "timeout"
                	cd ../../Recover_Nyx/
                	./clean_nyx
                	./clean_nyx
                	cd ../Nyx/
			if [ $var -eq 20 ]; then
				echo "Reboot FPGA and restart from j = " $j
				break 5
			fi
        	else
                	echo "Rebbot FPGA and restart from j = " $j
                	break 5
        	fi
	done
done

#3D Rendering
var=0
for i in 1 2 4 8; do
        var=0
        while : ; do
                var=$((var+1));
                cd ./src/
                timeout 10 ./nyx 2 $i
                EXIT=$?
                if [ $EXIT -eq 0 ] ; then
                        echo "success"
                        cd ../
                        break
                elif [ $EXIT -eq 124 ]; then
                        echo "timeout"
                        cd ../../Recover_Nyx/
                        ./clean_nyx
                        ./clean_nyx
                        cd ../Nyx/
                        if [ $var -eq 20 ]; then
                                echo "Reboot FPGA and restart from j = " $j
                                break 5
                        fi
                else
                        echo "Reboot FPGA and restart from j = " $j
                        break 5
                fi
        done
done

#Image Compression
var=0
for i in 1 2 4 8 12 16 32 64 128 256 512 1024; do
        var=0
        while : ; do
                var=$((var+1));
                cd ./src/
                timeout 20 ./nyx 3 $i
                EXIT=$?
                if [ $EXIT -eq 0 ] ; then
                        echo "success"
                        cd ../
                        break
                elif [ $EXIT -eq 124 ]; then
                        echo "timeout"
                        cd ../../Recover_Nyx/
                        ./clean_nyx
                        ./clean_nyx
                        cd ../Nyx/
                        if [ $var -eq 20 ]; then
                        	echo "Reboot FPGA and restart from j = " $j
                                break 5
                        fi
                else
                       	echo "Reboot FPGA and restart from j = " $j
                        break 5
                fi
        done
done

#Lenet
var=0
for i in 1 2 4 8 12 16 32 64 128 256 512 1024; do
        var=0
        while : ; do
                var=$((var+1));
                cd ./src/
                timeout 20 ./nyx 4 $i
                EXIT=$?
                if [ $EXIT -eq 0 ] ; then
                        echo "success"
                        cd ../
                        break
                elif [ $EXIT -eq 124 ]; then
                        echo "timeout"
                        cd ../../Recover_Nyx/
                        ./clean_nyx
                        ./clean_nyx
                        cd ../Nyx/
                        if [ $var -eq 20 ]; then
                       		echo "Reboot FPGA and restart from j = " $j
                                break 5
                        fi
                else
                       	echo "Reboot FPGA and restart from j = " $j
                        break 5
                fi
        done
done


#Digit Recognition
var=0
for i in 1 2 4 8 12 16; do
        var=0
        while : ; do
                var=$((var+1));
                cd ./src/
                timeout 80 ./nyx 5 $i
                EXIT=$?
                if [ $EXIT -eq 0 ] ; then
                        echo "success"
                        cd ../
                        break
                elif [ $EXIT -eq 124 ]; then
                        echo "timeout"
                        cd ../../Recover_Nyx/
                        ./clean_nyx
                        ./clean_nyx
                        cd ../Nyx/
                        if [ $var -eq 20 ]; then
                                echo "Reboot FPGA and restart from j = " $j
                                break 5
                        fi
                else
                        echo "Reboot FPGA and restart from j = " $j
                        break 5
                fi
        done
done

#Image Scaling
var=0
for i in 1 2 4 8 12 16 32 64; do
        var=0
        while : ; do
                var=$((var+1));
                cd ./src/
                timeout 150 ./nyx 6 $i
                EXIT=$?
                if [ $EXIT -eq 0 ] ; then
                        echo "success"
                        cd ../
                        break
                elif [ $EXIT -eq 124 ]; then
                        echo "timeout"
                        cd ../../Recover_Nyx/
                        ./clean_nyx
                        ./clean_nyx
                        cd ../Nyx/
                        if [ $var -eq 20 ]; then
                                echo "Reboot FPGA and restart from j = " $j
                                break 5
                        fi
                else
			echo "Reboot FPGA and restart from j = " $j
                        break 5
                fi
        done
done
done
