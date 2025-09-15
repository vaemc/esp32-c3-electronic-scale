#include "HX711.h"

// HX711 引脚定义
#define DT_PIN  10
#define SCK_PIN 5

HX711 scale;

// 初始校准因子（根据你的传感器可能需要调大或调小）
float calibration_factor = 390;

void setup() {
  Serial.begin(115200);
  Serial.println("HX711 校准程序开始");

  scale.begin(DT_PIN, SCK_PIN);
  scale.set_scale(calibration_factor);
  scale.tare(); // 去皮

  Serial.println("请确保秤台上没有物品...");
  delay(2000);
  Serial.println("现在可以开始放已知重量的物体进行校准");
  Serial.println("使用 '+' 或 '-' 调整校准因子");
  Serial.println("输入 's' 显示当前校准因子");
  Serial.println();
}

void loop() {
  // 检查是否有串口输入
  if (Serial.available()) {
    char input = Serial.read();

    if (input == '+') {
      calibration_factor += 10;
    } else if (input == '-') {
      calibration_factor -= 10;
    } else if (input == 's') {
      Serial.print("当前校准因子: ");
      Serial.println(calibration_factor, 4);
    }

    scale.set_scale(calibration_factor);
  }

  if (scale.is_ready()) {
    float weight = scale.get_units(10); // 取10次平均值
    Serial.print("重量: ");
    Serial.print(weight, 2);
    Serial.print(" g\t 校准因子: ");
    Serial.println(calibration_factor, 4);
  } else {
    Serial.println("HX711 未准备好");
  }

  delay(500);
}