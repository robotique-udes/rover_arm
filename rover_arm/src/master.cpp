#include "ros/ros.h"
#include "ros/console.h"
#include "rover_arm/vitesse_moteur_msg.h"
#include "rover_arm/angle.h"
#include "rover_arm/feedback.h"
#include "rover_arm/arm_gui_cmd.h"
#include "sensor_msgs/Joy.h"
#include <string>
#include <sstream>
#include "rover_arm/diffKinematicsCalc.h"

// _______________________________________________________________

// Fonctions prototypes
void init();
void loop();
void standbyLoop();
void runningLoop();
void holdLoop();
void calibrationLoop();
void angleMsgCallback(const rover_arm::angle::ConstPtr &data);
void joyMsgCallback(const sensor_msgs::Joy::ConstPtr &data);
void guiCmdCallback(const rover_arm::arm_gui_cmd::ConstPtr &data);
void calculateSpeedJoint();
void calculateSpeedCartesian();
void assembleAndSendArduinoMsg();
void assembleAndSendFeedbackMsg();
void speedLimiter(boost::array<double, 4UL> vitesse);
void bindKeybindings(ros::NodeHandle *n);

class Moteur
{
private:
    float angle = 0.0;
    float period = 0.0;
    bool enable = true;
    bool dir = 0;

public:
    void setAngle(float iAngle) { angle = iAngle; }
    float getAngle() { return angle; }

    void setPeriod(float iPeriod) { period = iPeriod; }
    float getPeriod() { return period; }

    void setEnable(bool iEnable) { enable = iEnable; }
    float getEnable() { return enable; }

    float getDir() { return (period > 0) ? 1 : 0; }
};
class Controller
{
public:
    bool a = 0;
    bool b = 0;
    bool x = 0;
    bool y = 0;
    bool lt = 0;
    bool rt = 0;

    float vx = 0.0;
    float vy = 0.0;
    float vz = 0.0;
    float va = 0.0;

    bool joint_mode_toggle = 0;
    int joint_change = 0;
    int joint_current = 1;
    bool calibration_button = 0;

    float speed_multiplier = 0.01;
    int speed_increase = 0;
};
struct Keybind
{
    // Buttons
    int a;
    int b;
    int x;
    int y;
    int rt;
    int lt;

    // axes
    int left_joystick_up_down;
    int left_joystick_left_right;
    int right_joystick_up_down;
    int right_joystick_left_right;
    int arrow_up_down;
    int arrow_left_right;
} keybind;

// Enum
enum states
{
    standby,
    running,
    hold,
    calibration
};
enum jogModes
{
    joint,
    cartesian
};

//_______________________________________________________________

// Constantes
#define RATE_FEEDBACK 10 // Hz
#define RATE_ROS 50      // Hz

const int NOMBRE_MOTEUR = 4;
const float BASE_SPEED = 10;
const jogModes DEFAULT_JOG_MODE = joint;
const float SPEED_INCREMENT = 0.1;
const float VITESSE_MAX = 20.0; // en deg/s
const bool ENABLE = 1;
const bool DISABLE = 0;

// Variables globales
bool singularMatrix_flag = false;
bool kineticsCalcError = false;

// Objets globaux
ros::Publisher pub_moteur;
ros::Publisher pub_feedback;
ros::Subscriber sub_angle;
ros::Subscriber sub_input;
ros::Subscriber sub_arm_gui_cmd;
ros::ServiceClient client_diff_kinetics_calc;

Moteur moteurs[NOMBRE_MOTEUR];
Controller input;

// Enum
jogModes jogMode = DEFAULT_JOG_MODE;
states currentState = running;

//_______________________________________________________________
int main(int argc, char *argv[])
{
    ros::init(argc, argv, "master");

    init();
    loop();
}

void init()
{
    ros::NodeHandle n;
    pub_moteur = n.advertise<rover_arm::vitesse_moteur_msg>("vitesses_moteur", 0);
    pub_feedback = n.advertise<rover_arm::feedback>("rover_arm_feedback", 0);
    sub_angle = n.subscribe("valeurAngles", 5, angleMsgCallback);
    sub_input = n.subscribe("/arm_joy", 5, joyMsgCallback);
    sub_arm_gui_cmd = n.subscribe("arm_gui_cmd", 5, guiCmdCallback);
    client_diff_kinetics_calc = n.serviceClient<rover_arm::diffKinematicsCalc>("diff_kinematics_calc");

    bindKeybindings(&n);
}

void loop()
{
    ros::Time lastTime;

    while (ros::ok())
    {
        switch (currentState)
        {
        case standby:
        {
            standbyLoop();
            break;
        }

        case running:
        {
            runningLoop();
            break;
        }

        case calibration:
        {
            calibrationLoop();
            break;
        }
        }
        assembleAndSendArduinoMsg();

        if ((ros::Time::now() - ros::Duration(1.0 / RATE_FEEDBACK)) > lastTime)
        {
            assembleAndSendFeedbackMsg();
            lastTime = ros::Time::now();
        }

        ros::Rate loop_rate(RATE_ROS);
        ros::spinOnce();
        loop_rate.sleep();
    }
}

void standbyLoop()
{
    // TODO
    // Go to StanbyPosition

    for (int i = 0; i < NOMBRE_MOTEUR; i++)
    {
        moteurs[i].setEnable(DISABLE);
    }
}

void runningLoop()
{
    switch (jogMode)
    {
    case joint:
    {
        calculateSpeedJoint();
        break;
    }
    case cartesian:
    {
        calculateSpeedCartesian();
        break;
    }
    }
}

void calibrationLoop() {}

void angleMsgCallback(const rover_arm::angle::ConstPtr &data)
{
    for (int i = 0; i < sizeof(moteurs) / sizeof(Moteur); i++)
    {
        moteurs[i].setAngle(data->angle[i]);
    }
}

void joyMsgCallback(const sensor_msgs::Joy::ConstPtr &data)
{
    /*Params :
    buttons:
    a
    b
    x
    y
    lt
    rt

    axes:
    vx
    vy
    vz
    arrow right+
    arrow left-
    arrow up
    arrow left
    */

    input.a = data->buttons[keybind.a];
    input.x = data->buttons[keybind.x];
    input.y = data->buttons[keybind.y];
    input.b = data->buttons[keybind.b];
    input.lt = data->buttons[keybind.lt];
    input.rt = data->buttons[keybind.rt];

    input.vx = -data->axes[keybind.left_joystick_up_down];
    input.vy = data->axes[keybind.right_joystick_up_down];
    input.vz = data->axes[keybind.left_joystick_left_right];

    input.vx = -input.vx * input.speed_multiplier;
    input.vy = input.vy * input.speed_multiplier;
    input.vz = -input.vz * input.speed_multiplier;
    input.va = (input.lt - input.rt) * input.speed_multiplier;

    input.joint_change = -data->axes[keybind.arrow_left_right];
    input.speed_increase = data->axes[keybind.arrow_up_down];

    //--------------------------------------------------
    // Change le joint actif pour le mode Joint
    if (input.joint_change > 0)
    {
        input.joint_current += 1;
        input.joint_current = input.joint_current <= NOMBRE_MOTEUR ? input.joint_current : 1;
    }

    if (input.joint_change < 0)
    {
        input.joint_current -= 1;
        input.joint_current = input.joint_current > 0 ? input.joint_current : 4;
    }

    //--------------------------------------------------
    // if (input.joint_mode_toggle && jogMode == joint)
    //     jogMode = cartesian;
    // else if (input.joint_mode_toggle && jogMode == cartesian)
    //     jogMode = joint;

    //--------------------------------------------------
    if (input.speed_increase > 0)
        input.speed_multiplier += SPEED_INCREMENT;
    if (input.speed_increase < 0)
        input.speed_multiplier -= SPEED_INCREMENT;
}

void guiCmdCallback(const rover_arm::arm_gui_cmd::ConstPtr &data)
{
    currentState = data->state == 0 ? standby : currentState;
    currentState = data->state == 1 ? running : currentState;
    currentState = data->state == 2 ? calibration : currentState;

    for (int i = 0; i < sizeof(moteurs) / sizeof(Moteur); i++)
    {
        moteurs[i].setEnable(data->enable[i]);
    }

    if (data->jog_is_cartesian)
        jogMode = cartesian;
    else
        jogMode = joint;
}

void calculateSpeedJoint()
{
    for (int i = 0; i < sizeof(moteurs) / sizeof(Moteur); i++)
        moteurs[i].setPeriod(0);

    if (input.rt)
    {
        float speed = BASE_SPEED * input.speed_multiplier;
        moteurs[input.joint_current - 1].setPeriod(speed);
    }
    if (input.lt)
    {
        float speed = -BASE_SPEED * input.speed_multiplier;
        moteurs[input.joint_current - 1].setPeriod(speed);
    }
}

void assembleAndSendArduinoMsg()
{
    rover_arm::vitesse_moteur_msg msg;

    for (int i = 0; i < sizeof(moteurs) / sizeof(Moteur); i++)
    {
        msg.En[i] = moteurs[i].getEnable();

        if (moteurs[i].getEnable())
        {
            msg.Period[i] = (long)(1000000 / moteurs[i].getPeriod());
            msg.Dir[i] = moteurs[i].getDir();
        }
    }

    pub_moteur.publish(msg);
}

void assembleAndSendFeedbackMsg()
{
    rover_arm::feedback msg;

    msg.limiteur = 0;

    for (int i = 0; i < sizeof(moteurs) / sizeof(Moteur); i++)
    {
        msg.angles[i] = moteurs[i].getAngle();
        msg.vitesses[i] = moteurs[i].getPeriod();
        msg.enable[i] = moteurs[i].getEnable();
        if (moteurs[i].getPeriod() == VITESSE_MAX)
            msg.limiteur = 1;
    }

    msg.ctrl_mode = (jogMode == joint ? 1 : 0);
    msg.current_joint = input.joint_current;
    msg.speed_multiplier = input.speed_multiplier;
    msg.calibration = 0;
    msg.singular_matrix = singularMatrix_flag;
    msg.kinetics_calc_error = kineticsCalcError;

    pub_feedback.publish(msg);
}

void calculateSpeedCartesian()
{
    rover_arm::diffKinematicsCalc srv;
    bool limiting_flag = 0;

    if (input.vx != 0.0 || input.vy != 0.0 || input.vz != 0.0 || input.va != 0.0)
    {
        for (int i = 0; i < NOMBRE_MOTEUR; i++)
            srv.request.angles[i] = moteurs[i].getAngle();

        srv.request.cmd[0] = input.vx;
        srv.request.cmd[1] = input.vy;
        srv.request.cmd[2] = input.vz;
        srv.request.cmd[3] = input.va;

        if (client_diff_kinetics_calc.call(srv))
        {
            singularMatrix_flag = srv.response.singularMatrix;
            for (int i = 0; i < NOMBRE_MOTEUR; i++)
            {
                if (std::abs(srv.response.vitesses[i] > 20))
                    limiting_flag = 1;
            }

            if (limiting_flag)
                speedLimiter(srv.response.vitesses);
            else
                for (int i = 0; i < NOMBRE_MOTEUR; i++)
                    moteurs[i].setPeriod(srv.response.vitesses[i]);

            kineticsCalcError = 0;
        }
        else
            kineticsCalcError = 1;
    }
    else
        for (int i = 0; i < NOMBRE_MOTEUR; i++)
            moteurs[i].setPeriod(0.0);
}

void speedLimiter(boost::array<double, 4UL> vitesse)
{
    float vitesseLaPlusGrande = std::abs(vitesse[0]);

    for (int i = 1; i < NOMBRE_MOTEUR; i++)
    {
        if (std::abs(vitesse[i]) > vitesseLaPlusGrande)
            vitesseLaPlusGrande = std::abs(vitesse[i]);
    }

    float facteurLimitant = VITESSE_MAX / vitesseLaPlusGrande;

    for (int i = 0; i < NOMBRE_MOTEUR; i++)
        moteurs[i].setPeriod(vitesse[i] * facteurLimitant);
}

void bindKeybindings(ros::NodeHandle *n)
{
    n->param("a", keybind.a, 0);
    n->param("b", keybind.b, 1);
    n->param("x", keybind.x, 2);
    n->param("y", keybind.y, 3);
    n->param("rt", keybind.rt, 5);
    n->param("lt", keybind.lt, 4);

    n->param("left_joystick_up_down", keybind.left_joystick_up_down, 1);
    n->param("left_joystick_left_right", keybind.right_joystick_left_right, 0);
    n->param("right_joystick_up_down", keybind.right_joystick_up_down, 4);
    n->param("right_joystick_left_right", keybind.right_joystick_left_right, 3);
    n->param("arrow_up_down", keybind.arrow_up_down, 7);
    n->param("arrow_left_right", keybind.arrow_left_right, 6);
}
