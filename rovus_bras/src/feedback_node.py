#!/usr/bin/env python

from numpy import angle
import rospy
from rovus_bras.msg import feedback
import PySimpleGUI as sg
import time


#------------------------------------------------
#Variables globale
update_time = 100

class display_fdbk:
    
    singular_matrix = False
    
    j1 = 0
    j2 = 0
    j3 = 0
    j4 = 0

    m1 = 0
    m2 = 0
    m3 = 0
    m4 = 0

    ctrl_mode = 0
    current_joint = 0
    speed_multiplier = 0


fdbk = display_fdbk()

#--------------------------------------------------
#ROS
def feedback_callback(feedback: feedback):
    fdbk.singular_matrix = feedback.singular_matrix
    fdbk.j1 = str(round(feedback.j1, 2))
    fdbk.j2 = str(round(feedback.j2, 2))
    fdbk.j3 = str(round(feedback.j3, 2))
    fdbk.j4 = str(round(feedback.j4, 2))

    fdbk.m1 = str(round(feedback.m1, 2))
    fdbk.m2 = str(round(feedback.m2, 2))
    fdbk.m3 = str(round(feedback.m3, 2))
    fdbk.m4 = str(round(feedback.m4, 2))
    fdbk.speed_multiplier = str(round(feedback.speed_multiplier, 1))

    fdbk.ctrl_mode = feedback.ctrl_mode
    fdbk.current_joint = feedback.current_joint
    fdbk.speed_multiplier = fdbk.speed_multiplier


# ----------------  Create window  ----------------
#sg.ChangeLookAndFeel('Black')
#sg.SetOptions(element_padding=(0, 0))

layout = [
            [sg.Text(size=(40, 1), font=('Helvetica', 16), justification='center', key='singular_matrix', text_color='Yellow')],
            [sg.Text(size=(40, 1), font=('Helvetica', 16), justification='center', key='ctrl_mode')],
            [sg.Text('Angles : ', size=(40,1), font=('Helvetica', 16), justification='left', key='angle_title')],
            [sg.Text(size=(40, 1), font=('Helvetica', 14), justification='left', key='j1')],
            [sg.Text(size=(40, 1), font=('Helvetica', 14), justification='left', key='j2')],
            [sg.Text(size=(40, 1), font=('Helvetica', 14), justification='left', key='j3')],
            [sg.Text(size=(40, 2), font=('Helvetica', 14), justification='left', key='j4')],
            [sg.Text('Vitesses moteurs :', size=(40,1), font=('Helvetica', 16), justification='left', key='moteur_title')],
            [sg.Text(size=(40, 1), font=('Helvetica', 14), justification='left', key='mult')],
            [sg.Text(size=(40, 1), font=('Helvetica', 14), justification='left', key='m1')],
            [sg.Text(size=(40, 1), font=('Helvetica', 14), justification='left', key='m2')],
            [sg.Text(size=(40, 1), font=('Helvetica', 14), justification='left', key='m3')],
            [sg.Text(size=(40, 2), font=('Helvetica', 14), justification='left', key='m4')],

         ]

window = sg.Window('Rovus Arm HMI', layout)

# ----------------  main loop  ----------------
rospy.init_node('feedback_node')
sub_feedback = rospy.Subscriber('rovus_bras_feedback', feedback, callback=feedback_callback)
#rospy.spin()

while (True):

    # --------- Update window / timeout ---------
    event, values = window.read(timeout=update_time)
    # --------- Display timer in window ---------
    
    if fdbk.singular_matrix == True:
        window['singular_matrix'].update('Singular Matrix --> Jog in joint')
    else:
        window['singular_matrix'].update('')

    if fdbk.ctrl_mode:
        window['ctrl_mode'].update('JOG Mode : Joint J{:d} selectionné'.format(fdbk.current_joint))
    elif not fdbk.ctrl_mode:
        window['ctrl_mode'].update('JOG Mode : Cartésien')
    else:
        window['ctrl_mode'].update('Impossible de faire la connection')

    window['j1'].update('\tJ1 :  {:s}'.format(fdbk.j1.zfill(6)))
    window['j2'].update('\tJ2 :  {:s}'.format(fdbk.j2.zfill(6)))    
    window['j3'].update('\tJ3 :  {:s}'.format(fdbk.j3.zfill(6)))    
    window['j4'].update('\tJ4 :  {:s}'.format(fdbk.j4.zfill(6))) 


    window['mult'].update('Multiplicateur : {:s}'.format(fdbk.speed_multiplier))
    window['m1'].update('\tm1 :  {:s}'.format(fdbk.m1.zfill(4)))
    window['m2'].update('\tm2 :  {:s}'.format(fdbk.m2.zfill(4)))    
    window['m3'].update('\tm3 :  {:s}'.format(fdbk.m3.zfill(4)))    
    window['m4'].update('\tm4 :  {:s}'.format(fdbk.m4.zfill(4))) 

    if event == sg.WIN_CLOSED:
        break