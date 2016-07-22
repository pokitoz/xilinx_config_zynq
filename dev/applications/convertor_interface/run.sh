#!/bin/bash

script_dir_abs=$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )
cd "${script_dir_abs}"

../load_driver.sh ../dma/driver/pl_axi_dma_driver.ko
make clean
make

rm -rf "./*.bmp"

echo "Launching demo"
./demo_convertor
echo "Done"