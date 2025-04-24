#!/bin/bash

var=0
for i in {0..9}; do
	var=0
	while : ; do
		var=$((var+1))
		cd ./src/
        	timeout 120 ./baseline $i 1.5
        	EXIT=$?

        	if [ $EXIT -eq 0 ] ; then
                	echo "success"
		        cd ../	
                	break
        	elif [ $EXIT -eq 124 ]; then
                	echo "timeout"
                	cd ../Baseline/
			if [ $var -eq 3 ]; then
				echo "Reboot FPGA and restart from i = " $i
				break 5
			fi
        	else
                	echo "Reboot FPGA and restart from i = " $i
			cd ../
                	break 5
        	fi
	done
done
