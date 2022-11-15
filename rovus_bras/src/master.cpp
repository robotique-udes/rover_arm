#include "ros/ros.h"
#include "ros/console.h"
#include "rovus_bras/vitesse_moteur_msg.h"
#include "rovus_bras/angle.h"
#include "rovus_bras/feedback.h"
#include "rovus_bras/arm_gui_cmd.h"
#include "sensor_msgs/Joy.h"
#include <string> 
#include <sstream>
#include "rovus_bras/diffKinematicsCalc.h"

// _______________________________________________________________
// Declarations and prototypes
// Fonctions prototypes
void init();
void loop();
void standbyLoop();
void runningLoop();
void holdLoop();
void calibrationLoop();
void angleMsgCallback(const rovus_bras::angle::ConstPtr& data);
void joyMsgCallback(const sensor_msgs::Joy::ConstPtr& data);
void guiCmdCallback(const rovus_bras::arm_gui_cmd::ConstPtr& data);
void calculateSpeedJoint();
void calculateSpeedCartesian();
void assembleAndSendArduinoMsg();
void assembleAndSendFeedbackMsg();


// Classes
class Moteur
{
    private:
        float angle = 0.0;
        float period = 0.0;
        bool enable = true;
        bool dir = 0;

    public:
        void setAngle(float iAngle) {angle = iAngle;}
        float getAngle() {return angle;}

        void setPeriod(float iPeriod) {period = iPeriod;}
        float getPeriod() {return period;}

        void setEnable(bool iEnable) {enable = iEnable;}
        float getEnable() {return enable;}

        float getDir() {return period>0? 1: 0;}
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

// Enum
enum states{standby, running, hold, calibration};
enum jogModes{joint, cartesian};

//_______________________________________________________________
//Cosntantes/Variables/etc.
//Constantes
const int NOMBRE_MOTEUR = 4;
const float BASE_SPEED = 10;
const jogModes DEFAULT_JOG_MODE = joint;
const float SPEED_INCREMENT= 0.1;

//Variables globales
bool singularMatrix_flag = false;
bool kineticsCalcError = false;

//Objets globaux
ros::Publisher pub_moteur;
ros::Publisher pub_feedback;
ros::Subscriber sub_angle;
ros::Subscriber sub_input;
ros::Subscriber sub_arm_gui_cmd;
ros::ServiceClient client_diff_kinetics_calc;

Moteur moteurs[NOMBRE_MOTEUR];
Controller input;

//Enum
jogModes jogMode=DEFAULT_JOG_MODE;
states currentState = running; 

//_______________________________________________________________
int main(int argc, char* argv[])
{

    ros::init(argc, argv, "master");

    init();
    loop();
}

void init()
{
    ros::NodeHandle n;
    pub_moteur = n.advertise<rovus_bras::vitesse_moteur_msg>("vitesses_moteur", 0);
    pub_feedback = n.advertise<rovus_bras::feedback>("rovus_bras_feedback", 0);
    sub_angle = n.subscribe("valeurAngles", 5, angleMsgCallback);
    sub_input = n.subscribe("joy", 5, joyMsgCallback);
    sub_arm_gui_cmd = n.subscribe("arm_gui_cmd", 5, guiCmdCallback);
    client_diff_kinetics_calc = n.serviceClient<rovus_bras::diffKinematicsCalc>("diff_kinematics_calc");
}

void loop()
{
    while(ros::ok())
    {
        switch(currentState)
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

            case hold:
            {
                holdLoop();
                break;
            }

            case calibration:
            {
                calibrationLoop();
                break;
            }
        }
        
        ros::Rate loop_rate(50);
        ros::spinOnce();
        loop_rate.sleep();    
    }
}

void standbyLoop(){}

void runningLoop()
{
    switch(jogMode)
    {
        case joint:
        {
            calculateSpeedJoint();
            break;
        }
        case cartesian:
        {
            calculateSpeedCartesian();

            if (singularMatrix_flag)
            {
                for(int i = 0; i < NOMBRE_MOTEUR; i++)
                    moteurs[i].setPeriod(0);
            }
        }
    }
    assembleAndSendArduinoMsg();
    assembleAndSendFeedbackMsg();
}

void holdLoop(){}

void calibrationLoop(){}

void angleMsgCallback(const rovus_bras::angle::ConstPtr &data)
{
    for( int i=0; i<sizeof(moteurs)/sizeof(Moteur); i++)
    {
        moteurs[i].setAngle(data->angle[i]);
    }
}

void joyMsgCallback(const sensor_msgs::Joy::ConstPtr &data)
{
    input.a = data->buttons[0];
    input.x = data->buttons[1];
    input.y = data->buttons[2];
    input.b = data->buttons[3];
    input.lt = data->buttons[4];
    input.rt = data->buttons[5];

    input.vx = -data->axes[1];
    input.vy = data->axes[4];
    input.vz = data->axes[0];

    input.calibration_button = data->buttons[12];

    input.joint_mode_toggle = data->buttons[8];

    input.joint_change = - data->axes[6];
    input.speed_increase = data->axes[7];

    //--------------------------------------------------
    //Change le joint actif pour le mode Joint
    if (input.joint_change > 0)
    {
        input.joint_current += 1;
        input.joint_current = input.joint_current <= NOMBRE_MOTEUR? input.joint_current : 1;
    }

    if (input.joint_change < 0)
    {
        input.joint_current -= 1;
        input.joint_current = input.joint_current > 0? input.joint_current : 4;
    }

    //--------------------------------------------------
    if(input.joint_mode_toggle && jogMode == joint)
        jogMode = cartesian;
    else if(input.joint_mode_toggle && jogMode == cartesian)
        jogMode = joint;

    //--------------------------------------------------
    if (input.speed_increase > 0)
        input.speed_multiplier += SPEED_INCREMENT;
    if (input.speed_increase < 0)
        input.speed_multiplier -= SPEED_INCREMENT;

    input.vx = -input.vx*input.speed_multiplier;
    input.vy = input.vy*input.speed_multiplier;
    input.vz = -input.vz*input.speed_multiplier;
    input.va = (input.lt-input.rt)*input.speed_multiplier;

    //ROS_INFO("%d", input.joint_current);
}

void guiCmdCallback(const rovus_bras::arm_gui_cmd::ConstPtr& data) 
{
    for(int i=0; i < sizeof(moteurs)/sizeof(Moteur); i++)
    {
        moteurs[i].setEnable(data->enable[i]);
    }
}

void calculateSpeedJoint()
{
    for(int i=0; i<sizeof(moteurs)/sizeof(Moteur); i++)
            moteurs[i].setPeriod(0);

    if (input.rt)
    {
        float speed = BASE_SPEED * input.speed_multiplier;
        moteurs[input.joint_current-1].setPeriod(speed);
    }
    if (input.lt)
    {
        float speed = -BASE_SPEED * input.speed_multiplier;
        moteurs[input.joint_current-1].setPeriod(speed);
    }

}

void assembleAndSendArduinoMsg()
{
    rovus_bras::vitesse_moteur_msg msg;

    for( int i=0; i<sizeof(moteurs)/sizeof(Moteur); i++)
    {
        msg.En[i] = moteurs[i].getEnable();

        if (moteurs[i].getEnable())
            msg.Period[i] = moteurs[i].getPeriod();
            msg.Dir[i] = moteurs[i].getDir();
    }

    pub_moteur.publish(msg);
}

void assembleAndSendFeedbackMsg()
{
    rovus_bras::feedback msg;
    
    for( int i=0; i<sizeof(moteurs)/sizeof(Moteur); i++)
    {
        msg.angles[i] = moteurs[i].getAngle();
        msg.vitesses[i] = moteurs[i].getPeriod();
        msg.enable[i] = moteurs[i].getEnable();
    }

    msg.ctrl_mode = (jogMode == joint? 1: 0);
    msg.current_joint = input.joint_current;
    msg.speed_multiplier = input.speed_multiplier;
    msg.limiteur = 0;
    msg.calibration = 0;
    msg.singular_matrix = singularMatrix_flag;
    msg.kinetics_calc_error = kineticsCalcError;

    pub_feedback.publish(msg);
}

void calculateSpeedCartesian()
{
    rovus_bras::diffKinematicsCalc srv;

    for(int i = 0; i < NOMBRE_MOTEUR; i++)
        srv.request.angles[i] = moteurs[i].getAngle();

    srv.request.cmd[0]=input.vx;
    srv.request.cmd[1]=input.vy;
    srv.request.cmd[2]=input.vz;
    srv.request.cmd[3]=input.va;

    if (client_diff_kinetics_calc.call(srv))
    {
        singularMatrix_flag = srv.response.singularMatrix;

        for(int i = 0; i < NOMBRE_MOTEUR; i++)
            moteurs[i].setPeriod(srv.response.vitesses[i]);

        kineticsCalcError = 0;

        ROS_INFO("I received : j1:%f, j2:%f, j3: %f, j4:%f", srv.response.vitesses[0], srv.response.vitesses[1], srv.response.vitesses[2], srv.response.vitesses[3]);
    }
    else
        kineticsCalcError = 1;
}



























