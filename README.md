<p align="center"><img src="/images/nematoduino.gif"></p>
<p align="center"><i>For a longer duration clip, click here: https://youtu.be/u406m-v4Y3Y</i>.</p>

## Update

Revision 2 of nematoduino has been released! This version includes a number of bug fixes, but also some important architectural changes to make the project more accessible to hobbyists. In order of largest impact to new users, here are the biggest differences:

#### Preferred Robot Platform
The [AlphaBot2-Ar](https://www.waveshare.com/wiki/AlphaBot2-Ar) is now the preferred robotics platform for nematoduino, and the code is written to run immediately "out of the box" on this hardware. The AlphaBot2-Ar was chosen because it is cheap (~$60 for the version without an Arduino board), easy to assemble with only a screwdriver, and readily available from multiple online retailers.

That said, nematoduino is easy to port to other two wheeled robot platforms, and the source code in this repository should comprise a relatively straight-forward template to help intermediate Arduino enthusiasts adapt the software to their robot of choice.

#### Arduino Library
The `Nematoduino_Library` folder in this directory contains the nematoduino Arduino library. This library provides a simple interface to the simulated worm's biological state through the `.getLeftMuscle()` and `.getRightMuscle()` methods, allowing hobbyists to potentially bring C. elegans-on-an-Arduino to a variety of different contexts.

#### Nanotode
The nematoduino Arduino library uses
[nanotode](https://github.com/nategri/nanotode) for the core of its C. elegans
neural simulation. Nanotode is written in pure C, and so is intended as a
general framework for running simple C. elegans spiking neural networks in
resource constrained contexts. For an example of another project of mine that
uses nanotode, check out [nematode.farm](https://nematode.farm).

## Description

Nematoduino is an Arduino UNO-compatible robotic simulation of the C. elegans nematode.

The core of this simulation is the [nanotode](https://github.com/nategri/nanotode) framework, which implements a simple leaky integrate-and-fire model of the biological worm's spiking neural network. This implementation is based on
prior work [1], but with the following improvements:

* A compressed representation of the connectome network (8 kilobytes)
* Ability to run on the ubiquitous and low-cost Arduino UNO platform
* A biologically motivated conntectome-motor interface

The nematoduino software consumes a significant portion of the Arduino UNO's onboard program and dynamic memory (probably about half of each), but that still leaves plenty of room for tweaking and experimentation. It's my hope that nematoduino will end up in the hands of a
lot of students and neurorobotics hobbyists!

## Installation

First, download the nematoduino source by selecting the green 'Clone or download' button near the top of this page and clicking 'Download ZIP,' then unzip the file that downloads. Open the nemaoduino/nemtoduino.ino file in the Arduino IDE.

To compile and upload nematoduino to your Arduino UNO you must first include the nemtoduino library. You can do this in the Arduino IDE by going to

Sketch -> Include Library -> Add .ZIP Library...

and selecting the `Nematoduino_Library` folder located in this repository.

## Locomotion

As in Ref. 1, either the chemotaxis ("smell finding") or nose touch neurons are stimulated depending on whether
an obstacle detecting sensor has been triggered. A flow chart representing how sensor state feeds through the connectome
simulation and out through the motors is given in Figure 1. Particular weight is given to the neck muscles of the
worm, as they provide the direction of locomotion (see Ref. 3).

<p align="center"><img width=700 src="/images/flow.jpeg"></p>
<p align="center"><b>Figure 1.</b><br><i>Flowchart depicting how motor output is generated.</i>.</p>

Backward and forward movement in the biological worm are governed by A and B-type motor neurons, with a nose touch resulting in
a reversal of the worm's locomotion. Since backward motion is driven by A-type cells (see Fig.1 of Ref. 2), the average activation of these motor neurons in the connectome is used as a trigger for reverse locomotion in the robot.

<p align="center"><img width=500 src="/images/avg_motor_neurons.png"></p>
<p align="center"><b>Figure 2.</b><br><i>Average A and B-type motor neuron activation in the connectome simulation during
chemotaxis (first half of time-series), and nose-touch (second half). The hand-selected threshold for backward motion in the robot is indicated by a dashed line.</i>.</p>

Indeed, the normalized activity of A-type motor neurons shows a stronger response to nose touch input than B-type, so this approach seems reasonably well motivated.

## Further Work (To Do)

* You might notice that your nematoduino robot has a slight "limp" and lists to one side. This is because the motor response on each side is typically not symmetric in cheap robots. There might be some kind of "calibration routine" I could devise that could ameliorate this.

* It would be nice to use a light sensor to see if the qualitative chemotaxis behavior investigated in Ref. 3 can be reproduced
using this connectome model.

## Other C. elegans Projects by This Author

#### [nematode.farm](https://nematode.farm)
A simple WASM and SDL2 based game utilizing C. elegans simulations

#### [nanotode](https://github.com/nategri/nanotode)
A framework for running lightweight simulations of the C. elegans nematode's
nervous system. Forms the core of the simulation seen in this project and nematode.farm.

## References

[1] Busbice, T. *Extending the C. Elegans Connectome to Robotics*. URL: https://goo.gl/pxavvY

[2] Zhen M., Samuel, A. *C. elegans locomotion: small circuits, complex functions.* URL: https://goo.gl/L1xXaw

[3] Morse, T. M., Ferrée, T. C., Lockery S. R. *Robust spatial navigation in a robot inspired by chemotaxis in C. elegans*.
URL: http://lox.uoregon.edu/papers/ab1998.pdf

## Related Projects

Busbice, T., Garrett, G., Churchill G. *GoPiGo C. elegans Connectome Code*. Github repository.
URL: https://github.com/Connectome/GoPiGo  
*Notes: Propgenitor of this project. Connectome model originates from file connectome.py.*

Olmos, F. V. *Robot gusano con arduino, paso a paso.* Article URL: https://goo.gl/Pc9fBg Pastebin URL: https://pastebin.com/NbXUu0gh  
*Notes: Appears to be a more or less straight translation of the Busbice Python code. Runs on an Arduino Mega.*

Baek, S. *Arduino + C. Elegans*. YouTube video. URL: https://www.youtube.com/watch?v=B0m5DY3hjTg  
*Notes: Runs on an Arduino UNO board, but author indicates implementation issues. No source code.*

## About The Author

Hi! My name is Nathan Griffith, and by now I think it's clear that I like
implementing complicated stuff on simple hardware (see also:
[Animated GIFs on an Apple II](https://youtu.be/-Hu47_GWbtU)).

I'm an astrophysicist and definitely *not* a biologist, so if you work with
C. elegans in an academic setting and want to give me feedback I would really love that.

I can be reached at nategri@gmail.com

## License

Nematoduino is covered under the GNU Public License v2.

