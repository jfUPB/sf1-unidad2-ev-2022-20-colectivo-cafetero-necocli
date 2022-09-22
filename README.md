# Evaluación Unidad 1: Software para sistemas embebidos

## Equipo: Colectivo Cafetero Necoclí


**Enunciado:**

*Realiza una aplicación interactiva en Unity que te permita controlar 3 LEDs y leer el estado de 3 pulsadores.

La aplicación debe tener la siguiente interfaz de usuario:*


![Diagrama](https://sistemasfisicosinteractivos1.readthedocs.io/es/latest/_images/evalUI.png)

**Criterios de evaluación**

##
# review del código

## Microcontrolador

* Este script inicializa las librerías, el estado de los botones, los eventos, y la función principal.


```

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

```

* Posteriormente, Declaramos las variables de los botones y leds en sus respectivos puertos.

```

 static TaskStates taskState = TaskStates::INIT;
constexpr uint8_t buttonTlc1 = 13;
constexpr uint8_t buttonTlc2 = 12;
constexpr uint8_t buttonTlc3 = 27;
constexpr u_int8_t led_r = 26;
constexpr u_int8_t led_g = 25;
constexpr u_int8_t led_y = 33;

   
```

* A continuación realizamos un *switch* para establecer lo que debe hacer cuando cambia al estado INIT.

```

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

        }
```
* En el anterior, inicializa el puerto serial en un baut rate de 115200, establece los estados de los botones como no presionados y el estado de los leds como apagados

* Luego, en el segundo case definimos el estado CALL y lo que debe hacer el programa al cambiar a dicho momento
```

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

```

* Si los datos enviados al puerto serial son mayores a cero (0), lee la cadena tipo string hasta que haya un salto de línea.
Cuando compara en los botones, hay tres casos posibles:

1. Si el mensaje corresponde al del boton1, revisa el estado actual del botón 
2. Si el mensaje corresponde al del boton2, revisa el estado actual del botón
3. Si el mensaje corresponde al del boton3, revisa el estado actual del botón

Mientras que con los leds, revisa el mensaje enviado desde el pc hasta el puerto serial. Si este mensaje corresponde con encender 
o apagar uno de los tres leds, comunica esto a la protoboard y enciende el led.

## Unity

* Comenzamos inicializando las librerías de Unity, y declarando las variables y los objetos dentro del editor.

´´´

using System;
using UnityEngine;
using System.IO.Ports;
using TMPro;

public class Main : MonoBehaviour
{
    StringComparer stringComparer = StringComparer.OrdinalIgnoreCase;
    private SerialPort _serialPort;
    public TextMeshProUGUI btn1;
    public TextMeshProUGUI btn2;
    public TextMeshProUGUI btn3;
    private string _message;
    private string _index, _ledState;
    
    public GameObject selection;
    public GameObject status;
    
´´´

* Continuamos con un Método Start en el que inicializamos cada una de las variables, declarando el puerto serial,
el puerto al que está conectado, su baut rate, habilitando su lectura, declarando cuando deja de leer una línea, 
abriendo el puerto serial y anuncia en la consola que se ha abierto.

´´´
 void Start()
    {
        _serialPort = new SerialPort();
        _serialPort.PortName = "/dev/ttyUSB0";
        _serialPort.BaudRate = 115200;
        _serialPort.DtrEnable = true;
        _serialPort.NewLine = "\n";
        _serialPort.Open();
        Debug.Log("Open Serial Port");
    }
    
´´´

*Posteriormente, nos encontramos con el método Update, que en él hay un ciclo *if* donde se lee qué botón está siendo
presionado y lo muestra por medio de la interfaz de Unity.

´´´

 void Update()
    {
        if (_serialPort.BytesToRead > 0)
        { 
            string response = _serialPort.ReadLine();
            // 1,0,0 
            // string[] resp = response.Split(',');
            Debug.Log(response);
            
            if (stringComparer.Equals("btn_1: OFF", response)){
                btn1.text = "RELEASED";
            }
            if (stringComparer.Equals("btn_1: ON", response)){
                btn1.text = "PUSHED";
            }
            if (stringComparer.Equals("btn_2: OFF", response)){
                btn2.text = "RELEASED";
            }
            if (stringComparer.Equals("btn_2: ON", response)){
                btn2.text = "PUSHED";
            }
            if (stringComparer.Equals("btn_3: OFF", response)){
                btn3.text = "RELEASED";
            }
            if (stringComparer.Equals("btn_3: ON", response)){
                btn3.text = "PUSHED";
            }
        }
    }
    
´´´

* Establecemos un método *ReadBtn* en el cual se leerán los estados de cada uno de los botones

´´´
 public void ReadBtn()
    {
        _serialPort.Write("C1\n");
        _serialPort.Write("C2\n");
        _serialPort.Write("C3\n");
        Debug.Log("Send CMS");
    }

´´´

* Por último, El método *LedControl* contiene un *switch* el cual, dependiendo del caso y el estado, se enviará la señal por medio del puerto serial a 
la protoboard y se prenderá el led del botón que se esté presionando.

´´´
  public void LedControl()
    {
        _index = selection.GetComponent<TMP_InputField>().text;
        Debug.Log(_index);
        _ledState = status.GetComponent<TMP_InputField>().text;
        Debug.Log(_ledState);
        
        switch (_index)
        {
            case "1":
                if (_ledState == "ON"){
                    _serialPort.Write("1ON");
                }
                else{
                    _serialPort.Write("1OFF");
                }
                break;
            case "2":
                if (_ledState == "ON"){
                    _serialPort.Write("2ON");
                }
                else{
                    _serialPort.Write("2OFF");
                }
                break;
            case "3":
                if (_ledState == "ON"){
                    _serialPort.Write("3ON");
                }
                else{
                    _serialPort.Write("3OFF");
                }
                break;
            
            default:
                Debug.Log("Case doesn't exist.");
                break;
        }
    }
}

´´´

