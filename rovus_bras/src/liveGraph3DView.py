#!/usr/bin/env python

from cmath import pi

from numpy import block
import rospy
import matplotlib as mpl
import matplotlib.pyplot as plt
import matplotlib.animation as animation
from matplotlib import style
from mpl_toolkits import mplot3d
import math as m
from rovus_bras.msg import angle

style.use('default')

#________________________________________________________________
#Constantes
ARM_COLOR = 'b'
JOINTCOLOR = 'o'

mpl.rcParams['lines.linewidth'] = 1

#Arm mesurements
J1x = 0
J1y = 0.2
J1z = 0

J2x = 0
J2y = 0.5
J2z = 0

J3x = 0
J3y = 0.5
J3z = 0

J4x = 0
J4y = 0.25
J4z = 0

q1 = 0
q2 = 0
q3 = 0
q4 = 0

#________________________________________________________________
#Fonctions

def main():
    rospy.init_node('liveGraph3DView')
    #Sub aux valeurs d'angles
    sub_angle = rospy.Subscriber('valeurAngles', angle, callback=angle_callback)

    plt.ion()

    fig = plt.figure(figsize=(8, 8))
    ax_3d = fig.add_subplot(1, 1, 1, projection='3d')

    while (not rospy.is_shutdown()):
        xs = []
        ys = []
        zs = []

        xs, ys, zs = calculateCoord()

        ax_3d.clear()
        ax_3d.set_title("3D view")
        ax_3d.plot3D(xs, ys, zs)
        ax_3d.scatter(xs, ys, zs)
        ax_3d.set_xlabel('z')
        ax_3d.set_ylabel('x')
        ax_3d.set_zlabel('y')
        ax_3d.set_xlim3d(-0.75, 0.75)
        ax_3d.set_ylim3d(0, 1.5)
        ax_3d.set_zlim3d(0, 1.5)
        ax_3d.grid(True)
        plt.draw()
        plt.pause(.01)

    rospy.spin()

def calculateCoord():
    J1ox = 0
    J1oy = 0
    J1oz = 0

    J2ox = J1x*m.cos(q1) + J1z*m.sin(q1)
    J2oy = J1y
    J2oz = J1z*m.cos(q1) - J1x*m.sin(q1)

    J3ox = J1x*m.cos(q1) + m.sin(q1)*(J1z+J2z) + J2x*m.cos(q1)*m.cos(q2) - J2y*m.sin(q2)*m.cos(q1)
    J3oy = J1y + J2x*m.sin(q2) + J2y*m.cos(q2)
    J3oz = m.cos(q1)*(J1z+J2z) + J2y*m.sin(q1)*m.sin(q2) - J1x*m.sin(q1) - J2x*m.sin(q1)*m.cos(q2)

    J4ox = J1x*m.cos(q1) + J3z*m.sin(q1) + m.sin(q1)*(J1z+J2z) + J2x*m.cos(q1)*m.cos(q2) + J3x*m.cos(q1)*m.cos(q2+q3) - J2y*m.sin(q2)*m.cos(q1) - J3y*m.cos(q1)*m.sin(q2+q3)
    J4oy = J1y + J2x*m.sin(q2) + J2y*m.cos(q2) + J3x*m.sin(q2+q3) + J3y*m.cos(q2+q3)
    J4oz = J3z*m.cos(q1) + m.cos(q1)*(J1z+J2z) + J2y*m.sin(q1)*m.sin(q2) + J3y*m.sin(q1)*m.sin(q2+q3) - J1x*m.sin(q1) - J2x*m.sin(q1)*m.cos(q2) - J3x*m.sin(q1)*m.cos(q2+q3)

    Px = J1x*m.cos(q1) + m.sin(q1)*(J1z+J2z) + m.sin(q1)*(J3z+J4z) + J2x*m.cos(q1)*m.cos(q2) + J3x*m.cos(q1)*m.cos(q2+q3) + J4x*m.cos(q1)*m.cos(q2+q4) - J2y*m.sin(q2)*m.cos(q1) - J3y*m.cos(q1)*m.sin(q2+q3) - J4y*m.cos(q1)*m.sin(q2+q4)
    Py = J1y + J2x*m.sin(q2) + J2y*m.cos(q2) + J3x*m.sin(q2+q3) + J3y*m.cos(q2+q3) + J4x*m.sin(q2+q4) + J4y*m.cos(q2+q4)
    Pz = m.cos(q1)*(J1z+J2z) + m.cos(q1)*(J3z+J4z) + J2y*m.sin(q1)*m.sin(q2) + J3y*m.sin(q1)*m.sin(q2+q3) + J4y*m.sin(q1)*m.sin(q2+q4) - J1x*m.sin(q1) - J2x*m.sin(q1)*m.cos(q2) - J3x*m.sin(q1)*m.cos(q2+q3) - J4x*m.sin(q1)*m.cos(q2+q4)

    xarray = [float(J1ox), float(J2ox), float(J3ox), float(J4ox), float(Px)]
    yarray = [float(J1oy), float(J2oy), float(J3oy), float(J4oy), float(Py)]
    zarray = [float(J1oz), float(J2oz), float(J3oz), float(J4oz), float(Pz)]

    return zarray, xarray, yarray

def angle_callback(angle: angle):
    global q1
    global q2
    global q3
    global q4
    q1 = angle.angle[0]*pi/180
    q2 = angle.angle[1]*pi/180
    q3 = angle.angle[2]*pi/180
    q4 = angle.angle[3]*pi/180

if __name__=='__main__':
    main()
