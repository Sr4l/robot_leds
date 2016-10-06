#!/usr/bin/env python
#coding: utf-8

import roslib; roslib.load_manifest('robot_leds')
import rospy

import serial
from time import sleep, localtime, strftime

SERIAL_DEVICE = "/dev/arduino_hl340"

def readable_time():
    return strftime("%d.%m.%Y %H:%M:%S", localtime())

def read_response(ser):
    resp = []
    while True:
        data = ser.read(1)
        if data == "\n" or len(resp) > 128:
            return "".join(resp)
        else:
            resp.append(data)

def leds_off(ser):
    ser.write(";")
    ser.flush()
    rospy.sleep(3)
    ser.write("0,0,0,0,0,0;")
    ser.write("1,0,0,0,0,0;")
    ser.flush()

def boden_leds_on(ser, hsv):
    ser.write("0,0,0,{},{},{};".format(*hsv))
    ser.flush()

def boden_get_hsv_color():
    ns = rospy.get_namespace().replace("/", "").strip()
    if ns.endswith("1"):
        rospy.loginfo("{} Choose color for Turtlebot1".format(readable_time()))
        return (170, 255, 255)
    elif ns.endswith("2"):
        rospy.loginfo("{} Choose color for Turtlebot2".format(readable_time()))
        return (0, 255, 255)
    elif ns.endswith("3"):
        rospy.loginfo("{} Choose color for Turtlebot3".format(readable_time()))
        return (85, 255, 255)
    elif ns.endswith("4"):
        rospy.loginfo("{} Choose color for Turtlebot4".format(readable_time()))
        return (43, 255, 255)
    else:
        rospy.loginfo("{} Choose color white for unknown Turtlebot".format(readable_time()))
        return (0, 0, 255)

def main():
    rospy.init_node('robot_leds')
    rospy.loginfo("{} Starting robot_leds".format(readable_time()))
    ser = serial.Serial(SERIAL_DEVICE, baudrate=9600, 
        parity=serial.PARITY_NONE,
        stopbits=serial.STOPBITS_ONE,
        bytesize=serial.EIGHTBITS,
        timeout=0.5)
    try:
        rospy.sleep(1)
        leds_off(ser)
        hsv = boden_get_hsv_color()
        while True:
            boden_leds_on(ser, hsv)
            sleep(15)
    except rospy.ROSInterruptException:
        pass
    finally:
        leds_off(ser)
    rospy.loginfo("{} Stoping muplant_led".format(readable_time()))

if __name__ == '__main__':
    main()
