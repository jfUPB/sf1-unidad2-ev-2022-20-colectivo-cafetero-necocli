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
    
    public void ReadBtn()
    {
        _serialPort.Write("C1\n");
        _serialPort.Write("C2\n");
        _serialPort.Write("C3\n");
        Debug.Log("Send CMS");
    }
    
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