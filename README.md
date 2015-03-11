###BeagleEye network datamatrix scanner
Copyright (C) 2015 Matthew Gary Switlik

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
  
###About 
BeagleEye is a Datamatrix scanner meant for the Beaglebone Black or similar small computer.  This program acquires a frame, scans it for datamatrices, compresses it, then outputs the frame data and all data associated with any found datamatrix over the network to the Robot Fight Club Server.  The BeagleEye scanner uses OpenCV 3.0 and libdmtx 0.7.2. A Beaglebone Black with a class 10 4GB microSD card is recommended. A Flasher image for the 2GB eMMC is planned at somepoint. Currently I've only run BeagleEye using Ubuntu 14.04 but other distros should work.

###Setup
Download and install the Ubuntu console img file https://rcn-ee.net/rootfs/2015-02-19/microsd/bone-ubuntu-14.04.2-console-armhf-2015-02-19-2gb.img.xz

There were a few things that needed tweaking once the image was running on a BBB.
The DNS server was hardcoded to 192.168.1.1. Add "nameserver 8.8.8.8" to /etc/resolvconf/resolv.conf.d/original
```
sudo nano /etc/resolvconf/resolv.conf.d/original
sudo resolvconf -u
```

Set the timezone
```
dpkg-reconfigure tzdata
```

Grow the partition
```
cd /opt/scripts/tools
git pull
sudo ./grow_partition.sh
sudo reboot
```

Update and install prerequisites.
```
sudo apt-get update
sudo apt-get upgrade
sudo apt-get install build-essential cmake pkg-config
sudo apt-get install libtiff4-dev libjpeg-dev libjasper-dev libpng12-dev
sudo apt-get install libavcodec-dev libavformat-dev libswscale-dev libv4l-dev
sudo apt-get install libdmtx-dev
sudo apt-get install libgtk2.0-dev
```

Build and Install Open CV (This takes a few hours)
```
git clone https://github.com/Itseez/opencv.git
cd opencv
mkdir build && cd build
cmake -D CMAKE_BUILD_TYPE=RELEASE -D CMAKE_INSTALL_PREFIX=/usr/local -D WITH_CUDA=OFF -D WITH_CUFFT=OFF -D WITH_CUBLAS=OFF -D WITH_NVCUVID=OFF -D WITH_OPENCL=OFF -D WITH_OPENCLAMDFFT=OFF -D WITH_OPENCLAMDBLAS=OFF -D BUILD_opencv_apps=OFF -D BUILD_DOCS=OFF -D BUILD_PERF_TESTS=OFF -D BUILD_TESTS=OFF -D ENABLE_NEON=on ..
make
sudo make install
sudo ldconfig
cd ~
```

Build and run BeagleEye
```
git clone https://github.com/SWiT/BeagleEye.git
cd BeagleEye
g++ beagleeye.cpp -o beagleeye -lopencv_core -lopencv_videoio -lopencv_imgcodecs -ldmtx
./beagleeye
```
