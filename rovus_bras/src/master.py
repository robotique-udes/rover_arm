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
debounce_time = 400 #for toggles in msec
fine_coarse_toggle = 0 # 0: toggle for coarse | 1: toggle for fine mean
nb_joint = 4 #Set le nombre de joint total du robot 
speed_increment = 0.1 #en deg/s
speed_base = 10 #en deg/s

#------------------------------------------------------------------------------------
#Classes globales
class controller():
    a = 0
    b = 0
    x = 0
    y = 0
    lt = 0
    rt = 0

    joint_mode_toggle = 0
    joint_mode = 0
    joint_next = 0
    joint_prev = 0
    joint_current = 1

    speed_multiplier = 0.1
    speed_increase = 0
    

class ctrl_mouvement():
    x = 0
    z = 0
    y = 0
    
controller_1 = controller()
#mem_controller_1 = controller()
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
    return angle_deg(motorspeed[0]), angle_deg(motorspeed[1]), angle_deg(motorspeed[2]), angle_deg(motorspeed[3])

def angle_rad(deg):
    rad=deg*pi/180
    return rad

def angle_deg(rad):
    deg=rad*180/pi
    return deg

def axes():
    v.x = (controller_1.y - controller_1.a)*controller_1.speed_multiplier
    v.y = (controller_1.rt - controller_1.lt)*controller_1.speed_multiplier
    v.z = (controller_1.x - controller_1.b)*controller_1.speed_multiplier

def joint_mode():
    m=np.zeros(4)
    speed = 0

    if not controller_1.lt and not controller_1.rt:
        return m

    if controller_1.lt:
        speed=(0-speed_base)*controller_1.speed_multiplier

    if controller_1.rt:
        speed=speed_base*controller_1.speed_multiplier

    for i in range(0, nb_joint):
        if i == controller_1.joint_current-1:
            m[i] = speed

    angle_deg(m[0])
    angle_deg(m[1])
    angle_deg(m[2])
    angle_deg(m[3])
    return m

#------------------------------------------------------------------------------------
def joy_callback(Joy: Joy):
    
    controller_1.a = Joy.buttons[0]
    controller_1.x = Joy.buttons[1]
    controller_1.y = Joy.buttons[2]
    controller_1.b = Joy.buttons[3]
    controller_1.lt = Joy.buttons[4]
    controller_1.rt = Joy.buttons[5]

    controller_1.joint_mode_toggle = Joy.buttons[8]

    controller_1.joint_prev = Joy.axes[6]
    controller_1.joint_next = -Joy.axes[6]
    controller_1.speed_increase = Joy.axes[7]
    

    #rospy.loginfo('\n fine buttons pressed?: %d', controller_1.coarse_toggle)
    #controller_1.timestamp = Joy.header.stamp.nsecs/1000 #--> gets time between command for debouncing toggles in msec
    
    #--------------------------------------------------
    #Change le joint actif pour le mode Joint
    if controller_1.joint_next == 1:
        if controller_1.joint_current < nb_joint:
            controller_1.joint_current += 1
        elif controller_1.joint_current == nb_joint:
            controller_1.joint_current = 1
    if controller_1.joint_prev == 1:
        if controller_1.joint_current > 1:
            controller_1.joint_current -= 1
        elif controller_1.joint_current == 1:
            controller_1.joint_current = nb_joint
    #--------------------------------------------------

    ##Change le mode de jog (joint/xyz)
    if controller_1.joint_mode_toggle:

        if controller_1.joint_mode == 1:
            controller_1.joint_mode = 0
        elif controller_1.joint_mode == 0:
            controller_1.joint_mode = 1

    #--------------------------------------------------
    if controller_1.speed_increase >0:
        controller_1.speed_multiplier+=speed_increment
    if controller_1.speed_increase < 0:
        controller_1.speed_multiplier-=speed_increment

    


def angle_callback(angle: angle):
    #Définir type de message
    cmd = vitesse_moteur_msg()


    if controller_1.joint_mode:
        cmd.m1, cmd.m2, cmd.m3, cmd.m4 = joint_mode()


    if not controller_1.joint_mode:
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
        try:
            cmd.m1, cmd.m2, cmd.m3, cmd.m4 = calcul_vitesse(bras, ctrl)
        
        except Exception:
            rospy.logerr("Matrice singuliaire --> Jogger en joints")

    
    #Commenter pour ne plus recevoir de feedback dans la console
    #rospy.loginfo('Received : \nAngle 1:\t%d \nAngle 2:\t%d \nAngle 3:\t%d \nAngle 4:\t%d\n\n', angle.j1, angle.j2, angle.j3, angle.j4)
    #rospy.loginfo(ctrl)

    rospy.logwarn("Joint mode: %d \t\tSelected Joint: %d", controller_1.joint_mode, controller_1.joint_current)
    rospy.loginfo('Sending : \nVitesse M1:\t%f \nVitesse M2:\t%f \nVitesse M3:\t%f \nVitesse M4:\t%f\nSpeed Multiplier : %f\n\n\n\n\n\n\n\n\n\n\n\n', cmd.m1, cmd.m2, cmd.m3, cmd.m4, controller_1.speed_multiplier)

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
    