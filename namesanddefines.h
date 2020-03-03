//!
//! \file abstractsensor.h
//! \auther PAN Yunjie, YU Bohang, Team 03
//! \date 30-Dec-2018
//! \brief to define a header datei for all defined parameters
//!
//! this part creat a header, that can represent all defined parameters and names
//!
//! \pre
//! \post

#ifndef NAMESANDDEFINES_H
#define NAMESANDDEFINES_H

#define DEFAULTABTASTFREQUENZ 1.0 //default interval of Qtimer,ms
#define ABTASTFREQUENZCOLORSENSOR 1.0 // set samplingrate of colorsensor,ms
#define TICS 1200.0//one circle, 1200 steps
#define GESAMTDREHUNGSGRAD 360.0//grad,one circle 360 grad
#define ABTASTFREQUENZENCODER 1.0//samplingrate of encoder,ms
#define ABTASTFREQUENZLINESENSOR 1.0 //samplingrate of linesensor,ms
#define ABTASTFREQUENZENCODERSPEED 1.0 //samplingrate of PIDControl of speed,ms
#define ABTASTFREQUENZULTRASCHALLSENSOR 100.0//samplingrate of ultrasonicsensor,ms
#define ABSTANDZWISHENSENSORBODEN 11.5//the distance between ultrasonicsensor and base
#define ECHOBACKPARAMETER 58.0// conversion factor from pulse-echo duration in microseconds to centimeters
#define DEFAULTCONTROLFREQUENZ 20 //ms
#define LEDBLINKFREQUENZ 800 //ms

#define MotorPin_AL  23//pinA of left motor
#define MotorPin_BL  24//pinB of left motor
#define MotorPin_EL  25//pinE of left motor
#define MotorPin_AR  27//pinA of right motor
#define MotorPin_BR  28//pinB of right motor
#define MotorPin_ER  29//pinE of right motor

#define LinePin_R  4//pin of right line sensor
#define LinePin_L  5//pin of left line sensor
#define EncoderPin_AL 16//pinA of left encoder
#define EncoderPin_BL 1//pinB of left encoder
#define EncoderPin_AR 10//pinA of right encoder
#define EncoderPin_BR 6//pinB of right encoder
#define UltraschallPin 15//pin of ultraschall sensor

#define LEDRED_PIN 21
#define LEDYELLOW_PIN 14
#define LEDGREEN_PIN 22

#define LDERED LEDRED_PIN
#define LEDYELLOW LEDYELLOW_PIN
#define LEDGREEN LEDGREEN_PIN

#define LineSensorL_ID 1//sensor ID of left line sensor
#define LineSensorR_ID 2//sensor ID of right line sensor
#define ColorSensor_ID 3//sensor ID of color sensor
#define EncoderL_ID 4//sensor ID of left encoder
#define EncoderR_ID 5//sensor ID of right encoder
#define UltraschallSensor_ID 6//sensor ID of ultraschall sensor

#define RADIUSWHEEL 3.17//cm,radius of wheels
#define L_WHEEL 10.6//cm,distance between two wheels
#define PI 3.14159265//math constant PI
#define DELTATS 50// unit is s^(-1),=(1000000/ABTASTZEITSPEED)
#define ABTASTZEITSPEED 20000.0//microseconds

#define KPSL 0.15 //parameter of PID Control for speed ,Kp for left motor
#define KISL 0.4  //parameter of PID Control for speed ,Ki for left motor
#define KDSL 0.1  //parameter of PID Control for speed ,Kd for left motor

#define KPSR 0.2 //parameter of PID Control for speed ,Kp for right motor
#define KISR 0.4  //parameter of PID Control for speed ,Ki for right motor
#define KDSR 0.1  //parameter of PID Control for speed ,Kd for right motor

#define KLL 0.45 //parameter of linesensor Control for speed , for left motor
#define KLR 0.45 //parameter of liensensor Control for speed , for right motor


//parameters of map,all distances datas
#define DISTANCE_START_STARTFRONT 35.0//cm
#define DISTANCE_STARTFRONT_LEFTBOTTOM 73//cm
#define DISTANCE_STARTFRONT_RIGHTBOTTOM 68//cm
#define DISTANCE_LEFTBOTTOM_CLERKALEFT 48.5//cm
#define DISTANCE_RIGHTBOTTOM_CLERKBRIGHT 48.5//cm
#define DISTANCE_CLERKALEFT_LEFTTOP 44.5//cm
#define DISTANCE_CLERKBRIGHT_RIGHTTOP 44.5//cm
#define DISTANCE_LEFTTOP_YELLOW 37.0//cm
#define DISTANCE_YELLOW_RED 32.5//cm
#define DISTANCE_RED_GREEN 36.0//cm
#define DISTANCE_GREEN_RIGHTTOP 35.5//cm
#define DISTANCE_CLERKB_CLERKBRIGHT 21.5//cm
#define DISTANCE_CLERKA_CLERKALEFT 22.0//cm
#define DISTANCE_RIGHTTOP_CLERKBRIGHT 45.0//cm
#define DISTANCE_RIGHTTOP_GREEN 35.5//cm
#define DISTANCE_CLERKB_RIGHTTOP 45.0//cm
#define DISTANCE_COLOR_ENDE 65.0//cm

#endif // NAMESANDDEFINES_H
