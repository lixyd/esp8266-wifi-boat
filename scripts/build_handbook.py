#!/usr/bin/env python3
"""Generate docs/手册.html from current firmware + fixed handbook copy."""
from pathlib import Path
import html

ROOT = Path(__file__).resolve().parents[1]
OUT = ROOT / "docs" / "手册.html"


def read(rel: str) -> str:
    return (ROOT / rel).read_text(encoding="utf-8")


def code(rel: str) -> str:
    return html.escape(read(rel))


blocks = {
    "config": code("firmware/wifi_boat/config.h"),
    "ino": code("firmware/wifi_boat/wifi_boat.ino"),
    "motor_h": code("firmware/wifi_boat/src/motor/Motor.h"),
    "motor_cpp": code("firmware/wifi_boat/src/motor/Motor.cpp"),
    "web_h": code("firmware/wifi_boat/src/web/WebControl.h"),
    "web_cpp": code("firmware/wifi_boat/src/web/WebControl.cpp"),
    "page": code("firmware/wifi_boat/src/web/page.html.h"),
    "compile": code("scripts/compile.sh"),
    "upload": code("scripts/upload.sh"),
}

page = r'''<!DOCTYPE html>
<html lang="zh-CN">
<head>
<meta charset="utf-8">
<meta name="viewport" content="width=device-width,initial-scale=1">
<title>ESP8266 Wi-Fi 遥控船 · 手册</title>
<style>
  :root {
    --bg: #f4efe6;
    --paper: #fffdf8;
    --ink: #1c1915;
    --muted: #5c564c;
    --line: #d9d0c2;
    --accent: #0f4c5c;
    --warn: #8a2f12;
    --ok: #1f5c3a;
    --code-bg: #1b2428;
    --code-fg: #e7eef0;
  }
  * { box-sizing: border-box; }
  html { scroll-behavior: smooth; }
  body {
    margin: 0;
    font: 16px/1.55 "Songti SC", "Source Han Serif SC", "Noto Serif SC", Georgia, serif;
    color: var(--ink);
    background: var(--bg);
  }
  .wrap { display: grid; grid-template-columns: 240px minmax(0, 920px); gap: 0; }
  nav {
    position: sticky; top: 0; height: 100vh; overflow: auto;
    padding: 28px 18px; border-right: 1px solid var(--line); background: #efe8dc;
  }
  nav h2 { font-size: 13px; letter-spacing: .12em; text-transform: uppercase; color: var(--muted); margin: 0 0 12px; }
  nav a { display: block; color: var(--ink); text-decoration: none; padding: 5px 0; font-size: 14px; }
  nav a:hover { color: var(--accent); }
  main { padding: 36px 40px 80px; background: var(--paper); min-height: 100vh; }
  header h1 { font-size: 34px; line-height: 1.15; margin: 0 0 8px; }
  .lede { color: var(--muted); margin: 0 0 28px; }
  h2 { margin: 40px 0 12px; font-size: 24px; border-bottom: 2px solid var(--ink); padding-bottom: 6px; }
  h3 { margin: 26px 0 10px; font-size: 18px; color: var(--accent); }
  table { width: 100%; border-collapse: collapse; font-size: 14.5px; margin: 10px 0 18px; }
  th, td { border: 1px solid var(--line); padding: 8px 10px; text-align: left; vertical-align: top; }
  th { background: #f3ebe0; }
  code, kbd { font-family: ui-monospace, Menlo, monospace; font-size: 13px; background: #efe6d8; padding: 1px 5px; }
  pre {
    background: var(--code-bg); color: var(--code-fg); padding: 14px 16px;
    overflow: auto; font: 12.5px/1.45 ui-monospace, Menlo, monospace;
    border-radius: 6px;
  }
  .note, .danger { padding: 10px 14px; border-left: 4px solid var(--ok); background: #eef6f0; margin: 12px 0 18px; }
  .danger { border-color: var(--warn); background: #f8eee8; }
  figure { margin: 12px 0 22px; }
  figure img { max-width: 100%; height: auto; border: 1px solid var(--line); background: #fff; }
  figcaption { font-size: 13px; color: var(--muted); margin-top: 6px; }
  .grid { display: grid; grid-template-columns: 1fr 1fr; gap: 14px; }
  @media (max-width: 900px) {
    .wrap { grid-template-columns: 1fr; }
    nav { position: relative; height: auto; }
    main { padding: 24px 16px 60px; }
    .grid { grid-template-columns: 1fr; }
  }
</style>
</head>
<body>
<div class="wrap">
<nav>
  <h2>目录</h2>
  <a href="#overview">1. 这是什么</a>
  <a href="#buy">2. 已买件</a>
  <a href="#use">3. 怎么开船</a>
  <a href="#wire">4. 接线</a>
  <a href="#speed">5. 油门</a>
  <a href="#flash">6. 编译烧录</a>
  <a href="#cam">7. 摄像头</a>
  <a href="#code">8. 程序</a>
  <a href="#pits">9. 踩过的坑</a>
</nav>
<main>
<header>
  <h1>ESP8266 双电机 Wi-Fi 遥控船</h1>
  <p class="lede">本机手册 · 2026-08-20 · 代码与照片都在这个仓库里。双击本文件即可离线阅读。<br>
  GitHub：<a href="https://github.com/lixyd/esp8266-wifi-boat">lixyd/esp8266-wifi-boat</a>（公开）</p>
</header>

<section id="overview">
<h2>1. 这是什么</h2>
<p>NodeMCU V3（ESP8266）开热点，手机网页遥控两只减速电机。驱动是 L298N（HW-095）。现在双电机已接，网页能调速，电池可以同时给驱动和板子供电。</p>
<table>
  <tr><th>热点</th><td><code>wifi-boat</code></td></tr>
  <tr><th>密码</th><td><code>12345678</code></td></tr>
  <tr><th>控制页</th><td><code>http://192.168.4.1/</code></td></tr>
  <tr><th>板型</th><td>NodeMCU 1.0 (ESP-12E)，FQBN <code>esp8266:esp8266:nodemcuv2</code></td></tr>
  <tr><th>串口</th><td><code>/dev/cu.usbserial-10</code> · CH340 · VID 1A86 PID 7523</td></tr>
  <tr><th>本仓库</th><td><code>~/Desktop/esp8266-wifi-boat</code></td></tr>
</table>
<div class="danger">无人机是另一套东西（125mm / 8520）。船上的板、L298N、减速电机、电池、摄像头一律不拆去飞。</div>
</section>

<section id="buy">
<h2>2. 已买件</h2>
<table>
  <tr><th>件</th><th>规格（按实物）</th><th>数量</th><th>状态</th></tr>
  <tr><td>开发板</td><td>ESP8266 串口 wifi 模块 NodeMCU Lua V3 物联网开发板 CH340，USB-C</td><td>1</td><td>船上在用</td></tr>
  <tr><td>数据线</td><td>USB-C，必须能传数据</td><td>1</td><td>烧录用</td></tr>
  <tr><td>驱动</td><td>L298N 双路，板号 <b>HW-095</b>，两颗 220μF 35V</td><td>1</td><td>已接线</td></tr>
  <tr><td>电机</td><td>直流减速电机（带齿轮箱）</td><td>2</td><td>左 OUT1/2，右 OUT3/4</td></tr>
  <tr><td>电池</td><td>7.4V（2S）或 12V 均可。螺丝印 12V 不代表必须 12 伏</td><td>1</td><td>已供电；空过会蜂鸣</td></tr>
  <tr><td>摄像头</td><td>Mobilechplus MW1350 USB2.0 130 万像素，圆板 YC-08-A3</td><td>1</td><td>不能接这块 ESP</td></tr>
  <tr><td>船体 / 桨</td><td>未拍型号</td><td>?</td><td>未确认</td></tr>
</table>
<p>本机软件：Arduino IDE 2.3.10 arm64、arduino-cli 1.5.1、esp8266 核心 3.1.2。CH340 驱动不要再装。</p>
<p>以后若还买：真要手机看画面再买 ESP32-CAM；烧了再买备用电机。不要为船再买空心杯、1S、飞控。</p>
</section>

<section id="use">
<h2>3. 怎么开船</h2>
<ol>
  <li>电池正极 → L298N +12V，负极 → L298N GND。USB 先拔掉。</li>
  <li>手机连热点 <b>wifi-boat</b> / <b>12345678</b>。</li>
  <li>浏览器打开 <code>http://192.168.4.1/</code>（部分手机会自动弹出）。</li>
  <li>先点「停止」，再点前进 / 左转 / 右转。特慢是脉冲爬行，慢 40%，快 90%。</li>
</ol>
<p>网页按钮对应接口 <code>/api/cmd?c=forward</code>（还有 backward、left、right、uturn、stop、crawl、slow、fast）。</p>
</section>

<section id="wire">
<h2>4. 接线</h2>
<figure>
  <img src="img/01-L298N-HW095-绿框针脚.jpg" alt="L298N HW-095 绿框 6 针">
  <figcaption>你拍的绿框：ENA / IN1 / IN2 / IN3 / IN4 / ENB。两端黑帽是 ENA、ENB 跳线，调速必须拔掉。</figcaption>
</figure>
<h3>绿框 6 针 → NodeMCU</h3>
<table>
  <tr><th>L298N</th><th>NodeMCU</th><th>GPIO</th><th>作用</th></tr>
  <tr><td>ENA</td><td>D5</td><td>14</td><td>左速度 PWM。跳线帽已拔，不要再接到 +5V</td></tr>
  <tr><td>IN1</td><td>D1</td><td>5</td><td>左方向</td></tr>
  <tr><td>IN2</td><td>D2</td><td>4</td><td>左方向</td></tr>
  <tr><td>IN3</td><td>D7</td><td>13</td><td>右方向</td></tr>
  <tr><td>IN4</td><td>D8</td><td>15</td><td>右方向。不要用 D0，反转会停</td></tr>
  <tr><td>ENB</td><td>D6</td><td>12</td><td>右速度 PWM。跳线帽已拔</td></tr>
  <tr><td>GND</td><td>GND</td><td>—</td><td>必须共地</td></tr>
</table>
<h3>电源与电机</h3>
<pre>电池正极 ──→ L298N +12V
电池负极 ──→ L298N GND ──→ NodeMCU GND
              L298N +5V ──→ NodeMCU VIN（或 5V/VU）  禁止接到 3V3

OUT1 / OUT2 → 左电机
OUT3 / OUT4 → 右电机</pre>
<div class="danger">三个螺丝旁那颗没字的小跳线帽<b>戴着</b>（板载 5V 稳压）。ENA/ENB 两颗帽子<b>拔掉</b>。不要把电池 12V 接到 5V 螺丝。不要 USB 和电池同时给 ESP。</div>
<p>桨转反了：只对调那一侧 OUT 的两根线。3V3、A0、D3、D4 都不要接到 L298N。</p>
</section>

<section id="speed">
<h2>5. 油门</h2>
<p>PWM 范围 0–1023，频率 1 kHz。连续低于约 75% 常常只叫不转（L298N 压降）。</p>
<table>
  <tr><th>档</th><th>网页</th><th>实际</th></tr>
  <tr><td>特慢</td><td>crawl</td><td>每 400ms 给 20ms、85%（最低可转 75%+10%）的脉冲</td></tr>
  <tr><td>慢</td><td>slow</td><td>连续 40%</td></tr>
  <tr><td>快</td><td>fast</td><td>连续 90%</td></tr>
  <tr><td>转弯</td><td>left / right</td><td>内侧 50%、外侧 80%（相对当前档）</td></tr>
</table>
</section>

<section id="flash">
<h2>6. 编译烧录</h2>
<pre>cd ~/Desktop/esp8266-wifi-boat
./scripts/compile.sh     # 只编译
./scripts/upload.sh      # 编译并烧到当前 CH340 串口</pre>
<p>接了 L298N 后 115200 烧录容易失败，脚本固定 <b>57600</b>。串口监视器在 IDE「查看」菜单，波特率 115200。</p>
<h3>compile.sh</h3>
<pre>__COMPILE__</pre>
<h3>upload.sh</h3>
<pre>__UPLOAD__</pre>
</section>

<section id="cam">
<h2>7. 摄像头（接不上这块 ESP）</h2>
<p>外壳铭牌：<code>USB2.0 1.3Mega Pixel · Mobilechplus MW1350</code>。圆板丝印 <code>YC-08-A3</code>。这是 USB 免驱摄像头，ESP8266 没有 USB 主机。</p>
<table>
  <tr><th>线色</th><th>USB 脚</th></tr>
  <tr><td>红</td><td>+5V</td></tr>
  <tr><td>白</td><td>D−</td></tr>
  <tr><td>绿</td><td>D+</td></tr>
  <tr><td>黑</td><td>GND</td></tr>
  <tr><td>黄 / 蓝</td><td>驻极体麦，不是给 ESP ADC 的</td></tr>
</table>
<div class="danger">不要把 D+/D− 焊到 GPIO。以后要手机看画面另买 ESP32-CAM。</div>
<div class="grid">
  <figure><img src="img/02-摄像头外壳-MW1350.jpg" alt="MW1350 外壳"><figcaption>外壳铭牌</figcaption></figure>
  <figure><img src="img/06-镜头丝印-YC-08-A3.jpg" alt="YC-08-A3"><figcaption>圆板丝印 YC-08-A3</figcaption></figure>
  <figure><img src="img/04-摄像头芯片与麦.jpg" alt="芯片与麦"><figcaption>主控 + 驻极体麦</figcaption></figure>
  <figure><img src="img/05-摄像头线束.jpg" alt="线束"><figcaption>红白绿黑 + 黄蓝麦线</figcaption></figure>
</div>
</section>

<section id="code">
<h2>8. 程序（仓库当前文件）</h2>
<p>入口 <code>firmware/wifi_boat/</code>。改油门改 <code>config.h</code>，改接线改同一文件里的 PIN_*。</p>
<h3>config.h</h3>
<pre>__CONFIG__</pre>
<h3>wifi_boat.ino</h3>
<pre>__INO__</pre>
<h3>src/motor/Motor.h</h3>
<pre>__MOTOR_H__</pre>
<h3>src/motor/Motor.cpp</h3>
<pre>__MOTOR_CPP__</pre>
<h3>src/web/WebControl.h</h3>
<pre>__WEB_H__</pre>
<h3>src/web/WebControl.cpp</h3>
<pre>__WEB_CPP__</pre>
<h3>src/web/page.html.h</h3>
<pre>__PAGE__</pre>
</section>

<section id="pits">
<h2>9. 踩过的坑</h2>
<table>
  <tr><th>现象</th><th>原因 / 处理</th></tr>
  <tr><td>反转变成停止</td><td>IN4 接到了 D0。改接到 D8（GPIO15）</td></tr>
  <tr><td>烧录 verify fail / Invalid head</td><td>接电机后 115200 不稳。改 57600</td></tr>
  <tr><td>慢档几乎是满速</td><td>不要把 40% 映射到 780–1023。百分比按 0–1023 线性</td></tr>
  <tr><td>10% 连续只蜂鸣</td><td>低于约 75% 转不动。特慢改成 85% 短脉冲</td></tr>
  <tr><td>点按键蜂鸣不转</td><td>电池空了</td></tr>
  <tr><td>Wi-Fi 一调电机就掉</td><td>用 WIFI_NONE_SLEEP，PWM 1 kHz，不要乱改 analogWriteRange(100)</td></tr>
  <tr><td>IDE 找不到串口监视器</td><td>在「查看」，不在「工具」</td></tr>
  <tr><td>摄像头没画面</td><td>MW1350 是 USB 头，这块板带不了</td></tr>
</table>
</section>
</main>
</div>
</body>
</html>
'''

for key, token in (
    ("__CONFIG__", "config"),
    ("__INO__", "ino"),
    ("__MOTOR_H__", "motor_h"),
    ("__MOTOR_CPP__", "motor_cpp"),
    ("__WEB_H__", "web_h"),
    ("__WEB_CPP__", "web_cpp"),
    ("__PAGE__", "page"),
    ("__COMPILE__", "compile"),
    ("__UPLOAD__", "upload"),
):
    page = page.replace(key, blocks[token])

OUT.write_text(page, encoding="utf-8")
print(f"wrote {OUT} ({OUT.stat().st_size} bytes)")
