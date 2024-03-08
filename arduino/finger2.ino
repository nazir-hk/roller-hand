/*
MKS ESP32 FOC 闭环位置控制例程 测试库：SimpleFOC 2.1.1 测试硬件：MKS ESP32 FOC V1.0
在串口窗口中输入：T+位置，就可以使得两个电机闭环转动
比如让两个电机都转动180°，则输入其弧度制：T3.14
在使用自己的电机时，请一定记得修改默认极对数，即 BLDCMotor(7) 中的值，设置为自己的极对数数字
程序默认设置的供电电压为 12V,用其他电压供电请记得修改 voltage_power_supply , voltage_limit 变量中的值
默认PID针对的电机是 2804云台电机 ，使用自己的电机需要修改PID参数，才能实现更好效果
*/

#include <SimpleFOC.h>


MagneticSensorI2C sensor = MagneticSensorI2C(AS5600_I2C);
TwoWire I2Cone = TwoWire(0);

//电机参数
BLDCMotor motor = BLDCMotor(14);                               //根据选用电机的极对数，修改此处BLDCMotor()的值
BLDCDriver3PWM driver = BLDCDriver3PWM(32,33,25,22);


//命令设置
//在串口窗口中输入：T+位置，就可以使得两个电机闭环转动
//比如让两个电机都转动180°，则输入其弧度制：T3.14
float target_position = 0.0;
Commander command = Commander(Serial);
void doTarget(char* cmd) { command.scalar(&target_position, cmd); }

void setup() {
  I2Cone.begin(19,18, 400000UL); 
  sensor.init(&I2Cone);
  //连接motor对象与传感器对象
  motor.linkSensor(&sensor);

  //供电电压设置 [V]
  driver.voltage_power_supply = 24;                   //根据供电电压，修改此处voltage_power_supply的值
  driver.init();

  //连接电机和driver对象
  motor.linkDriver(&driver);
  
  //FOC模型选择
  motor.foc_modulation = FOCModulationType::SpaceVectorPWM;
  //运动控制模式设置
  motor.controller = MotionControlType::angle;

  //速度PI环设置                                     
  motor.PID_velocity.P = 0.1;             //根据选用的电机，修改此处的PID参数，以实现更好的效果
  motor.PID_velocity.I = 0.5;

  //角度P环设置 
  // motor.P_angle.P = 20; //low stiffness
  motor.P_angle.P = 250; //high stiffness

  //最大电机限制电机
  motor.voltage_limit = 24;                //根据供电电压，修改此处voltage_limit的值

  //速度低通滤波时间常数
  motor.LPF_velocity.Tf = 0.01;

  //设置最大速度限制
  motor.velocity_limit = 40;

  Serial.begin(115200);
  motor.useMonitoring(Serial);

  
  //初始化电机
  motor.init();
  //初始化 FOC
  motor.initFOC();
  command.add('T', doTarget, "target position");

  // Serial.println(F("Motor ready."));
  // Serial.println(F("Set the target position using serial terminal:"));
  
}



void loop() {
  sensor.update(); // 如果simplefoc库版本为2.20及以上，取消这两行的注释
  // Serial.print(sensor.getAngle()); 
  // Serial.println();
  motor.loopFOC();

  motor.move(target_position);

  
  command.run();
}
