# Fanatec Shifter USB Adapter
Standalone USB adapter for Fanatec Clubsport Shifter SQ v1.5

Built using a Teensy LC and PlatformIO

Works using software hysteresis to prevent bouncing.

## Preliminary note
If all you need is connecting your Fanatec shifter to a USB port, your best bet is to purchase the official adapter.

The main advantage of this project is to modify it to include other peripherals, extra buttons, etc

## Thanks
Big thanks to [summivox](https://github.com/summivox) for inspiring me to make my own adapter, and for providing much helpful details about the shifter.

You can find his work about this at

 * hackaday : [wiring and reverse engineering](https://hackaday.io/project/171155-fanatec-clubsport-shifter-sq-v15-usb-adapter-diy)
 * github : [arduino code](https://gist.github.com/summivox/cfbcb8d309d416cefc3c0df10379339f)

## X axis

I decided to implement software hysteresis instead of traditional debouncing. Mostly for fun, I guess...

I also tried smoothing/filtering, but it seemed unnecessary. Especialy with the ADC set to 11 bits precision (instead of the maximum 12 bits).
There are unused leftovers of my attempt at smoothing, dig it up and activate it if you need/want.

### Calibration

I tried to auto-calibrate the X axis, with mixed results. It almost works, but as the gates of the shifter aren't evenly spaced, I had gear 4 sometimes "bleed" into gear 2.

I ended up hard-coding the values.
If you need to adjust these for your shifter :

 * Activate debug (uncomment [this line](https://github.com/trigger7/FanatecShifterUSBAdapter/blob/f0937c960d81fabd83f9cbca7249cea6fe212ee8/src/main.cpp#L5))
 * Upload the code onto your microcontroller
 * Open the serial monitor
 * Engage each odd gears (including 7th and reverse) and look at the X value on the serial monitor
* Take note of the average value for each gear
* Replace the values [here](https://github.com/trigger7/FanatecShifterUSBAdapter/blob/f0937c960d81fabd83f9cbca7249cea6fe212ee8/src/main.cpp#L57) with yours, in ascending order (7th, 5th, 3rd, 1st, reverse). The 32 at the end is the deadband for hysteresis, leave it as is.
* Recompile and reupload

## Y axis
The Y axis seems very stable and consistent, it shouldn't need any adjustment.

## Extra stuff

### Other microcontrollers
Other microcontrollers will probably work too. Other Teensys (Teensies?) in particular shouldn't need any modification.

For Arduinos (Arduini?), you will need to use the Joystick library instead of the "native" support from the Teensy.

### Arduino IDE
Converting to the Arduino IDE should be straightforward (rename src/main.cpp to <your_sketch>.ino and add the .h files from the include directory).

For Teensy, don't forget to set the USB type to "Serial + Keyboard + Mouse + Joystick". You can also downclock the CPU (I set my LC to 24MHz with "Fastest with LTO" optimization).

### Other shifters
It should be possible to use the same approach for the Logitech G25/G27/G29 shifters. As far as I know, they also use 2 analog axes for the gear stick.
Code will need extensive modifications though (different wiring, different number of gears, different sequential mode)...