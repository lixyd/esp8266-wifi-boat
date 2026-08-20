# 物料清单

跨 Agent 细目（照片+你给的铭牌）：`Obsidian/AgentSharedMemory/项目/小船/采购.md`

| 序号 | 名称 | 规格 | 状态 |
|------|------|------|------|
| 1 | 开发板 | NodeMCU V3，ESP8266，CH340，USB-C | 船上在用；串口 `/dev/cu.usbserial-10` |
| 2 | USB 数据线 | USB-C，必须能传数据（不能是纯充电线） | 已使用 |
| 3 | 电机驱动 | L298N 双路 **HW-095** | 已接线（ENA/ENB 帽已拔） |
| 4 | 直流减速电机 | 2 个（带齿轮箱） | 左 OUT1/2，右 OUT3/4 |
| 5 | 船体外壳 / 双桨 | — | 未在记录里确认型号 |
| 6 | 电池 | 7.4V 或 12V | 已给 L298N+ESP 供电 |
| 7 | 摄像头+麦 | Mobilechplus MW1350 USB2.0 1.3MP；圆板 YC-08-A3 | **不接 NodeMCU** |
| 8 | 舵机 | — | 暂不用 |

USB 识别记录（第一阶段实测）：

- 设备名：`USB Serial`
- 厂商 ID：`0x1A86`（沁恒 / WCH）
- 产品 ID：`0x7523`（CH340）
- Mac 串口：`/dev/cu.usbserial-10`

接线表见 `docs/04-接线测试.md`（第四阶段才填）。
