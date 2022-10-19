#!/usr/bin/env python

from ast import Global
from unittest.mock import CallableMixin
from numpy import angle, rate
import rospy
import rospkg
from rovus_bras.msg import feedback
from rosgraph_msgs.msg import Log
import PySimpleGUI as sg
from std_srvs.srv import SetBool, SetBoolResponse
from std_msgs.msg import Bool
import rosnode
import roslaunch

# ------------------------------------------------
# Variables globale
update_rate = 10
mem_keybind = 0
mem_liveAllViews = 0
mem_live3DView = 0
mem_toggle_mode = 0
last_arm_mode = None
window2 = None
live3DView_process = None
liveAllViews_process = None
LOGPERIOD = 0.1

# Constantes
COL1_SIZE = 30
COL2_SIZE = 15
COL3_SIZE = 20

# Image data
keybinding_image_path = rospkg.RosPack().get_path("rovus_bras") + ("/img/keybindings.png")
keybind_image = open(keybinding_image_path, "rb").read()

# Class and global object
class display_fdbk:

    singular_matrix = False

    j1 = "0"
    j2 = "0"
    j3 = "0"
    j4 = "0"

    m1 = "0"
    m2 = "0"
    m3 = "0"
    m4 = "0"

    ctrl_mode = 0
    current_joint = 0
    speed_multiplier = "0"
    limiteur = 0
    calibration = False


fdbk = display_fdbk()

# ---------- Fonctions ---------------
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
    fdbk.speed_multiplier = str(round(feedback.speed_multiplier, 2))

    fdbk.ctrl_mode = feedback.ctrl_mode
    fdbk.current_joint = feedback.current_joint
    fdbk.speed_multiplier = fdbk.speed_multiplier
    fdbk.limiteur = feedback.limiteur
    fdbk.calibration = feedback.calibration


def joy_callback(msg: Bool):
    global last_arm_mode
    last_arm_mode = msg.data


def log_callback(msg: Log):
    lastMsgRosSerial = msg.header

    rospy.logwarn(msg.name)


def getLayout():
    column1 = [
        [
            sg.Text(
                size=(COL1_SIZE, 1),
                font=("Helvetica", 16),
                justification="center",
                key="singular_matrix",
                text_color="Yellow",
            )
        ],
        [sg.Text(size=(COL1_SIZE, 1), font=("Helvetica", 16), justification="center", key="ctrl_mode")],
        [sg.Text("Angles : ", size=(COL1_SIZE, 1), font=("Helvetica", 16), justification="left", key="angle_title")],
        [sg.Text(size=(COL1_SIZE, 1), font=("Helvetica", 14), justification="left", key="j1")],
        [sg.Text(size=(COL1_SIZE, 1), font=("Helvetica", 14), justification="left", key="j2")],
        [sg.Text(size=(COL1_SIZE, 1), font=("Helvetica", 14), justification="left", key="j3")],
        [sg.Text(size=(COL1_SIZE, 2), font=("Helvetica", 14), justification="left", key="j4")],
        [
            sg.Text(
                "Vitesses moteurs :", size=(15, 1), font=("Helvetica", 16), justification="left", key="moteur_title"
            ),
            sg.Text(
                size=(COL1_SIZE - 15, 1),
                font=("Helvetica", 16),
                justification="left",
                key="limiting",
                text_color="Yellow",
            ),
        ],
        [sg.Text(size=(COL1_SIZE, 1), font=("Helvetica", 14), justification="left", key="mult")],
        [sg.Text(size=(COL1_SIZE, 1), font=("Helvetica", 14), justification="left", key="m1")],
        [sg.Text(size=(COL1_SIZE, 1), font=("Helvetica", 14), justification="left", key="m2")],
        [sg.Text(size=(COL1_SIZE, 1), font=("Helvetica", 14), justification="left", key="m3")],
        [sg.Text(size=(COL1_SIZE, 3), font=("Helvetica", 14), justification="left", key="m4")],
    ]

    column2 = [
        [
            sg.Button(
                "Switch to Arm mode",
                font=("Helvetica", 16),
                size=(COL2_SIZE, 1),
                button_color="#212121",
                key="rover_toggle",
            )
        ],
        [
            sg.Button(
                "Open 3D View", font=("Helvetica", 16), size=(COL2_SIZE, 1), button_color="#212121", key="live3DView"
            )
        ],
        [
            sg.Button(
                "Open All Views",
                font=("Helvetica", 16),
                size=(COL2_SIZE, 1),
                button_color="#212121",
                key="liveAllViews",
            )
        ],
        [sg.Button("Keybinding", font=("Helvetica", 16), size=(COL2_SIZE, 1), button_color="#212121", key="Keybind")],
    ]

    column3 = [
        [sg.Text(size=(1, 1), font=("Helvetica", 14), justification="center", key="spacer")],
        [LEDIndicator("LED_node_masterPy")],
        [LEDIndicator("LED_node_rosSerial")],
        [LEDIndicator("LED_node_joy")],
        [LEDIndicator("LED_node_live3DView")],
        [LEDIndicator("LED_node_liveAllViews")],
    ]

    column4 = [
        [
            sg.Text(
                "Nodes Status", size=(COL3_SIZE, 1), font=("Helvetica", 14), justification="center", key="Column3Title"
            )
        ],
        [sg.Text("master.py", size=(COL3_SIZE, 1), font=("Helvetica", 14), justification="left", key="node_masterPy")],
        [
            sg.Text(
                "Arduino (ROS Serial)",
                size=(COL3_SIZE, 1),
                font=("Helvetica", 14),
                justification="left",
                key="node_rosSerial",
            )
        ],
        [
            sg.Text(
                "Controller (joy)", size=(COL3_SIZE, 1), font=("Helvetica", 14), justification="left", key="node_joy"
            )
        ],
        [
            sg.Text(
                "Live 3D View", size=(COL3_SIZE, 1), font=("Helvetica", 14), justification="left", key="node_live3DView"
            )
        ],
        [
            sg.Text(
                "Live All Views",
                size=(COL3_SIZE, 1),
                font=("Helvetica", 14),
                justification="left",
                key="node_liveAllViews",
            )
        ],
    ]

    return [
        [
            sg.Column(column1),
            sg.VerticalSeparator(),
            sg.Column(column2),
            sg.VerticalSeparator(),
            sg.Column(column3),
            sg.Column(column4),
        ]
    ]


def update_for_mode(arm_mode: bool, window: sg.Window, display_only: bool = False):
    global mem_toggle_mode
    is_arm = mem_toggle_mode == 0

    if arm_mode and is_arm:
        return

    if not arm_mode and not is_arm:
        return

    if arm_mode:
        window["rover_toggle"].update("Switch to Rover mode")  # type: ignore
        mem_toggle_mode = 0
    else:
        window["rover_toggle"].update("Switch to Arm mode")  # type: ignore
        mem_toggle_mode = 1

    mem_toggle_mode = int(not arm_mode)


def update_for_keybind(window: sg.Window, event):
    global mem_keybind
    global window2

    if event == "Keybind" and mem_keybind == 0:
        mem_keybind = 1

        layout2 = [
            [sg.Text(size=(COL1_SIZE, 1), font=("Helvetica", 16), justification="center", key="empty_line")],
            [sg.Image(data=keybind_image)],
        ]

        window2 = sg.Window("Keybinding", layout2)
        event, values = window2.read(timeout=update_rate)

    elif event == "Keybind" and mem_keybind == 1:
        mem_keybind = 0
        window2.close()


def update_for_liveAllViews(window: sg.Window, event, launch: roslaunch.ROSLaunch):
    global mem_liveAllViews
    global liveAllViews_process

    if event == "liveAllViews" and mem_liveAllViews == 0:
        mem_liveAllViews = 1

        node_live3DView = roslaunch.core.Node("rovus_bras", "liveGraph3DView.py")
        node_liveAllViews = roslaunch.core.Node("rovus_bras", "liveGraphAllViews.py")
        # live3DView_process = launch.launch(node_live3DView)
        liveAllViews_process = launch.launch(node_liveAllViews)
        window["liveAllViews"].update("Close All Views")

    elif event == "liveAllViews" and mem_liveAllViews == 1:
        mem_liveAllViews = 0

        # live3DView_process.stop()
        liveAllViews_process.stop()
        window["liveAllViews"].update("Open All Views")


def update_for_live3DView(window: sg.Window, event, launch: roslaunch.ROSLaunch):
    global mem_live3DView
    global live3DView_process

    if event == "live3DView" and mem_live3DView == 0:
        mem_live3DView = 1

        node_live3DView = roslaunch.core.Node("rovus_bras", "liveGraph3DView.py")
        # node_liveAllViews = roslaunch.core.Node('rovus_bras', 'liveGraphAllViews.py')
        live3DView_process = launch.launch(node_live3DView)
        # liveAllViews_process = launch.launch(node_liveAllViews)
        window["live3DView"].update("Close 3D View")

    elif event == "live3DView" and mem_live3DView == 1:
        mem_live3DView = 0

        live3DView_process.stop()
        # liveAllViews_process.stop()
        window["live3DView"].update("Open 3D View")


def update_for_MainWindows(window: sg.Window):
    if fdbk.calibration == True:
        window["singular_matrix"].update("|-- Calibration en cours --|")
    elif fdbk.singular_matrix == True:
        window["singular_matrix"].update("Singular Matrix --> Jog in joint")
    else:
        window["singular_matrix"].update("")

    if fdbk.ctrl_mode:
        window["ctrl_mode"].update("Mode Joint | J{:d} selectionné".format(fdbk.current_joint))
    elif not fdbk.ctrl_mode:
        window["ctrl_mode"].update("Mode Cartésien")
    else:
        window["ctrl_mode"].update("Impossible de faire la connection")

    if fdbk.limiteur:
        window["limiting"].update(" Limiting")
    else:
        window["limiting"].update("")

    window["j1"].update("\tJ1 :  {:s}".format(fdbk.j1.zfill(6)))
    window["j2"].update("\tJ2 :  {:s}".format(fdbk.j2.zfill(6)))
    window["j3"].update("\tJ3 :  {:s}".format(fdbk.j3.zfill(6)))
    window["j4"].update("\tJ4 :  {:s}".format(fdbk.j4.zfill(6)))

    window["mult"].update("Multiplicateur : {:s}".format(fdbk.speed_multiplier))
    window["m1"].update("\tm1 :  {:s}".format(fdbk.m1.zfill(4)))
    window["m2"].update("\tm2 :  {:s}".format(fdbk.m2.zfill(4)))
    window["m3"].update("\tm3 :  {:s}".format(fdbk.m3.zfill(4)))
    window["m4"].update("\tm4 :  {:s}".format(fdbk.m4.zfill(4)))


def update_for_toggle_mode(window: sg.Window, event):
    global mem_toggle_mode

    if event == "rover_toggle" and mem_toggle_mode == 0:
        # send bool no error
        try:
            rospy.ServiceProxy("/set_arm_joy", SetBool).call(False)
        except rospy.ServiceException as e:
            rospy.logerr("Failed to swith to Rover mode, service unavailable: %s" % e)
        else:
            window["rover_toggle"].update("Switch to Arm mode")
            mem_toggle_mode = 1
        # We are now in Rover mode

    # send bool error
    # erreur

    elif event == "rover_toggle" and mem_toggle_mode == 1:
        # send bool no error
        try:
            rospy.ServiceProxy("/set_arm_joy", SetBool).call(True)
        except rospy.ServiceException as e:
            rospy.logerr("Failed to swith to Arm mode, service unavailable: %s" % e)
        else:
            window["rover_toggle"].update("Switch to Rover mode")
            mem_toggle_mode = 0
        # We are now in Arm mode

    # send bool error
    # erreur


def update_node_status(window: sg.Window):
    activeNodes = rosnode.get_node_names()

    SetLED(window, "LED_node_masterPy", color="red")
    SetLED(window, "LED_node_rosSerial", "red")
    SetLED(window, "LED_node_rosSerial", "red")
    SetLED(window, "LED_node_joy", "red")
    SetLED(window, "LED_node_live3DView", "red")
    SetLED(window, "LED_node_liveAllViews", "red")

    for text in activeNodes:
        if "master" in text:
            SetLED(window, "LED_node_masterPy", color="green")

    for text in activeNodes:
        if "arduino" in text:
            SetLED(window, "LED_node_rosSerial", "green")

    for text in activeNodes:
        if "joy" in text:
            SetLED(window, "LED_node_joy", "green")

    for text in activeNodes:
        if "liveGraph3DView" in text:
            SetLED(window, "LED_node_live3DView", "green")

    for text in activeNodes:
        if "liveGraphAllViews" in text:
            SetLED(window, "LED_node_liveAllViews", "green")

    update_node_error()


def update_node_error():
    penis = 1


def LEDIndicator(key=None, radius=30):
    return sg.Graph(
        canvas_size=(radius, radius),
        graph_bottom_left=(-radius, -radius),
        graph_top_right=(radius, radius),
        pad=(0, 0),
        key=key,
    )


def SetLED(window, key, color):
    graph = window[key]
    graph.erase()
    graph.draw_circle((0, 0), 12, fill_color=color, line_color=color)


def main():
    # ----------------- ROS init -----------------
    rospy.init_node("feedback_node")
    # sub_rosout_agg = rospy.Subscriber('/rosout_agg', Log, callback=rosout_agg_callback)
    sub_feedback = rospy.Subscriber("rovus_bras_feedback", feedback, callback=feedback_callback)
    sub_joy_demux = rospy.Subscriber("joy_state_is_arm", Bool, callback=joy_callback)
    sub_log = rospy.Subscriber("/rosout_agg", Log, callback=log_callback)
    launch = roslaunch.scriptapi.ROSLaunch()
    launch.start()

    # ----------------- PySimpleGUI init -----------------
    sg.ChangeLookAndFeel("DarkGrey")
    window = sg.Window("Rovus Arm HMI", getLayout())

    # ----------------- Bool Service Stuff -----------------
    global last_arm_mode
    try:
        resp: SetBoolResponse = rospy.ServiceProxy("get_arm_joy", SetBool).call()
        last_arm_mode = not resp.success
    except rospy.ServiceException:
        last_arm_mode = rospy.get_param("~start_in_arm_mode", False)

    # ----------------- main Loop -----------------
    while not rospy.is_shutdown():
        # updates
        event, values = window.read(timeout=update_rate)
        update_for_keybind(window, event)
        update_for_liveAllViews(window, event, launch)
        update_for_live3DView(window, event, launch)
        update_for_toggle_mode(window, event)
        update_for_MainWindows(window)
        update_node_status(window)

        if last_arm_mode is not None:
            update_for_mode(last_arm_mode, window)
            last_arm_mode = None

        if event == sg.WIN_CLOSED:
            launch.stop()
            break

    # ----------------- exiting -----------------
    rospy.spin()


if __name__ == "__main__":
    main()
