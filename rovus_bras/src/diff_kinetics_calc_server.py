#!/usr/bin/env python

import rospy 
import numpy as np
from numpy.linalg import inv
from rovus_bras.srv import diffKinematicsCalc, diffKinematicsCalcResponse

PI = 3.14159265359

def handle_diff_kinematics_calc(req):
    resp = diffKinematicsCalcResponse()

    rospy.loginfo("I Received %d", req.cmd[0])

    commande = np.array([[0], [0], [0], [0]])

    try:
        #angles initiaux
        q1 = req.angles[0]
        q2 = req.angles[1]
        q3 = req.angles[2]
        q4 = req.angles[3]

        #cmd
        for i in range(4):
            commande[i][0] = req.cmd[i]

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
        #EqCinematique X = J1x*np.cos(q1) + np.sin(q1)*(J1z+J2z) + np.sin(q1)*(J3z+J4z) + J2x*np.cos(q1)*np.cos(q2) + J3x*np.cos(q1)*np.cos(q2+q3) + J4x*np.cos(q1)*np.cos(q2+q4) - J2y*np.sin(q2)*np.cos(q1) - J3y*np.cos(q1)*np.sin(q2+q3) - J4y*np.cos(q1)*np.sin(q2+q4)
        x_1 = J1x*-np.sin(q1) + np.cos(q1)*(J1z+J2z) + np.cos(q1)*(J3z+J4z) + J2x*-np.sin(q1)*np.cos(q2) + J3x*-np.sin(q1)*np.cos(q2+q3) + J4x*-np.sin(q1)*np.cos(q2+q4) - J2y*np.sin(q2)*-np.sin(q1) - J3y*-np.sin(q1)*np.sin(q2+q3) - J4y*-np.sin(q1)*np.sin(q2+q4)
        x_2 = J2x*np.cos(q1)*-np.sin(q2) + J3x*np.cos(q1)*-np.sin(q2+q3) + J4x*np.cos(q1)*-np.sin(q2+q4) - J2y*np.cos(q2)*np.cos(q1) - J3y*np.cos(q1)*np.cos(q2+q3) - J4y*np.cos(q1)*np.cos(q2+q4)
        x_3 = J3x*np.cos(q1)*-np.sin(q2+q3) - J3y*np.cos(q1)*np.cos(q2+q3)
        x_4 = J4x*np.cos(q1)*-np.sin(q2+q4) - J4y*np.cos(q1)*np.cos(q2+q4)

        #EqCinematique Y = J1y + J2x*np.sin(q2) + J2y*np.cos(q2) + J3x*np.sin(q2+q3) + J3y*np.cos(q2+q3) + J4x*np.sin(q2+q4) + J4y*np.cos(q2+q4)
        y_1 = 0
        y_2 = J2x*np.cos(q2) + J2y*-np.sin(q2) + J3x*np.cos(q2+q3) + J3y*-np.sin(q2+q3) + J4x*np.cos(q2+q4) + J4y*-np.sin(q2+q4)
        y_3 = J3x*np.cos(q2+q3) + J3y*-np.sin(q2+q3)
        y_4 = J4x*np.cos(q2+q4) + J4y*-np.sin(q2+q4)

        #EqCinematique Z = np.cos(q1)*(J1z+J2z) + np.cos(q1)*(J3z+J4z) + J2y*np.sin(q1)*np.sin(q2) + J3y*np.sin(q1)*np.sin(q2+q3) + J4y*np.sin(q1)*np.sin(q2+q4) - J1x*np.sin(q1) - J2x*np.sin(q1)*np.cos(q2) - J3x*np.sin(q1)*np.cos(q2+q3) - J4x*np.sin(q1)*np.cos(q2+q4)
        z_1 = -np.sin(q1)*(J1z+J2z) + -np.sin(q1)*(J3z+J4z) + J2y*np.cos(q1)*np.sin(q2) + J3y*np.cos(q1)*np.sin(q2+q3) + J4y*np.cos(q1)*np.sin(q2+q4) - J1x*np.cos(q1) - J2x*np.cos(q1)*np.cos(q2) - J3x*np.cos(q1)*np.cos(q2+q3) - J4x*np.cos(q1)*np.cos(q2+q4)
        z_2 = J2y*np.sin(q1)*np.cos(q2) + J3y*np.sin(q1)*np.cos(q2+q3) + J4y*np.sin(q1)*np.cos(q2+q4) - J2x*np.sin(q1)*-np.sin(q2) - J3x*np.sin(q1)*-np.sin(q2+q3) - J4x*np.sin(q1)*-np.sin(q2+q4)
        z_3 = J3y*np.sin(q1)*np.cos(q2+q3) - J3x*np.sin(q1)*-np.sin(q2+q3)
        z_4 = J4y*np.sin(q1)*np.cos(q2+q4) - J4x*np.sin(q1)*-np.sin(q2+q4)

        #EqCinematique a(angle effecteur) = q2+q4
        a_1 = 0
        a_2 = 1
        a_3 = 0
        a_4 = 1

        j = np.array(  [[x_1, x_2, x_3, x_4],
                        [y_1, y_2, y_3, y_4],
                        [z_1, z_2, z_3, z_4],
                        [a_1, a_2, a_3, a_4]])
            
        resp.vitesses = np.matmul(inv(j), commande)
        resp.singularMatrix = 0;

    except:
        resp.vitesses = [0, 0, 0, 0]
        resp.singularMatrix = 1;

    return resp

def angle_deg(angle: float):
    return angle*180/PI;

def diff_kinematics_calc_server():
    rospy.init_node('diff_kinematics_calc_server')
    diff_calc_service = rospy.Service('diff_kinematics_calc', diffKinematicsCalc, handle_diff_kinematics_calc)
    rospy.spin()

if __name__ == "__main__":
    diff_kinematics_calc_server()