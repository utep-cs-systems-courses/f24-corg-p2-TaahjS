Welcome to Simon Says!

The game will begin by displaying a pattern to the user which will come in the form of 5 lights.
During this time, you cannot use the switch to enter the input early. Only AFTER the 5
lights have been shown can you begin copying them.

To copy the pattern, you must use the switch on the MSP430 to switch between red and green. If
you press down, it is green. If you do nothing to the switch, it is red. The program will only
begin detecting your input when you press down once, so the beginning of the pattern will
always be green. You will be done putting in input after you press (or don't press) the light
5 times.

You can tell whether or not you have succeeded based on the noise. If it is high pitched, you
won. If it is low pitched, you lost. You can also tell through the lights. If you won, both lights
are lightly lit, and if you lost both lights are very dim.
