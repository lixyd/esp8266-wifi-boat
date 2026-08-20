/*
 * 控制页 HTML，放 PROGMEM 节省 RAM。
 * 手机浏览器打开后是大按钮，点一下发 /api/cmd?c=xxx。
 */
#ifndef WIFI_BOAT_PAGE_HTML_H
#define WIFI_BOAT_PAGE_HTML_H

#include <Arduino.h>

static const char CONTROL_PAGE[] PROGMEM = R"HTML(
<!DOCTYPE html>
<html lang="zh-CN">
<head>
<meta charset="utf-8">
<meta name="viewport" content="width=device-width,initial-scale=1,viewport-fit=cover">
<meta name="apple-mobile-web-app-capable" content="yes">
<title>遥控船</title>
<style>
  :root { color-scheme: dark; }
  * { box-sizing: border-box; -webkit-tap-highlight-color: transparent; }
  body {
    margin: 0; min-height: 100vh; font-family: -apple-system, sans-serif;
    background: #0b1220; color: #e8eefc; display: flex; flex-direction: column;
    align-items: center; padding: 20px 16px 32px;
  }
  h1 { font-size: 20px; font-weight: 650; margin: 8px 0 4px; }
  .sub { color: #8aa0c4; font-size: 13px; margin-bottom: 18px; }
  .grid {
    width: min(360px, 100%);
    display: grid;
    grid-template-columns: 1fr 1fr 1fr;
    gap: 12px;
  }
  button {
    border: 0; border-radius: 16px; padding: 18px 8px;
    font-size: 18px; font-weight: 650; color: #fff;
    background: #24344f; min-height: 72px;
  }
  button:active { transform: scale(0.97); filter: brightness(1.15); }
  .fwd { grid-column: 2; background: #1f6feb; }
  .left { grid-column: 1; }
  .right { grid-column: 3; }
  .back { grid-column: 2; }
  .uturn { grid-column: 1 / span 3; background: #3d4f6f; }
  .stop { grid-column: 1 / span 3; background: #c62828; min-height: 84px; font-size: 22px; }
  .spd { min-height: 52px; font-size: 15px; background: #1a283c; }
  .note { width: min(360px, 100%); color: #8aa0c4; font-size: 12px; line-height: 1.45; margin: 14px 0 0; }
  #status {
    margin-top: 16px; min-height: 24px; color: #9ad07a; font-size: 15px;
  }
</style>
</head>
<body>
  <h1>Wi-Fi 遥控船</h1>
  <div class="sub">特慢=脉冲爬行 · 慢40% · 快90%</div>
  <div class="grid">
    <button class="fwd" onclick="go('forward')">前进</button>
    <button class="left" onclick="go('left')">左转</button>
    <button class="right" onclick="go('right')">右转</button>
    <button class="back" onclick="go('backward')">后退</button>
    <button class="uturn" onclick="go('uturn')">掉头</button>
    <button class="stop" onclick="go('stop')">停止</button>
    <button class="spd" onclick="go('crawl')">特慢</button>
    <button class="spd" onclick="go('slow')">慢</button>
    <button class="spd" onclick="go('fast')">快</button>
  </div>
  <div class="note">特慢：每 0.4 秒给 20ms「刚能转+10%」的脉冲，桨会一顿一顿往前。连续 10% 油门转不动。慢 40%，快 90%。</div>
  <div id="status">已连接 · 慢</div>
<script>
async function go(cmd) {
  const el = document.getElementById('status');
  el.textContent = '发送中…';
  try {
    const r = await fetch('/api/cmd?c=' + cmd);
    const j = await r.json();
    el.textContent = j.text || cmd;
  } catch (e) {
    el.textContent = '发送失败，确认还连着 wifi-boat';
  }
}
setInterval(async () => {
  try { await fetch('/api/ping'); }
  catch (e) {
    document.getElementById('status').textContent = '已断开，请重连 wifi-boat';
  }
}, 2500);
</script>
</body>
</html>
)HTML";

#endif
