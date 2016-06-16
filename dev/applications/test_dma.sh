#!/bin/bash



print_status_reg(){

	for i in /sys/module/dmatest/parameters/*; do

		if [ -f "$i" ]; then
			chmod a+r "$i"
			echo "`basename $i`: `cat "$i"`"
			echo "-------------------"
		fi
	done

}


echo -e "Available channel list"
ls -1 /sys/class/dma/
echo -e ""

echo -e "DMA test parameters"
ls -l /sys/module/dmatest/parameters

echo -e ""

iterations=1
channel=dma0chan0
timeout=2000
echo -e "Launching $iterations with $channel and timeout:$timeout"
echo -e "Run `cat /sys/module/dmatest/parameters/run`"
echo $channel > /sys/module/dmatest/parameters/channel
echo $timeout > /sys/module/dmatest/parameters/timeout
echo $iterations > /sys/module/dmatest/parameters/iterations
echo -e "Run before: `cat /sys/module/dmatest/parameters/run`"
print_status_reg
echo 1 > /sys/module/dmatest/parameters/run
dmesg | grep "$channel"
echo -e "Run after: `cat /sys/module/dmatest/parameters/run`"



iterations=42
channel=dma0chan1
echo -e "Launching $iterations with $channel and timeout:$timeout"
echo $timeout > /sys/module/dmatest/parameters/timeout
echo $iterations > /sys/module/dmatest/parameters/iterations
echo $channel > /sys/module/dmatest/parameters/channel
chmod +rwx /sys/module/dmatest/parameters/wait
echo 1 > /sys/module/dmatest/parameters/wait
print_status_reg
echo 1 > /sys/module/dmatest/parameters/run
echo -e "Wait `cat /sys/module/dmatest/parameters/wait`"
dmesg | grep "$channel"



