#!/usr/bin/env python

# to build use this commande in cd ~/rovus0/src/rover_arm :
# chmod +x master.py
# Don't forget to source before run
# source ~/rovus0/devel/setup.bash

from matplotlib.pyplot import axis
import numpy as np
from numpy.linalg import inv
import math as m
import rospy
from rover_arm.msg import vitesse_moteur_msg
from rover_arm.msg import angle
from rover_arm.msg import feedback
from sensor_msgs.msg import Joy

#------------------------------------------------------------------------------------
#Constantes et variables globales
pi = 3.14159265359
debounce_time = 400 #for toggles in msec
fine_coarse_toggle = 0 # 0: toggle for coarse | 1: toggle for fine mean
nb_joint = 4 #Set le nombre de joint total du robot 
speed_increment = 0.1 #en deg/s
speed_base = 10 #en deg/s
vitesse_maximal = 20 #deg/s**
DEAD_ZONE = 0.25
MSG_PERIOD = 0.15 #s

#Timers
fdbkTimer_prevMillis = 0

#------------------------------------------------------------------------------------
#Classes
class controller():
    a = 0
    b = 0
    x = 0
    y = 0
    lt = 0
    rt = 0

    vx = 0.0
    vy = 0.0
    vz = 0.0

    joint_mode_toggle: bool = False
    joint_mode: bool = False
    joint_next: bool = False
    joint_prev: bool = False
    joint_current: int = 1
    calibration_button: bool = False

    speed_multiplier = 0.01
    speed_increase = 0

class Stepper():
    dir:bool = 0
    en: bool = 0
    period: float = 0

class ctrl_mouvement():
    x = 0
    z = 0
    y = 0
    a = 0

controller_1 = controller()
#mem_controller_1 = controller()
v = ctrl_mouvement()

m1 = Stepper()
m2 = Stepper()
m3 = Stepper()
m4 = Stepper()

#------------------------------------------------------------------------------------

def calcul_vitesse(robot, axe):
    #angles initiaux
    q1 = robot[0]
    q2 = robot[1]
    q3 = robot[2]
    q4 = robot[3]

    #grandeur physique (const)

    J1x = 0.0
    J1y = 0.2
    J1z = 0.0

    J2x = 0.0
    J2y = 0.5
    J2z = 0.0

    J3x = 0.0
    J3y = 0.5
    J3z = 0.0

    J4x = 0.0
    J4y = 0.25
    J4z = 0.0

    #Initialisation de la jacobienne

    #EqCinematique X = J1x*m.cos(q1) + m.sin(q1)*(J1z+J2z) + m.sin(q1)*(J3z+J4z) + J2x*m.cos(q1)*m.cos(q2) + J3x*m.cos(q1)*m.cos(q2+q3) + J4x*m.cos(q1)*m.cos(q2+q4) - J2y*m.sin(q2)*m.cos(q1) - J3y*m.cos(q1)*m.sin(q2+q3) - J4y*m.cos(q1)*m.sin(q2+q4)
    x_1 = J1x*-m.sin(q1) + m.cos(q1)*(J1z+J2z) + m.cos(q1)*(J3z+J4z) + J2x*-m.sin(q1)*m.cos(q2) + J3x*-m.sin(q1)*m.cos(q2+q3) + J4x*-m.sin(q1)*m.cos(q2+q4) - J2y*m.sin(q2)*-m.sin(q1) - J3y*-m.sin(q1)*m.sin(q2+q3) - J4y*-m.sin(q1)*m.sin(q2+q4)
    x_2 = J2x*m.cos(q1)*-m.sin(q2) + J3x*m.cos(q1)*-m.sin(q2+q3) + J4x*m.cos(q1)*-m.sin(q2+q4) - J2y*m.cos(q2)*m.cos(q1) - J3y*m.cos(q1)*m.cos(q2+q3) - J4y*m.cos(q1)*m.cos(q2+q4)
    x_3 = J3x*m.cos(q1)*-m.sin(q2+q3) - J3y*m.cos(q1)*m.cos(q2+q3)
    x_4 = J4x*m.cos(q1)*-m.sin(q2+q4) - J4y*m.cos(q1)*m.cos(q2+q4)

    #EqCinematique Y = J1y + J2x*m.sin(q2) + J2y*m.cos(q2) + J3x*m.sin(q2+q3) + J3y*m.cos(q2+q3) + J4x*m.sin(q2+q4) + J4y*m.cos(q2+q4)
    y_1 = 0
    y_2 = J2x*m.cos(q2) + J2y*-m.sin(q2) + J3x*m.cos(q2+q3) + J3y*-m.sin(q2+q3) + J4x*m.cos(q2+q4) + J4y*-m.sin(q2+q4)
    y_3 = J3x*m.cos(q2+q3) + J3y*-m.sin(q2+q3)
    y_4 = J4x*m.cos(q2+q4) + J4y*-m.sin(q2+q4)

    #EqCinematique Z = m.cos(q1)*(J1z+J2z) + m.cos(q1)*(J3z+J4z) + J2y*m.sin(q1)*m.sin(q2) + J3y*m.sin(q1)*m.sin(q2+q3) + J4y*m.sin(q1)*m.sin(q2+q4) - J1x*m.sin(q1) - J2x*m.sin(q1)*m.cos(q2) - J3x*m.sin(q1)*m.cos(q2+q3) - J4x*m.sin(q1)*m.cos(q2+q4)
    z_1 = -m.sin(q1)*(J1z+J2z) + -m.sin(q1)*(J3z+J4z) + J2y*m.cos(q1)*m.sin(q2) + J3y*m.cos(q1)*m.sin(q2+q3) + J4y*m.cos(q1)*m.sin(q2+q4) - J1x*m.cos(q1) - J2x*m.cos(q1)*m.cos(q2) - J3x*m.cos(q1)*m.cos(q2+q3) - J4x*m.cos(q1)*m.cos(q2+q4)
    z_2 = J2y*m.sin(q1)*m.cos(q2) + J3y*m.sin(q1)*m.cos(q2+q3) + J4y*m.sin(q1)*m.cos(q2+q4) - J2x*m.sin(q1)*-m.sin(q2) - J3x*m.sin(q1)*-m.sin(q2+q3) - J4x*m.sin(q1)*-m.sin(q2+q4)
    z_3 = J3y*m.sin(q1)*m.cos(q2+q3) - J3x*m.sin(q1)*-m.sin(q2+q3)
    z_4 = J4y*m.sin(q1)*m.cos(q2+q4) - J4x*m.sin(q1)*-m.sin(q2+q4)

    #EqCinematique a(angle effecteur) = q2+q4
    a_1 = 0
    a_2 = 1
    a_3 = 0
    a_4 = 1

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

#get controller inputs and multiply by a factor
def getAndScaleInputs():
    v.x = -controller_1.vx*controller_1.speed_multiplier
    v.y = controller_1.vy*controller_1.speed_multiplier
    v.z = -controller_1.vz*controller_1.speed_multiplier
    v.a = (controller_1.lt-controller_1.rt)*controller_1.speed_multiplier

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

def limiteur_de_vitesse(v_j1, v_j2, v_j3, v_j4 ):
        v_j = np.array([v_j1, v_j2, v_j3, v_j4])
        
        vitesse_la_plus_grande = abs(v_j[0])
        for i in range(1,4):
            if abs(v_j[i]) > vitesse_la_plus_grande:
                vitesse_la_plus_grande = abs(v_j[i])
    
        facteur_vitesse = vitesse_maximal/vitesse_la_plus_grande

        #rospy.loginfo(facteur_vitesse)

        for i in range(0,4):
            v_j[i] = v_j[i]*facteur_vitesse

            #rospy.loginfo(v_j[i])
        #rospy.loginfo("Limiter ON")

        return(v_j[0], v_j[1], v_j[2], v_j[3])

def getDir(m: Stepper):
    if m.period < 0:
        m.dir = False
    else:
        m.dir = True
        
#------------------------------------------------------------------------------------
def joy_callback(Joy: Joy):
    
    controller_1.a = Joy.buttons[0]
    controller_1.x = Joy.buttons[1]
    controller_1.y = Joy.buttons[2]
    controller_1.b = Joy.buttons[3]
    controller_1.lt = Joy.buttons[4]
    controller_1.rt = Joy.buttons[5]

    controller.vx = -Joy.axes[1]
    controller.vy = Joy.axes[4]
    controller.vz = Joy.axes[0]

    controller_1.calibration_button = Joy.buttons[12]

    controller_1.joint_mode_toggle = Joy.buttons[8]

    controller_1.joint_prev = Joy.axes[6]
    controller_1.joint_next = -Joy.axes[6]
    controller_1.speed_increase = Joy.axes[7]
    

    # rospy.loginfo('\n %f', controller_1.vx)
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

    #Change le mode de jog (joint/xyz)
    if controller_1.joint_mode_toggle:

        if controller_1.joint_mode == 1:
            controller_1.joint_mode = 0
        elif controller_1.joint_mode == 0:
            controller_1.joint_mode = 1

    #--------------------------------------------------
    if controller_1.speed_increase > 0:
        controller_1.speed_multiplier+=speed_increment
    if controller_1.speed_increase < 0:
        controller_1.speed_multiplier-=speed_increment

def angle_callback(angle: angle):
    global fdbkTimer_prevMillis
    #Définir type de message

    cmd = vitesse_moteur_msg()
    fdbk = feedback()
    
    fdbk.limiteur = 0   
    cmd.calib = 0       
    fdbk.calibration = 0
    if not controller_1.calibration_button:


        if controller_1.joint_mode:
            m1.period, m2.period, m3.period, m4.period = joint_mode()


        if not controller_1.joint_mode:
            #Calcul des vitesses moteurs et publier au topic           
            
            bras = np.array([[angle_rad(angle.j1)],
                            [angle_rad(angle.j2)],
                            [angle_rad(angle.j3)],
                            [angle_rad(angle.j4)]])
            
            getAndScaleInputs()
            
            ctrl = np.array([[v.x],
                            [v.y],
                            [v.z],
                            [v.a]])
            try:
                m1.period, m2.period, m3.period, m4.period = calcul_vitesse(bras, ctrl)
                
                #Max speed security
                if (abs(m1.period) > vitesse_maximal or 
                    abs(m2.period) > vitesse_maximal or
                    abs(m3.period) > vitesse_maximal or 
                    abs(m4.period) > vitesse_maximal):

                    m1.period, m2.period, m3.period, m4.period = limiteur_de_vitesse(m1.period, m2.period, m3.period, m4.period)
                    fdbk.limiteur = 1       
                
                fdbk.singular_matrix = 0        
            
            except Exception:
                fdbk.singular_matrix = 1        
                #rospy.logerr("Matrice singuliaire --> Jogger en joints")
    
    else:
        fdbk.calibration = 1      
        m1.period = 0
        m2.period = 0
        m3.period = 0
        m4.period = 0
        cmd.calib = 1
    
    #Commenter pour ne plus recevoir de feedback dans la console (log)
    #rospy.loginfo('Received : \nAngle 1:\t%d \nAngle 2:\t%d \nAngle 3:\t%d \nAngle 4:\t%d\n\n', angle.j1, angle.j2, angle.j3, angle.j4)
    #rospy.loginfo(ctrl)
    #rospy.logwarn("Joint mode: %d \t\tSelected Joint: %d", controller_1.joint_mode, controller_1.joint_current)
    #rospy.loginfo('Sending : \nVitesse M1:\t%f \nVitesse M2:\t%f \nVitesse M3:\t%f \nVitesse M4:\t%f\nSpeed Multiplier : %f\n\n\n\n\n\n\n\n\n\n\n\n', cmd.m1, cmd.m2, cmd.m3, cmd.m4, controller_1.speed_multiplier)

    getDir(m1)
    getDir(m2)
    getDir(m3)
    getDir(m4)

    cmd.m1_Period = m1.period
    cmd.m1_Dir = m1.dir
    cmd.m2_Period = m2.period
    cmd.m2_Dir = m2.dir
    cmd.m3_Period = m3.period
    cmd.m3_Dir = m3.dir
    cmd.m4_Period = m4.period
    cmd.m4_Dir = m4.dir

    pub.publish(cmd)

    #Build feedback message
    fdbk.angles = [angle.j1, angle.j2, angle.j3, angle.j4]
    fdbk.vitesses = [m1.period, m2.period, m3.period, m4.period]
    fdbk.enable = [1, 1, 1, 1]

    fdbk.ctrl_mode = controller_1.joint_mode
    fdbk.current_joint = controller_1.joint_current
    fdbk.speed_multiplier = controller_1.speed_multiplier

    if time() - fdbkTimer_prevMillis > MSG_PERIOD:
        pub_feedback.publish(fdbk)
        fdbkTimer_prevMillis = time()


def time(): return rospy.get_time()

#------------------------------------------------------------------------------------
if __name__=='__main__':
    rospy.init_node('master')

    pub = rospy.Publisher('vitesses_moteur', vitesse_moteur_msg, queue_size=10)

    pub_feedback = rospy.Publisher('rover_arm_feedback', feedback, queue_size=1)

    #Sub au controlleur
    sub_ctrl = rospy.Subscriber('joy', Joy, callback=joy_callback)
    #Sub aux valeurs d'angles
    sub_angle = rospy.Subscriber('valeurAngles', angle, callback=angle_callback)

    rospy.spin()
    