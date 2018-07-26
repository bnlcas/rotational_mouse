# Rotational Mouse Prototype

The mouse is the leading input device for desktop computing, but for 3d interactions, a 2DoF controller can be very limiting. In principle, an optical mouse should be able to track rotation in addition to translation to provide a 3DoF (x, y, theta) controller.

This repository contains code for a prototype (still very crude) to enable rotational interactions on a mouse with a webcam duct taped on the side.

Currently the rotation of the camera is tracked using a visual odometry algorithm that was taken from the work of [Avi Singh](https://github.com/avisingh599/mono-vo). The rotational tracking can be found in the C++ portion of this project and requires OpenCV to be installed before you can build.

The unity demo gives an example. It uses a native plugin to track rotation, so it should be able run as a standalone project. The demo scene uses a 3d model of a calibi-yau manifold that was generated with code provided by
[Anders Sandberg](http://aleph.se/andart2/).
