###BeagleEye datamatrix scanner
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
BeagleEye is a Datamatrix scanner meant for the Beaglebone Black or similar small computer.  This program acquires a frame, scans it for datamatrices.  This was just a test project to see if I could get the BBB to scan for datamatrices. All future development will be done under the https://github.com/SWiT/RobotFightClub project.

###Setup
Download and install the latest Debian image from http://beagleboard.org/latest-images
This guide has been tested with the Debian 4GB 2015-03-01 release.

There are a few things that need to be tweaked once the image is running on a BBB.

Set a password, set the hostname (rfc_bbb#),Set the timezone, and add ll alias
```
passwd
nano /etc/hostname
sudo dpkg-reconfigure tzdata
echo 'alias ll="ls -la"' >> ~/.bash_aliases
sudo reboot
```

Grow the partition (optional)
```
cd /opt/scripts/tools
git pull
sudo ./grow_partition.sh
sudo reboot
```

Update and install prerequisites.
```
sudo apt-get update && sudo apt-get -y upgrade && sudo apt-get -y dist-upgrade
sudo apt-get -y purge libopencv-dev
sudo rm /usr/lib/libopencv_*
sudo apt-get -y install build-essential cmake pkg-config libtiff4-dev libjpeg-dev libjasper-dev libpng12-dev libavcodec-dev libavformat-dev libswscale-dev libv4l-dev libdmtx-dev libgtk2.0-dev
sudo reboot
```

Build and Install Open CV (This takes a few hours)
```
git clone https://github.com/Itseez/opencv.git
cd opencv && mkdir build && cd build
cmake -D CMAKE_BUILD_TYPE=RELEASE -D CMAKE_INSTALL_PREFIX=/usr/local -D WITH_CUDA=OFF -D WITH_OPENCL=OFF -D BUILD_opencv_apps=OFF -D BUILD_DOCS=OFF -D BUILD_PERF_TESTS=OFF -D BUILD_TESTS=OFF -D ENABLE_NEON=on ..
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
./beagleeye 127.0.0.1 3333
```


