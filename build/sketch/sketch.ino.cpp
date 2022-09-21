#include <Arduino.h>
#line 1 "/home/pop-os/SFI/Unidad2/Ejercicio2/sketch.ino"
#line 1 "/home/pop-os/SFI/Unidad2/Ejercicio2/sketch.ino"
String btnState(uint8_t btnState);
#line 8 "/home/pop-os/SFI/Unidad2/Ejercicio2/sketch.ino"
void task();
#line 72 "/home/pop-os/SFI/Unidad2/Ejercicio2/sketch.ino"
void setup();
#line 77 "/home/pop-os/SFI/Unidad2/Ejercicio2/sketch.ino"
void loop();
#line 1 "/home/pop-os/SFI/Unidad2/Ejercicio2/sketch.ino"
String btnState(uint8_t btnState){
  if(btnState == HIGH){
    return "OFF";
  }
  else return "ON";
}

void task()
{
  enum class TaskStates
  {
    INIT,
    WAIT_COMMANDS
  };
  static TaskStates taskState = TaskStates::INIT;
  constexpr uint8_t led = 25;
  constexpr uint8_t button1Pin = 13;
  constexpr uint8_t button2Pin = 12;
  constexpr uint8_t button3Pin = 33;
  constexpr uint8_t button4Pin = 32;

  switch (taskState)
  {
  case TaskStates::INIT:
  {
    Serial.begin(115200);
    pinMode(led, OUTPUT);
    digitalWrite(led, LOW);
    pinMode(button1Pin, INPUT_PULLUP);
    pinMode(button2Pin, INPUT_PULLUP);
    pinMode(button3Pin, INPUT_PULLUP);
    pinMode(button4Pin, INPUT_PULLUP);
    taskState = TaskStates::WAIT_COMMANDS;
    break;
  }
  case TaskStates::WAIT_COMMANDS:
  {
    if (Serial.available() > 0)
    {
      String command = Serial.readStringUntil('\n');
      if (command == "ledON")
      {
        digitalWrite(led, HIGH);
      }
      else if (command == "ledOFF")
      {
        digitalWrite(led, LOW);
      }
      else if (command == "readBUTTONS")
      {

        Serial.print("btn1: ");
        Serial.print(btnState(digitalRead(button1Pin)).c_str());
        Serial.print(" btn2: ");
        Serial.print(btnState(digitalRead(button2Pin)).c_str());
        Serial.print(" btn3: ");
        Serial.print(btnState(digitalRead(button3Pin)).c_str());
        Serial.print(" btn4: ");
        Serial.print(btnState(digitalRead(button4Pin)).c_str());
        Serial.print('\n');
      }
    }
    break;
  }
  default:
  {
    break;
  }
  }
}

void setup()
{
  task();
}

void loop()
{
  task();
}
