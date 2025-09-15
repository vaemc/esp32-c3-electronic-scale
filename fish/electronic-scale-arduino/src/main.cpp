#include "HX711.h"
#include <WiFi.h>
#include <WebSocketsServer.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

const char *ssid = "xxx";
const char *password = "xxx";
WebSocketsServer webSocket = WebSocketsServer(81);

// 定义 HX711 引脚
#define DT_PIN 10 // HX711 DT 接 ESP32 GPIO10
#define SCK_PIN 5 // HX711 SCK 接 ESP32 GPIO5

HX711 scale;

float calibration_factor = 390; // 校准因子, 初始值可调

void _task(void *pvParameters)
{
  while (1)
  {
    if (scale.is_ready())
    {
      int weight = (int)scale.get_units(10); // 读取平均值
      Serial.print("重量: ");
      Serial.print(weight);
      Serial.println(" g");
      String str = String(weight);
      webSocket.broadcastTXT(str);
    }
    else
    {
      Serial.println("HX711 未准备好");
    }

    vTaskDelay(pdMS_TO_TICKS(500));
  }
}

void setup()
{
  Serial.begin(115200);

  WiFi.begin(ssid, password);
  Serial.print("连接 WiFi 中");
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }

  webSocket.begin();


  Serial.print("IP 地址: ");
  Serial.println(WiFi.localIP());

  xTaskCreate(_task, "_task", 8192, NULL, 5, NULL);

  Serial.println("HX711 称重模块初始化...");

  scale.begin(DT_PIN, SCK_PIN);
  scale.set_scale(calibration_factor); // 设置校准因子
  scale.tare();                        // 去皮（清零）

  Serial.println("请确保秤台上没有物品...");
  delay(2000);
  Serial.println("开始读取重量 (克)...");
}

void loop()
{

  webSocket.loop();
}