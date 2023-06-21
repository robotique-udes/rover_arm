#Angles
float32[6] angles

#Vitesse
bool singular_matrix
float32[6] vitesses

#Enable
bool[6] enable

#Controller
bool ctrl_mode #1 = joint mode | 0 = cartesian
int8 current_joint
float32 speed_multiplier
bool limiteur
bool calibration
bool kinetics_calc_error
