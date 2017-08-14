Nematoduino attempts to emulate the C. Elegans nervous system
on an Arduino Uno (AtMega328). It is based heavily upon the file
'connectome.py' from this repository:

https://github.com/Connectome/GoPiGo

This should still be considered beta software.

Arduino pin assignments can be configured in `pin_config.h`. Note
that all motor pins are PWM capable, with the same carrier frequency.

The C. Elegans 'chemotaxis' (food sensing) neurons are stimulated by
default, while the 'nose touch' response neurons are stimulated when
the robot senses an obstacle at a distance of 50 cm or less.

Nematoduino is covered under the GNU Public License v2.
