#include "controlsystem.h"

int main(int argc, char *argv[])
{
    wiringPiSetup(); //Raspberry Pi Start
    QApplication a(argc, argv);
    ControlSystem Challenge; //Object  declare
    return a.exec();//Eventloop start and wait until Window closed
}
