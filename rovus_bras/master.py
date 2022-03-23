#!/usr/bin/env python

# to build use this commande in cd ~/rovus0/src/rovus_bras :
# chmod +x master.py
# Don't forget to source before run
# source ~/rovus0/devel/setup.bash


import numpy as np
from numpy.linalg import inv
import math as m
import rospy
from rovus_bras.msg import vitesse_moteur_msg
from rovus_bras.msg import angle


def calcul_vitesse(robot, axe):
    #angles initiaux
    angle1 = robot[0]
    angle2 = robot[1]
    angle3 = robot[2]
    angle4 = robot[3]

    #grandeur physique (const)
    l1 = 0.7
    l2 = 1.5
    l3 = 2
    l4 = 0.5


    #Initialisation de la jacobienne
    x_1 = -m.sin(angle1) * (l2*m.sin(angle2) + l3*m.sin(angle2+angle3) + l4*m.sin(angle2+angle3+angle4))
    x_2 = m.cos(angle1) * (l2*m.cos(angle2) + l3*m.cos(angle2+angle3) + l4*m.cos(angle2+angle3+angle4))
    x_3 = m.cos(angle1) * (l3*m.cos(angle2+angle3) + (l4*m.cos(angle2+angle3+angle4)))
    x_4 = m.cos(angle1) * (l4*m.cos(angle2+angle3+angle4))

    y_1 = 0
    y_2 = -l2*m.sin(angle2) - l3*m.sin(angle2+angle3) - l4*m.sin(angle2+angle3+angle4)
    y_3 = - l3*m.sin(angle2+angle3) - l4*m.sin(angle2+angle3+angle4)
    y_4 = - l4*m.sin(angle2+angle3+angle4)

    z_1 = -m.cos(angle1) * (l2 * m.sin(angle2) + l3 * m.sin(angle2 + angle3) + l4 * m.sin(angle2 + angle3 + angle4))
    z_2 = -m.sin(angle1) * (l2 * m.cos(angle2) + l3 * m.cos(angle2 + angle3) + l4 * m.cos(angle2 + angle3 + angle4))
    z_3 = -m.sin(angle1) * (l3 * m.cos(angle2 + angle3) + (l4 * m.cos(angle2 + angle3 + angle4)))
    z_4 = -m.sin(angle1) * (l4 * m.cos(angle2 + angle3 + angle4))

    a_1 = 0
    a_2 = 1
    a_3 = 2
    a_4 = 3


    j = np.array([[x_1, x_2, x_3, x_4],
                  [y_1, y_2, y_3, y_4],
                  [z_1, z_2, z_3, z_4],
                  [a_1, a_2, a_3, a_4]])

    motorspeed = np.matmul(inv(j), axe)
    return motorspeed[0], motorspeed[1], motorspeed[2], motorspeed[3]


def angle_callback(angle: angle):
    cmd = vitesse_moteur_msg()


    bras = np.array([[angle.j1],
                     [angle.j2],
                     [angle.j3],
                     [angle.j4]])

    #faire le publisher (commande avec la manette)
    ctrl = np.array([[1],
                     [0],
                     [0],
                     [0]])

    cmd.m1, cmd.m2, cmd.m3, cmd.m4 = calcul_vitesse(bras, ctrl)

    pub.publish(cmd)

    rospy.loginfo('Sending : \n%f \n%f \n%f \n%f', cmd.m1, cmd.m2, cmd.m3, cmd.m4)



if __name__=='__main__':
    rospy.init_node('master2')

    pub = rospy.Publisher('vitesses_moteur', vitesse_moteur_msg, queue_size=10)
    sub = rospy.Subscriber("valeurAngles", angle, callback=angle_callback)

    rospy.spin()