#include <Romi32U4.h>
#include "Behaviors.h"
#include "Speed_controller.h"

//sensors
Romi32U4ButtonA buttonA;

//motor-speed controller
SpeedController robot;

void Behaviors::Init(void)
{
    robot.Init();
}

void Behaviors::Stop(void)
{
    robot.Stop();
}

void Behaviors::Run(void)
{
    switch (robot_state)
    {
    case IDLE:
        if(buttonA.getSingleDebouncedRelease()){ 
            robot_state = DRIVE; 
            robot.Stop();             
        } 
        else { 
            robot_state = IDLE;
            robot.Stop(); 
        }   
        break;
    
    case DRIVE:
        if(buttonA.getSingleDebouncedRelease()){ 
            robot_state = IDLE; 
            robot.Stop();             
        } 
        else {
            Serial.println("start");
            robot.MoveToPosition(50,50);
            robot.MoveToPosition(1.2, 0.7);
            robot.MoveToPosition(2.0, 1.2);
            robot.MoveToPosition(0.1, 3);
            robot.MoveToPosition(3, 1.8);
            Serial.println("end");
            robot_state = IDLE;
        }
        break;
    };
}