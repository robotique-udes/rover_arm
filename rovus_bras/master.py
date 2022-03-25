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
from sensor_msgs.msg import Joy

#------------------------------------------------------------------------------------
#Constantes et variables globales
pi = 3.14159265359
debounce_time = 4000 #for toggles in msec
fine_speed = 100 #division of m/s ex: 100 = cm/s
coarse_speed = 10 #division of m/s ex: 100 = cm/s
fine_coarse_toggle = 0 # 0: toggle for coarse | 1: toggle for fine mean

#------------------------------------------------------------------------------------
#Classes globales
class controller():
    a = 0
    b = 0
    x = 0
    y = 0
    lt = 0
    rt = 0
    adjustment = 0
class ctrl_mouvement():
    x = 0
    z = 0
    y = 0
    
controller_1 = controller()
v = ctrl_mouvement()
    
#------------------------------------------------------------------------------------
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

def angle_rad(deg):
    rad=180*deg/pi
    return rad

def axes():
    if fine_coarse_toggle == 0:
        if controller_1.adjustment == 0:
            v.x = (controller_1.y - controller_1.a)/(fine_speed)
            v.y = (controller_1.rt - controller_1.lt)/(fine_speed)
            v.z = (controller_1.x - controller_1.b)/(fine_speed)

        elif controller_1.adjustment == 1:
            v.x = (controller_1.y - controller_1.a)/(coarse_speed)
            v.y = (controller_1.rt - controller_1.lt)/(coarse_speed)
            v.z = (controller_1.x - controller_1.b)/(coarse_speed)   


    if fine_coarse_toggle == 1:
        if controller_1.adjustment == 1:
            v.x = (controller_1.y - controller_1.a)/(fine_speed)
            v.y = (controller_1.rt - controller_1.lt)/(fine_speed)
            v.z = (controller_1.x - controller_1.b)/(fine_speed)

        elif controller_1.adjustment == 0:
            v.x = (controller_1.y - controller_1.a)/(coarse_speed)
            v.y = (controller_1.rt - controller_1.lt)/(coarse_speed)
            v.z = (controller_1.x - controller_1.b)/(coarse_speed)



#------------------------------------------------------------------------------------
def joy_callback(Joy: Joy):
    
    controller_1.a = Joy.buttons[0]
    controller_1.x = Joy.buttons[1]
    controller_1.y = Joy.buttons[2]
    controller_1.b = Joy.buttons[3]
    controller_1.lt = Joy.buttons[4]
    controller_1.rt = Joy.buttons[5]
    controller_1.adjustment = Joy.buttons[8]
    rospy.loginfo('\n fine buttons pressed?: %d', controller_1.adjustment)
    #controller_1.timestamp = Joy.header.stamp.nsecs/1000 #--> gets time between command for debouncing toggles in msec

def angle_callback(angle: angle):
    #Définir type de message
    cmd = vitesse_moteur_msg()


#Calcul des vitesses moteurs et publier au topic   
    bras = np.array([[angle_rad(angle.j1)],
                     [angle_rad(angle.j2)],
                     [angle_rad(angle.j3)],
                     [angle_rad(angle.j4)]])
    
    axes() #transfere les inputs en cm/s
    ctrl = np.array([[v.x],
                     [v.y],
                     [v.z],
                     [0]])
    cmd.m1, cmd.m2, cmd.m3, cmd.m4 = calcul_vitesse(bras, ctrl)

    
    #Commenter pour ne plus recevoir de feedback dans la console
    #rospy.loginfo('Received : \nAngle 1:\t%d \nAngle 2:\t%d \nAngle 3:\t%d \nAngle 4:\t%d\n\n', angle.j1, angle.j2, angle.j3, angle.j4)
    #rospy.loginfo(ctrl)
    #rospy.loginfo('Sending : \nVitesse M1:\t%f \nVitesse M2:\t%f \nVitesse M3:\t%f \nVitesse M4:\t%f\n\n', cmd.m1, cmd.m2, cmd.m3, cmd.m4)

    pub.publish(cmd)

#------------------------------------------------------------------------------------
if __name__=='__main__':
    rospy.init_node('master')

    pub = rospy.Publisher('vitesses_moteur', vitesse_moteur_msg, queue_size=10)
    #Sub au controlleur
    sub_ctrl = rospy.Subscriber('joy', Joy, callback=joy_callback)
    #Sub aux valeurs d'angles
    sub_angle = rospy.Subscriber('valeurAngles', angle, callback=angle_callback)

    rospy.spin()