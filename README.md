<p align="center"><img src="/images/nematoduino.gif"></p>

## Description

Nematoduino is an Arduino UNO-compatible robotic simulation of the C. elegans nematode.

At the core of the simulation is a spiking neural network incorporating 300 neuron cells of the
biological worm's connectome, along with associated muscle cells. This implementation is based on
prior work [1], but with the following improvements:

* A compressed representaion of the connectome network (8 kilobytes)
* Ability to run on the extremely flexible Arduino UNO platform
* A biologically motivated conntectome-motor interface

In its current form the nematoduino software consumes 13542 bytes (42%) of the program memory
of an UNO-compatible board, and 825 bytes (40%) of the SRAM, leaving plenty of room for experimentation
and further development. It's my hope that nematoduino will end up in the hands of a lot of students
and neurorobotics hobbyists!

## Locomotion

As in Ref. 1, either the chemotaxis ("smell finding") or nose touch neurons are stimulated depending on whether
the ultrasonic sensor has been triggered. A flow chart representing how sensor state feeds through the connectome
simulation and out through the motors is given in Figure 1. Particular weight is given to the neck muscles of the
worm, as they provide the direction of locomotion (see Ref. 3).

<p align="center"><img width=700 src="/images/flow.jpeg"></p>
<p align="center"><b>Figure 1.</b><br><i>Flowchart depicting how motor output is generated.</i>.</p>

Forward and backward movement are governed by A and B-type motor neurons [2], with a nose touch resulting in
a reversal of the worm's locomotion. Keeping this in mind, the activation of these motor neurons in the connectome
simulation was inspected during chemotaxis and nose touch (see Figure 2), and several groups of neurons
were found the be good indicators for forward and reverse locomotion.

<p align="center"><img width=500 src="/images/signature_motor_neurons.png"></p>
<p align="center"><b>Figure 2.</b><br><i>A and B-type motor neuron activation in the connectome simulation during
chemotaxis (first half of time-series), and nose-touch (second half). Neurons that make good indicators
for forward/reverse are circled.</i>.</p>

These significant motor neurons are weighted and tracked using a running average. The sign of the signal sent to the motors
is reversed when this average crosses a threshold.

## Further Work (To Do)

It would be nice to use a light sensor to see if the qualitative chemotaxis behavior investigated in Ref. 3 can be reproduced
using this connectome model.

## References

[1] Busbice, T. *Extending the C. Elegans Connectome to Robotics*. URL: https://goo.gl/pxavvY

[2] Zhen M., Samuel, A. *C. elegans locomotion: small circuits, complex functions.* URL: https://goo.gl/L1xXaw

[3] Morse, T. M., Ferrée, T. C., Lockery S. R. *Robust spatial navigation in a robot inspired by chemotaxis in C. elegans*.
URL: http://lox.uoregon.edu/papers/ab1998.pdf

## Related Projects

Busbice, T., Garrett, G., Churchill G. *GoPiGo C. elegans Connectome Code*. Github repository.
URL: https://github.com/Connectome/GoPiGo  
*Notes: Propgenitor of this project. Connectome model originates from file connectome.py.*

Griffith, N. Github Repository. *c_matoduino*. URL: https://github.com/nategri/c_matoduino
*Notes: Author's own work. A pure C implementation of the connectome model useful for plotting purposes.*

Olmos, F. V. *Robot gusano con arduino, paso a paso.* Article URL: https://goo.gl/Pc9fBg Pastebin URL: https://pastebin.com/NbXUu0gh  
*Notes: Appears to be a more or less straight translation of the Busbice Python code. Runs on an Arduino Mega.*

Baek, S. *Arduino + C. Elegans*. YouTube video. URL: https://www.youtube.com/watch?v=B0m5DY3hjTg  
*Notes: Runs on an Arduino UNO board, but author indicates implementation issues. No source code.*

## About The Author

Hi! My name is Nathan Griffith, and by now I think it's clear that I like
implementing complicated stuff on simple hardware
(https://youtu.be/-Hu47_GWbtU).

I'm an astrophysicist and definitely *not* a biologist, so if you work with
C. elegans in an academic setting and want to give me feedback I would really love that.

I can be reached at nategri@gmail.com

## License

Nematoduino is covered under the GNU Public License v2.
