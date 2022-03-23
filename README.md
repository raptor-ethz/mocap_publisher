# RAPTOR Motion Capture Publisher

This application publishes motion capture data from a ViCon motino capture system on a FastDDS domain. It does so for multiple objects which can be specified as command line arguments. 

## Installation

You will need the following dependencies installed on your system:
- [eProsima FastDDS](https://fast-dds.docs.eprosima.com/en/latest/installation/binaries/binaries_linux.html)
- [MAVSDK](https://mavsdk.mavlink.io/main/en/cpp/guide/installation.html)
- yaml-cpp: This can be installed with `sudo apt install libyaml-cpp-dev` on Ubuntu and `brew install yaml-cpp` on macOS
  
After the dependencies have been installed, clone this repository using: 

```bash
git clone --recursive https://github.com/raptor-ethz/mocap_publisher
```

Then, you will be able to compile and build the project with cmake. 

## Usage

First of all, you will need to tap into the datastream of your motion capture system on the local network. The IP for this is usually statically configured and hard-coded for convenience. You will need to change this accordingly. 

Then, you can run the executable and enter the names of the objects in the ViCon tracker for which you want to publish the pose. Currently, this is limited to 10 objects, but this length can be changed in the code. 

