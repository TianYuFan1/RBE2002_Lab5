#include <Romi32U4.h>
#include "Encoders.h"
#include  "Speed_controller.h"
#include "Position_estimation.h"

Romi32U4Motors motors;
Encoder MagneticEncoder; 
Position odometry;

float time_track = 0;

void SpeedController::Init(void)
{
    MagneticEncoder.Init();
    odometry.Init();
}

void SpeedController::Run(float target_velocity_left, float target_velocity_right)
{
    if(MagneticEncoder.UpdateEncoderCounts()){
        time_track = time_track + 50/1000.0;
        float e_left = target_velocity_left - MagneticEncoder.ReadVelocityLeft();
        float e_right = target_velocity_right - MagneticEncoder.ReadVelocityRight();
        
        E_left += e_left;
        E_right += e_right;

        float u_left = Kp*e_left + Ki*E_left;
        float u_right = Kp*e_right + Ki*E_right;

        motors.setEfforts(u_left,u_right);
        odometry.UpdatePose(target_velocity_left,target_velocity_right);
    }
}

boolean SpeedController::MoveToPosition(float target_x, float target_y)
{
    do {    
            // PI
        float x = odometry.ReadPose().X;
        float y = odometry.ReadPose().Y;
        float theta = odometry.ReadPose().THETA;

        float error_x = target_x - x;
        float error_y = target_y - y;

        error_distance = sqrt(pow(error_x, 2) + pow(error_y, 2));
        error_theta = atan2(error_y, error_x) - theta;

        if (error_theta > (PI/180) * 185) {error_theta -= 2 * PI;}
        else if (error_theta < -(PI/180) * 185) {error_theta += 2 * PI;}

        error_distance_sum += error_distance;
        error_theta_sum += error_theta;

        float left_speed = Kp_e * error_distance + Ki_e * error_distance_sum - Kp_e * error_theta - Ki_e * error_theta_sum; // TODO Check
        float right_speed = Kp_e * error_distance + Ki_e * error_distance_sum + Kp_e * error_theta + Ki_e * error_theta_sum; 

        left_speed = constrain(left_speed, -75, 75);
        right_speed = constrain(right_speed, -75, 75);

        Run(left_speed, right_speed);
        
        Serial.println(error_distance);

    } while (error_distance > 0.01); //define a distance criteria that lets the robot know that it reached the waypoint.
    return 1;
}

void SpeedController::Problem2(void) {
        //     // PI
        // float x = odometry.ReadPose().X;
        // float y = odometry.ReadPose().Y;
        // float theta = odometry.ReadPose().THETA;

        // float error_x = target_x - x;
        // float error_y = target_y - y;

        // error_distance = sqrt(pow(error_x, 2) + pow(error_y, 2));
        // error_theta = atan2(error_y, error_x) - theta;

        // error_distance_sum += error_distance;
        // error_theta_sum += error_theta;

        // float left_speed = Kp_e * error_distance + Ki_e * error_distance_sum - Kp_e * error_theta - Ki_e * error_theta_sum; // TODO Check
        // float right_speed = Kp_e * error_distance + Ki_e * error_distance_sum + Kp_e * error_theta + Ki_e * error_theta_sum; 

        // left_speed = constrain(left_speed, -75, 75);
        // right_speed = constrain(right_speed, -75, 75);

        // Run(left_speed, right_speed);
        
        // Serial.println(error_distance);
}

void SpeedController::Problem1(void) {
        // float x = odometry.ReadPose().X;
        // float y = odometry.ReadPose().Y;
        // float theta = odometry.ReadPose().THETA;

        // float error_x = target_x - x;
        // float error_y = target_y - y;

        // error_distance = sqrt(pow(error_x, 2) + pow(error_y, 2));
        // error_theta = atan2(error_y, error_x) - theta;

        // float left_speed = Kp_e * error_distance - Kp_e * error_theta; // TODO Check
        // float right_speed = Kp_e * error_distance + Kp_e * error_theta; 

        // left_speed = constrain(left_speed, -75, 75);
        // right_speed = constrain(right_speed, -75, 75);

        // Run(left_speed, right_speed);
        
        // Serial.println(error_distance);
}

boolean SpeedController::Turn(int degree, int direction)
{
    motors.setEfforts(0, 0);
    int turns = counts*(degree/180.0);
    int count_turn = MagneticEncoder.ReadEncoderCountLeft();

    while(abs(abs(count_turn) - abs(MagneticEncoder.ReadEncoderCountLeft())) <= turns)
    {
        if(!direction) Run(50,-50);
        else Run(-50,50);
    }
    motors.setEfforts(0, 0);
    return 1;
}

boolean SpeedController::Straight(int target_velocity, int time)
{
    motors.setEfforts(0, 0);
    unsigned long now = millis();

    float current_velocity = 0;

    while ((unsigned long)(millis() - now) <= time*1000){
        // if ((current_velocity < target_velocity) && ((unsigned long) (millis() - now) <= 2000)) {current_velocity = current_velocity + 0.008;}
        // if (((unsigned long) (millis() - now) >= 4000)) {current_velocity = current_velocity - 0.008;}
        Run(target_velocity,target_velocity);
    }
    motors.setEfforts(0, 0);
    return 1;
}

boolean SpeedController::Curved(int target_velocity_left, int target_velocity_right, int time) //in mm/s and s
{
    motors.setEfforts(0, 0);
    unsigned long now = millis();

    while ((unsigned long)(millis() - now) <= time*1000){
        Run(target_velocity_left,target_velocity_right);
    }
    motors.setEfforts(0, 0);
    return 1;
}

void SpeedController::Stop()
{
    motors.setEfforts(0,0);
    odometry.Stop();
    time_track = 0;
}