README
======

What is Launcher?
-----------------

Laucnher is a websockets daemon which allows to launch
custom C applications on GearS smart watch

Compilation
-----------

On ubuntu 16.04 you need to install `gcc`, `cmake`, `libjson-glib-dev` and 
`libwebsockets-dev` packages:

    $ sudo apt-get install gcc cmake libwebsockets-dev libjson-glib-dev

Create build directory and run cmake:

    $ mkdir build && cd build
    $ export CMAKE_PREFIX_PATH=/usr/lib/x86_64-linux-gnu #libraries location
    $ cmake ../

cmake will check all requirements and create build files. 
After that you can compile launcher with the following command:

    $ make


