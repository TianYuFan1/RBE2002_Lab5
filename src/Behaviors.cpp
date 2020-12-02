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
            // robot.Straight(200, 5);
            Serial.println("start");
            robot.MoveToPosition(0.3,0.1);
            robot.MoveToPosition(0.3, 0.3);
            robot.MoveToPosition(0.5, 0.5);
            robot.MoveToPosition(0.2, 0.4);
            robot.MoveToPosition(0.0, 0.0);
            Serial.println("end");
            robot_state = IDLE;
        }
        break;
    };
}