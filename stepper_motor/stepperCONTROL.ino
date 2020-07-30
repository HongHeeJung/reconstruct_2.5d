/*
2020.07.03 Revise 'Connections to A4988' to 'built-in driver version'
2020.07.07 COM is VCC(5V) for SBD stepper motor
2020.07.08 if(enable == HIGH) ON
2020.07.15 Add ROS code for ros serial comm.
2020.07.29 Initialize motor postion by turnning the enable pin - del
2020.07.30 Add publish code for alerting one revolution 
           & set up one revolution for constant velocity
*/

#include <ros.h>
#include <std_msgs/Int16.h>
const int dirPin = 2;  // Direction 회전 방향
const int stepPin = 3; // Step  클럭을 만들어 주면 펄스 수마큼 속도가 변함
const int enPin = 4;   // Enable 1 or 연결x: WORK, 0 or GND: OFF 

const int STEPS_PER_REV = 120; // Motor steps per rotation (1.5 degree per step)
const int stepDelayMicros = 4750;

void CallBack(const std_msgs::Int16& control);

ros::NodeHandle nh;
ros::Subscriber<std_msgs::Int16> sub("/control", &CallBack);
std_msgs::Int16 msg;
ros::Publisher pub("/one_rev", &msg);

void setup() {
  // Setup the pins as Outputs
  pinMode(stepPin,OUTPUT); 
  pinMode(dirPin,OUTPUT);
  pinMode(enPin,OUTPUT);
  digitalWrite(enPin,High);
  // revolve one rotation for linear revolution
  for(int x = 0; x < STEPS_PER_REV; x++) {
    digitalWrite(stepPin,HIGH);
    delayMicroseconds(stepDelayMicros); 
    digitalWrite(stepPin,LOW); 
    delayMicroseconds(stepDelayMicros);
  }

  nh.initNode();
  nh.subscribe(sub);
  nh.advertise(pub);
}

void loop() {
  nh.spinOnce();
  // Spin motor one rotation
  msg.data = 0;
  pub.Publish(&msg);
  for(int x = 0; x < STEPS_PER_REV; x++) {
    digitalWrite(stepPin,HIGH);
    delayMicroseconds(stepDelayMicros); 
    digitalWrite(stepPin,LOW); 
    delayMicroseconds(stepDelayMicros);
  }
  msg.data = 180;
  pub.Publish(&msg);
}

void CallBack(const std_msgs::Int16& control)
{
  // direction of rotation
  if(control.data == 0) {
    digitalWrite(dirPin,HIGH); // Set motor direction clockwise
  } else {
    digitalWrite(dirPin,LOW); // Set motor direction counterclockwise
  }
}
