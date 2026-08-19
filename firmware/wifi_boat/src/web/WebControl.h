/*
 * WebControl.h
 * 热点 + 网页控制台。
 * 第三阶段起：按钮会调用 Motor，但尚未接线。
 */

#ifndef WIFI_BOAT_WEB_CONTROL_H
#define WIFI_BOAT_WEB_CONTROL_H

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <DNSServer.h>
#include "../../config.h"
#include "../motor/Motor.h"

class WebControl {
 public:
  void begin(Motor* motors);
  void loop();

 private:
  ESP8266WebServer server_;
  DNSServer dns_;
  IPAddress apIp_;
  Motor* motors_ = nullptr;

  void handleRoot();
  void handleCommand();
  void handleNotFound();
  void printCommand(const String& cmd);
};

#endif
