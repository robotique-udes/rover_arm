#!/usr/bin/env python

import rospy
import numpy as np
from numpy.linalg import inv
from rover_arm_msgs.srv import diff_kinematics_calc, diff_kinematics_calcResponse
from math import pi as PI
from numba import jit
from traceback import print_exc
import math as m

@jit(nopython=True) #si tu met pas nopython y'a une coupe de chose qui seront pas optimisé
def build_jacobienne(q1, q2, q3):
    q4 = 0.0
    #grandeur physique (const)
    J1x = 0.0
    J1y = 0.423
    J1z = 0.0

    J2x = 0.0
    J2y = 0.41
    J2z = 0.0

    J3x = 0.0
    J3y = 0.407
    J3z = 0.0

    # #Initialisation de la jacobienne
    # J1x*cos(q1) + J3z*sin(q1) + sin(q1)*(J1z+J2z) + J2x*cos(q1)*cos(q2) + J3x*cos(q1)*cos(q2+q3) - J2y*sin(q2)*cos(q1) - J3y*cos(q1)*sin(q2+q3)
    x_1 = J1x*-m.sin(q1) + J3z*m.cos(q1) + m.cos(q1)*(J1z+J2z) + J2x*-m.sin(q1)*m.cos(q2) + J3x*-m.sin(q1)*m.cos(q2+q3) - J2y*m.sin(q2)*-m.sin(q1) - J3y*-m.sin(q1)*m.sin(q2+q3)
    x_2 = J2x*m.cos(q1)*-m.sin(q2) + J3x*m.cos(q1)*-m.sin(q2+q3) - J2y*m.cos(q2)*m.cos(q1) - J3y*m.cos(q1)*m.cos(q2+q3)
    x_3 = J3x*m.cos(q1)*-m.sin(q2+q3) - J3y*m.cos(q1)*m.cos(q2+q3)

    # J1y + J2x*sin(q2) + J2y*cos(q2) + J3x*sin(q2+q3) + J3y*cos(q2+q3)
    y_1 = 0
    y_2 = J2x*m.cos(q2) + J2y*-m.sin(q2) + J3x*m.cos(q2+q3) + J3y*-m.sin(q2+q3)
    y_3 = J3x*m.cos(q2+q3) + J3y*-m.sin(q2+q3)

    # J3z*cos(q1) + cos(q1)*(J1z+J2z) + J2y*sin(q1)*sin(q2) + J3y*sin(q1)*sin(q2+q3) - J1x*sin(q1) - J2x*sin(q1)*cos(q2) - J3x*sin(q1)*cos(q2+q3)
    z_1 = J3z*-m.sin(q1) + -m.sin(q1)*(J1z+J2z) + J2y*m.cos(q1)*m.sin(q2) + J3y*m.cos(q1)*m.sin(q2+q3) - J1x*m.cos(q1) - J2x*m.cos(q1)*m.cos(q2) - J3x*m.cos(q1)*m.cos(q2+q3)
    z_2 = J2y*m.sin(q1)*m.cos(q2) + J3y*m.sin(q1)*m.cos(q2+q3) - J2x*m.sin(q1)*-m.sin(q2) - J3x*m.sin(q1)*-m.sin(q2+q3)
    z_3 = J3y*m.sin(q1)*m.cos(q2+q3) - J3x*m.sin(q1)*-m.sin(q2+q3)

    #Make this matrix without a list because it won't work otherwise
    # [x_1, x_2, x_3],
    # [y_1, y_2, y_3],
    # [z_1, z_2, z_3]
    j = np.zeros((3, 3))
    j[0][0] = x_1
    j[0][1] = x_2
    j[0][2] = x_3
    j[1][0] = y_1
    j[1][1] = y_2
    j[1][2] = y_3
    j[2][0] = z_1
    j[2][1] = z_2
    j[2][2] = z_3

    return j

def handle_diff_kinematics_calc(req):
    resp = diff_kinematics_calcResponse()

    commande = np.array([0.0] * 3).T

    try:
        # angles initiaux
        q1 = angle_rad(req.angles[0])
        q2 = angle_rad(req.angles[1])
        q3 = angle_rad(req.angles[2])

        #cmd
        for i in range(3):
           commande[i] = req.cmd[i]

        j = build_jacobienne(q1, q2, q3)
            
        resp.vitesses = np.matmul(inv(j), commande)

        resp.singularMatrix = 0;

    except Exception:
        rospy.logwarn("Singular matrix, encountered a column of 0 (can't divide by 0), jog in joint to a less critical position")
        resp.vitesses = [0, 0, 0, 0]
        resp.singularMatrix = 1;

    return resp

def angle_deg(angle: float):
    return angle*180.0/PI;

def angle_rad(angle: float):
    return angle*PI/180.0;

def diff_kinematics_calc_server():
    rospy.init_node('diff_kinematics_calc_server')
    diff_calc_service = rospy.Service('diff_kinematics_calc', diff_kinematics_calc, handle_diff_kinematics_calc)
    rospy.spin()

if __name__ == "__main__":
    diff_kinematics_calc_server()
