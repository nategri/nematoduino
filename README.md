## Description

<p align="center"><img src="/images/nematoduino.gif"></p>

Nematoduino is an Arduino UNO-compatible robotic emulation of a C. elegans nematode.

## Locomotion

<p align="center"><img width=700 src="/images/flow.jpeg"></p>
<p align="center"><b>Figure 1.</b><br><i>Flowchart depicting how motor output is generated.</i>.</p>

<p align="center"><img width=500 src="/images/signature_motor_neurons.png"></p>
<p align="center"><b>Figure 2.</b><br><i>A and B-type motor neuron activation in the connectome simulation during
chemotaxis (first half of time-series), and nose-touch (second half) neural input. Neurons that make good indicators
for forward/reverse are circled.</i>.</p>

## References

[1] Busbice, T. *Extending the C. Elegans Connectome to Robotics*. URL: https://goo.gl/pxavvY

[2] Zhen M., Samuel, A. *C. elegans locomotion: small circuits, complex functions.* URL: https://goo.gl/L1xXaw

[3] Morse, T. M., Ferrée, T. C., Lockery S. R. *Robust spatial navigation in a robot inspired by chemotaxis in C. elegans*.
URL: http://lox.uoregon.edu/papers/ab1998.pdf

## Related Projects

Busbice, T., Garrett, G., Churchill G. Github repository. URL: https://github.com/Connectome/GoPiGo  
*Notes: Propgenitor of this project. Connectome model originates from file connectome.py.*

Olmos, F. V. *Robot gusano con arduino, paso a paso.* Article URL: https://goo.gl/Pc9fBg Pastebin URL: https://pastebin.com/NbXUu0gh  
*Notes: Appears to be a more or less straight translation of the Busbice Python code. Runs on an Arduino Mega.*

Baek, S. *Arduino + C. Elegans*. YouTube video. URL: https://www.youtube.com/watch?v=B0m5DY3hjTg  
*Notes: Runs on an Arduino UNO board, but author indicates implementation issues. No source code.*

## License

Nematoduino is covered under the GNU Public License v2.
