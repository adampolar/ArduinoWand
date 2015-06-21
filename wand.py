#!/usr/bin/env python3
'''Plots values from arduino wand device onto screen'''
import serial
import re
from graphics import *

WINDOW_SIZE = 1000

class Wand(object):

    def __init__(self):
        self.serial = serial.Serial('/dev/ttyACM0', 115200, timeout = 1)
        self.init_vals = self.try_parse_output()

        while (not self.init_vals):
            self.init_vals = self.try_parse_output()

    def try_parse_output(self):
        while True:
            try:
                line = self.serial.readline()
                vals = self.parse_line_to_values(line)
                if (len(vals) != 12):
                    continue
                return vals
            except Exception as e:
                print(e)
                # Something went wrong when we tried to parse the line. Let's try again.

    def parse_line_to_values(self, line):
        '''Parses the serial output to obtain values for the accelerometer and
        magnetometer. Example format of line 
        xa-940    ya248   za14748 xg-458  yg-22   zg-175  xm-56   ym-133  zm-51   ya-2.40 pi-1.94 ro-2.74'''
        vals = re.findall("[-+]?\d+[\.]?\d*", line.decode("utf-8"))

        def format_values(vals):
            accel_gyro_magnet_vals = [int(x) for x in vals[0:9]]
            yaw_pitch_roll_vals = [int(round(100 * float(x))) for x in vals[9:]]
            return accel_gyro_magnet_vals + yaw_pitch_roll_vals

        vals = format_values(vals)
        return vals

    def normalise_values(self):
        vals = self.try_parse_output()

        new_vals = []
        for idx, val in enumerate(vals):
            new_vals.append(val - self.init_vals[idx])
        return new_vals

class Grapher(object):
    
    def __init__(self, window_size):
        self.window_size = window_size
        self.sensitivity = window_size/100000
        self.window = GraphWin("adam", window_size, window_size)
        self.centre = self.window_size/2
        #accel output, gyroscope output, magnetometer output, ypr output

    def get_new_accel_point(self, xval, yval, zval):
        new_yval = self.centre + yval * self.sensitivity
        new_xval = self.centre + xval * self.sensitivity
        return Point(new_yval, new_xval)

    def get_new_gyro_point(self, xval, yval, zval):
        new_xval = self.centre + xval * self.sensitivity
        new_yval = self.centre - yval * self.sensitivity
        return Point(new_xval, new_yval)

    def get_new_magnet_point(self, xval, yval, zval):
        new_xval = self.centre + xval
        new_yval = self.centre - yval
        return Point(new_xval, new_yval)

    def get_new_ypr_point(self, xval, yval, zval):
        new_xval = self.centre + xval
        new_yval = self.centre - yval
        return Point(new_xval, new_yval)

    def draw(self, vals):
        accel_point = self.get_new_accel_point(*vals[0:3])
        accel_point.setFill('red')
        accel_point.draw(self.window)

        gyro_point = self.get_new_gyro_point(*vals[3:6])
        gyro_point.setFill('green')
        gyro_point.draw(self.window)

        magnet_point = self.get_new_magnet_point(*vals[6:9])
        magnet_point.setFill('blue')
        magnet_point.draw(self.window)

        #ypr_point = self.get_new_ypr_point(*vals[9:12])
        #ypr_point.setFill('black')
        #ypr_point.draw(self.window)

def main():
    wand = Wand()
    grapher = Grapher(WINDOW_SIZE)

    while True:
        grapher.draw(wand.normalise_values())

if __name__ == '__main__':
    main()
