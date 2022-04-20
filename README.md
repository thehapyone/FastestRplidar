# FastestRplidar [![MIT License](https://img.shields.io/github/license/mashape/apistatus.svg)](https://github.com/thehapyone/FastestRplidar/blob/master/LICENSE) [![No Maintenance Intended](http://unmaintained.tech/badge.svg)](http://unmaintained.tech/)

A Fast Python Library (Helper) for the RPLidar A2 rangefinder scanners.
 
It is simple, lightweight, fast, Python module for working with RPLidar rangefinder scanners. Currently tested with only RPLidar A2, but should work with RPlidar A1 as well. 
 
This library is an extension or a wrapper for the C++ Slamtec RPLIDAR Public SDK for use in a Python environment. The underlying code is based on C++, but a Python wrapper was generated using the popular SWIG interface. To add, edit or change the codebase, you will need to re-compile using the SWIG inteface. 

The idea of using C++ for the underlying codebase is to give it the performance boost that was lacking in similar [RPLidar Library](https://github.com/SkoltechRobotics/rplidar). 

You can read more about this library in my blog [thehapyone - A Fast RPLidarA2 Python Library](https://thehapyone.com/a-fast-rplidara2-python-library-for-the-raspberry-pi/)


## Compatibility 
 - Python 3. Won't work with Python 2 unless re-complied.
 - Works only with Raspberry Pis or any other ARM-based processor. To make it work with other platforms like Windows or Linux, you will need to re-compile the C++ code.

## Note - Know Issues
If you have troubles making this work for your Raspberry PI, it is possible that you will need to re-compile the current binary or you can download the [Originals](https://github.com/thehapyone/FastestRplidar/releases/tag/0.1) release before new changes began to be introducded. That was definetely compiled for the ARM processor.

If you keep getting the error “wrong ELF class: ELFCLASS64” - This is because you are using a binary complied for a different host machine, you will have to re-compile again for your ARM device.
 
## Supports
 - Supports reading health
 - Can fetch 360 degree scan at once
 - Connect motor and stop motor
 - Some level of compatibility with SkoltechRobotics [RPLidar](https://github.com/SkoltechRobotics/rplidar) Library 

## Re-Compiling

Re-Compiling is easy and staight forward. You will require [SWIG](http://www.swig.org/>) installed, and have the [Slamtec RPLIDAR Public SDK for C++ available](https://github.com/slamtec/rplidar_sdk) Not really needed, I have included the compiled [static library file](https://github.com/thehapyone/FastestRplidar/blob/master/librplidar_sdk.a). To re-compile, go to the [source directory](https://github.com/thehapyone/FastestRplidar/tree/master/source(for%20recompiling)) and download the source files. Navigate to the source directory use the code below in terminal.

```C++
// generates the wrapper code. It will generate fastestrplidar_wrap.cxx
swig -c++ -python fastestrplidar.i
// Update the new library by running the setup.py with the below code
python3 setup.py build_ext --inplace
```

## Installing

No special installation is needed. Just download the source code and unzip into your working directory.

## Usage example

Simple example:

```Python
from myRplidar import RPlidar
# uses the default port ttyUSB0
lidar = RPlidar()

health = lidar.get_health()
print(health)

for i, scan in enumerate(lidar.iter_scans()):
    print('%d: Got %d measurments' % (i, len(scan)))
    if i > 10:
        break
        
# stops lidar and disconnect the driver
lidar.stopmotor()
```

In addition to it you can view example applications inside
[examples](https://github.com/thehapyone/FastestRplidar/tree/master/examples) directory.

## Future Changes
I don't intend to make new changes to this repository. It was a one-time project.
