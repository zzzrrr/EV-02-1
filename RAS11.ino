// RAS11.ino
//
// A3 ----- 出氣量(舵機角度)
// A2 ----- 兩次出氣動作的時間間隔(=呼吸頻率)
// A1 ----- 出氣速度(出氣快慢)
// D5 ----- 舵機腳位

#include <Arduino.h>
#include <Wire.h>
#include <SoftwareSerial.h>

#include <Servo.h>
#include <LiquidCrystal_I2C.h>

double angle_rad = PI/180.0;
double angle_deg = 180.0/PI;
double BPM;
double servar;
LiquidCrystal_I2C lcd_I2C_0x27(0x27, 16, 2);
double currentTime = 0;
double lastTime = 0;
Servo servo_5;
double getLastTime(){
      return currentTime = millis()/1000.0 - lastTime;
}



void setup(){
    lcd_I2C_0x27.init();
    lcd_I2C_0x27.backlight();
    lcd_I2C_0x27.setBacklight( 1 );
    lcd_I2C_0x27.setCursor( (1) - 1, (1) - 1 );
    lcd_I2C_0x27.print( "Taiwan can Help!" );
    lcd_I2C_0x27.setCursor( (2) - 1, (2) - 1 );
    lcd_I2C_0x27.print( "EV-02 by Bio" );
    _delay(3);
    
    pinMode(A3,INPUT);
    pinMode(A2,INPUT);
    pinMode(A1,INPUT);
    servo_5.attach(5); // init pin
}

void loop(){
    
    lastTime = millis()/1000.0;
    lcd_I2C_0x27.clear();
    lcd_I2C_0x27.setCursor( (1) - 1, (1) - 1 );
    lcd_I2C_0x27.print( String("V:")+String((7) * (round(((analogRead(A3)) * (10)) / (1000)))) );//出氣量最大7*10=70度，此修改須與下方參數一樣
    lcd_I2C_0x27.setCursor( (6) - 1, (1) - 1 );
    lcd_I2C_0x27.print( String(" ET:")+String((1.5) + ((0.2) * (round(((analogRead(A2)) * (20)) / (1000))))) );//呼吸間隔最大0.2*20=10秒，此修改須與下方參數一樣
    lcd_I2C_0x27.setCursor( (1) - 1, (2) - 1 );
    lcd_I2C_0x27.print( String("SP:")+String((0) + ((0.21) * (round(((analogRead(A1)) * (10)) / (1000))))) );//出氣速度最長0.21*10=2.1秒，此修改須與下方參數一樣
    lcd_I2C_0x27.setCursor( (9) - 1, (2) - 1 );
    lcd_I2C_0x27.print( String("BPM:")+round((60) / (BPM)) );//引用計時器統計單次循環，除60秒而得
    servo_5.write(110); // write to servo
    _delay((0.8) + ((0.2) * (round(((analogRead(A2)) * (20)) / (1000)))));//呼吸間隔最大0.2*20=10秒，此修改須與上方參數一樣
    servar = 110; //歸位角度，越大越往外
    while(!((servar) < ((110) - ((7) * (round(((analogRead(A3)) * (10)) / (1000))))))) //出氣量最大7*10=70度，此修改須與上方參數一樣
    {
        _loop();
        servo_5.write(servar); // write to servo
        _delay((0) + ((0.003) * (round(((analogRead(A1)) * (10)) / (1000)))));//出氣速度最長0.003*10*7=2.1秒，只有0.003能改，並乘7與上方參數相同
        servar += -1;
    }
    _delay(1); //壓球動作維持1秒
    servo_5.write(110); //歸位角度，越大越往外
    BPM = getLastTime();
    
    _loop();
}

void _delay(float seconds){
    long endTime = millis() + seconds * 1000;
    while(millis() < endTime)_loop();
}

void _loop(){
    
}
