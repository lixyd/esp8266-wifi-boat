#include "WebControl.h"
#include "page.html.h"

void WebControl::begin() {
  apIp_ = IPAddress(192, 168, 4, 1);

  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(apIp_, apIp_, IPAddress(255, 255, 255, 0));
  const bool ok = WiFi.softAP(WIFI_AP_SSID, WIFI_AP_PASSWORD, WIFI_AP_CHANNEL, false, WIFI_AP_MAX_CONN);

  Serial.println();
  Serial.println(F("================================"));
  Serial.println(F("wifi_boat  第二阶段  网页控制"));
  Serial.println(F("按钮只打印文字，不驱动电机"));
  Serial.print(F("热点名称: "));
  Serial.println(WIFI_AP_SSID);
  Serial.print(F("热点密码: "));
  Serial.println(WIFI_AP_PASSWORD);
  Serial.print(F("控制页:   http://"));
  Serial.println(apIp_);
  Serial.print(F("softAP:   "));
  Serial.println(ok ? F("OK") : F("失败"));
  Serial.println(F("================================"));

  // 把所有域名解析到 192.168.4.1，手机连上后容易弹出登录页。
  dns_.start(53, "*", apIp_);

  server_.on("/", HTTP_GET, [this]() { handleRoot(); });
  server_.on("/api/cmd", HTTP_GET, [this]() { handleCommand(); });
  // iOS / Android 探测「需要登录的热点」时会访问这些地址
  server_.on("/hotspot-detect.html", HTTP_GET, [this]() { handleRoot(); });
  server_.on("/generate_204", HTTP_GET, [this]() { handleRoot(); });
  server_.on("/ncsi.txt", HTTP_GET, [this]() { handleRoot(); });
  server_.on("/canonical.html", HTTP_GET, [this]() { handleRoot(); });
  server_.onNotFound([this]() { handleNotFound(); });

  server_.begin(WEB_SERVER_PORT);
}

void WebControl::loop() {
  dns_.processNextRequest();
  server_.handleClient();
}

void WebControl::handleRoot() {
  server_.send_P(200, "text/html; charset=utf-8", CONTROL_PAGE);
}

void WebControl::handleCommand() {
  String cmd = server_.arg("c");
  cmd.toLowerCase();
  printCommand(cmd);

  String text = cmd;
  if (cmd == "forward") text = "前进";
  else if (cmd == "backward") text = "后退";
  else if (cmd == "left") text = "左转";
  else if (cmd == "right") text = "右转";
  else if (cmd == "uturn") text = "掉头";
  else if (cmd == "stop") text = "停止";
  else text = "未知指令: " + cmd;

  String json = "{\"ok\":true,\"cmd\":\"" + cmd + "\",\"text\":\"" + text + "\"}";
  server_.send(200, "application/json; charset=utf-8", json);
}

void WebControl::handleNotFound() {
  // 没匹配到的地址也给控制页，方便手机强制门户弹出。
  handleRoot();
}

void WebControl::printCommand(const String& cmd) {
  Serial.print(F("[CMD] "));
  if (cmd == "forward") Serial.println(F("前进  forward"));
  else if (cmd == "backward") Serial.println(F("后退  backward"));
  else if (cmd == "left") Serial.println(F("左转  left"));
  else if (cmd == "right") Serial.println(F("右转  right"));
  else if (cmd == "uturn") Serial.println(F("掉头  uturn"));
  else if (cmd == "stop") Serial.println(F("停止  stop"));
  else {
    Serial.print(F("未知  "));
    Serial.println(cmd);
  }
}
