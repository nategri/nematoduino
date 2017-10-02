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

## References

Busbice, T. *Extending the C. Elegans Connectome to Robotics*. SOLID
2014 Conference. URL: https://goo.gl/pxavvY

Zhen M., Samuel, A. *C. elegans locomotion: small circuits, complex functions.* URL: https://goo.gl/L1xXaw

Morse, T. M., Ferrée, T. C., Lockery S. R. *Robust spatial navigation in a robot inspired by chemotaxis in C. elegans*.
URL: http://lox.uoregon.edu/papers/ab1998.pdf

## Related Projects

Busbice, T., Garrett, G., Churchill G. Github repository. URL: https://github.com/Connectome/GoPiGo
*Notes: Propgenitor of this project. Connectome model originates from file connectome.py.*

Olmos, F. V. *Robot gusano con arduino, paso a paso.* Article URL: https://goo.gl/Pc9fBg Pastbin URL: https://pastebin.com/NbXUu0gh.
*Notes: Appears to be a more or less straight translation of the Busbice Python code. Runs on Arduino Mega.*

Baek, S. *Arduino + C. Elegans*. YouTube video. URL: https://www.youtube.com/watch?v=B0m5DY3hjTg
*Notes: Runs on an Arduino UNO board, but author indicates implementation issues. No source code.*

## License

Nematoduino is covered under the GNU Public License v2.
