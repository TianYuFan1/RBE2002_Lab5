#ifndef SPEED_CONTROLLER
#define SPEED_CONTROLLER

#include <Romi32U4.h>

class SpeedController{
    private:
        const float Kp = 0.5; //Adapt the parameters until your robot moves at the speed you command it to drive
        const float Ki = 0.1; 
        const float Kp_e = 100;
        const float Ki_e = 0.01; 
        float E_left = 0; 
        float E_right = 0;
        int counts = 1450; //number of counts for a 180 degree turn; you will likely have to change this
        float error_distance = 0;
        float error_theta = 0;
        float error_distance_sum = 0;
        float error_theta_sum = 0;

    public:
        struct constrained_acceleration {
            float constrained_velocity_left;
            float constrained_velocity_right;

        };

        void Init(void);
        void Run(float, float); 
        boolean Turn(int,int); //degrees, direction of rotation: 0->left, 1->right
        boolean Straight(int, int); //speed, duration
        boolean Curved(int,int,int); //speed left, speed right, duration
        boolean MoveToPosition(float,float); //target_x, target_y
        void Problem1(void);
        void Stop(void);
};

#endif
