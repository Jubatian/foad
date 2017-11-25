
Flight of a Dragon
==============================================================================

.. image:: screenshot.png
   :align: center
   :width: 100%

:Author:    Sandor Zsuga (Jubatian)
:License:   GNU GPLv3 (version 3 of the GNU General Public License)




Overview
------------------------------------------------------------------------------


Flight of a Dragon is a runner - platformer game for the Uzebox console
(http://www.uzebox.org) featuring a flightless dragon as protagonist who must
escape from his prison in an empire which wanted to break and train him to
use him as a war machine in their conquests.

As such he is a powerful fire-breather who can easily storm through
opposition, however the empire has a large and well equipped military all
around who can grind him down if he wasn't careful. It is also important to
be fast, to flee before forces could be mustered to thwart his attempt.

He doesn't want bloodshed, to be remembered as a monster, so he should be
cautious to not cause more losses than necessary, and he may also help people
(prisoners) on his way, supporting a probable uprising against the power.




Requirements
------------------------------------------------------------------------------


For playing the game you either need an Uzebox (or build something compatible
using an ATMega644p), see http://www.uzebox.org, or use an emulator such as
CUzeBox or Uzem. For peripherals you need an SNES controller only (no SD card
is required).

Game binaries are provided (a .hex and a .uze file, the former can be burned
in the ATMega directly, the latter can be used with Uzeboxes with an SD card
slot and a bootloader) in the _bin_ folder, so you don't necessarily need to
compile if you just want to play it.




Compiling the game
------------------------------------------------------------------------------


You need the avr-gcc toolchain to compile the game. It should build fine using
Make producing the .hex file. To get a .uze file, you need the UzeRom packager
(packrom) from the UzeBox project, set up its path within the Makefile.




Alternate licenses
------------------------------------------------------------------------------


All the game contents created by Jubatian (Sandor Zsuga) may also be used
according to the Creative Commons CC-BY-SA 4.0 license. Note that the game
kernel contains components which are created by various authors (from the
UzeBox project) which can only be used under GPLv3.
