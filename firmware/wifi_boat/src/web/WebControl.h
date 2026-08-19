/*
 * WebControl.h
 * 第二阶段：ESP8266 开热点 + 网页控制台。
 *
 * 按钮目前只在串口打印中文，不驱动电机 GPIO。
 * 以后第三阶段把 handleCommand() 接到 Motor 类即可。
 */

#ifndef WIFI_BOAT_WEB_CONTROL_H
#define WIFI_BOAT_WEB_CONTROL_H

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <DNSServer.h>
#include "../../config.h"

class WebControl {
 public:
  void begin();
  void loop();

 private:
  ESP8266WebServer server_;
  DNSServer dns_;
  IPAddress apIp_;

  void handleRoot();
  void handleCommand();
  void handleNotFound();
  void printCommand(const String& cmd);
};

#endif
