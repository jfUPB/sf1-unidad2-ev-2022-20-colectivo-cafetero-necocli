#include <Arduino.h>
#include "task2.h"

String btnState(uint8_t btnState)
{
  if(btnState == HIGH){
    return "OFF";
  }
  else return "ON";
}

void task2()
{

enum TaskStates
{
    INIT,
    CALL,
};

static TaskStates taskState = TaskStates::INIT;
constexpr uint8_t buttonTlc1 = 13;
constexpr uint8_t buttonTlc2 = 12;
constexpr uint8_t buttonTlc3 = 27;
constexpr u_int8_t led_r = 26;
constexpr u_int8_t led_g = 25;
constexpr u_int8_t led_y = 33;

    switch (taskState)
    {
        case TaskStates::INIT:
        {
            Serial.begin(115200);
            pinMode(buttonTlc1, INPUT_PULLUP);
            pinMode(buttonTlc2, INPUT_PULLUP);
            pinMode(buttonTlc3, INPUT_PULLUP);
            pinMode(led_r, OUTPUT);
            digitalWrite(led_r, LOW);
            pinMode(led_g, OUTPUT);
            digitalWrite(led_g, LOW);
            pinMode(led_y, OUTPUT);
            digitalWrite(led_y, LOW);
            taskState = TaskStates::CALL;
        }
        break;

        case TaskStates::CALL:
        {
            if (Serial.available() > 0)
            {
                String command = Serial.readStringUntil('\n');
                if (command == "C1"){
                    Serial.print("btn_1: ");
                    Serial.print(btnState(digitalRead(buttonTlc1)).c_str());
                    Serial.print('\n');     
                }
                else if (command == "C2"){
                    Serial.print("btn_2: ");
                    Serial.print(btnState(digitalRead(buttonTlc2)).c_str());
                    Serial.print('\n');    
                }
                else  if (command == "C3"){
                    Serial.print("btn_3: ");
                    Serial.print(btnState(digitalRead(buttonTlc3)).c_str());
                    Serial.print('\n');               
                }
                if (command == "1ON"){
                    digitalWrite(led_r, HIGH);
                }
                if (command == "1OFF"){
                    digitalWrite(led_r, LOW);
                }
                if (command == "2ON"){
                    digitalWrite(led_g, HIGH);
                }
                if (command == "2OFF"){
                    digitalWrite(led_g, LOW);
                }
                if (command == "3ON"){
                    digitalWrite(led_y, HIGH);
                }
                if (command == "3OFF"){
                    digitalWrite(led_y, LOW);
                }  
            }
        }
        break;

        default:
        break;
    }
}

