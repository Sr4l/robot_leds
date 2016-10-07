robot_leds
==========

This repository holds the robot_leds packages for
[ROS][1] [Turtlebots][2]. For more information see `package.xml` file.
For license see `LICENSE` and the source code.

The robot_leds package, is meant to dynamically light up or color robots
in a multi robot environment to distinguish them easily from each other.
It's designed to use NeoPixel/WS2812B RGB Leds an Arduino Nano and on
USB port as power and serial interface and other USB ports as additional
power source.

This code is not really configurable. You may have to change the
sourcecode to get a certain behaviour. Also the Arduino code supports
animation but the ROS package don't use them.

Creat symlink for arduino
-------------------------

Create the file /etc/udev/rules.d/60-objdev.rules with the following content:

```udev
SUBSYSTEM!="usb_device", ACTION!="add", GOTO="objdev_rules_end"

# Arduino als /dev/arduino
ATTRS{idVendor}=="0403", ATTRS{idProduct}=="6001", SYMLINK+="arduino_ftdi"
ATTRS{idVendor}=="1a86", ATTRS{idProduct}=="7523", SYMLINK+="arduino_hl340"

LABEL="objdev_rules_end"
```

and restart udev:

```bash
sudo /etc/init.d/udev restart
# or
sudo systemctl restart udev.service
```

after this you might want to add yourself to the grup dialout to use
serial devices:

```bash
sudo usermod -a -G dialout $USER
```

You can now access original arduinos as `/dev/arduino_ftdi` and clones
as `/dev/arduino_hl340`

[1]: http://www.ros.org
[2]: http://wiki.ros.org/Robots/TurtleBot
