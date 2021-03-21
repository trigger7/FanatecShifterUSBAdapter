# Fanatec Shifter USB Adapter Enclosure

Here is a 3D printed enclosure for the USB Adapter.

It is designed to fit a Teensy LC, a straight RJ12 jack (6P6C) and 2.54mm pin headers for extra connectivity.

## Tolerances

The enclosure has been designed with my printer and ABS in mind. As such, tolerances have been added for this purpose (approximatively 0.1 to 0.2 mm on every side of holes/pockets).

The FreeCAD source file is included on case you need to make adjustements.

Please note that, due to the way the enclosure is designed/built, you can modify any length/diameter constraint in any sketch (and the whole design will update), but all other modifications will likely break the object.

## RJ12 jack

The straight 6P6C jack used was bought at a local electonics store, but seems like a common design. It can be recognized by 2 small ribs on each side.

## Pin headers

For the pin header, 9 pins are available. Cut a length of 11 pins, but remove one pin en each side. This extra length is used to secure the headers to the top cover of the enclosure.

These pins can be used as needed, and will require modification to the code (they are not implemented by default).

For example, you can use them for 8 buttons (1 common ground + 8 pull-up inputs), 2 rotary encoders (common ground and 3.3V + 3 pins per encoder), or anything in between.