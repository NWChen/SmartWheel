<<<<<<< HEAD
# driving-simulator

Requires numpy, and optionally glumpy if you want to run the graphics
visualization.

To install the dependencies, first install `pip`, and then

    pip install numpy
    pip install glumpy  # optional

To run,

    python ./main.py

To run with the visualization,

    python ./main.py --gl

There are several command-line options :

    python ./main.py --help
=======
# SmartWheel
Prevent accidents before they happen! Practice safe driving through car simulation.
With [@numaer](https://github.com/numaer), [@timotius02](https://github.com/timotius02), [@HunterWeis](https://github.com/HunterWeis).

SmartWheel provides a tangible safe driving experience through a custom driving simulation, an actuated and sensing steering wheel, and a machine learning approach to detecting impaired driving.

Hand positions on the wheel are detected via photosensors. Steering input and actuation is performed through a hacked servo. Addressable LEDs alert the driver to dangerous driving conditions, such as inebriation. These low-level hardware interactions are conducted by an Arduino sketch. Driver data over time is analyzed by a boosted decision tree on the Azure platform, which communicates with the SmartWheel via a Python script. A Unity Game Engine instance runs simultaneously, receiving serial data from the SmartWheel to simulate a vehicle on terrain.

Built with Arduino, Microsoft Azure, and Unity.

![](https://i.gyazo.com/0d330b2b1f3e69db3d707796f704d9e0.png) ![](http://challengepost-s3-challengepost.netdna-ssl.com/photos/production/software_photos/000/314/960/datas/gallery.jpg)
>>>>>>> 863e72d4e716f5fe80a55358bd0d84a18b4c7f3a
