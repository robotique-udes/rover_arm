#!/usr/bin/env python

# to build use this commande in cd ~/rovus0/src/rover_arm :
# chmod +x master.py
# Don't forget to source before run
# source ~/rovus0/devel/setup.bash

import numpy as np
import rospy
from rover_arm.msg import vitesse_moteur_msg
from rover_arm.msg import angle


def callback(angle):
    rospy.loginfo("J1 : %d", angle.j1)
    rospy.loginfo("J2 : %d", angle.j2)
    rospy.loginfo("J3 : %d", angle.j3)
    rospy.loginfo("J4 : %d\n", angle.j4)


def listenandwrite():        
    rospy.init_node('master', anonymous=True)
    sub = rospy.Subscriber('valeur_angles', angle, callback)
    pub = rospy.Publisher('vitesse_moteur', vitesse_moteur_msg, queue_size=10)

    rate = rospy.Rate(1)
    msg = vitesse_moteur_msg()
    


    #msg.m1 = data.j1
    #msg.m2 = data.j2
    #msg.m3 = data.j3
    #msg.m4 = data.j4


    while not rospy.is_shutdown():
        rospy.loginfo("\n%s\n\n", msg)       
        pub.publish(msg)
        rate.sleep()



if __name__ == '__main__':
    try:
        listenandwrite()
    except rospy.ROSInterruptException:
        pass
