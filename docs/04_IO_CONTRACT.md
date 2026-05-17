# 04_IO_CONTRACT.md：IO Contract v0.1（待 M2 冻结）

> 本文件为 M1 阶段草案。Project Brief 已确认字段级需求，但 C API、结构体和 enum 需在 M2 冻结后才作为最终事实。

## 输入需求

每个样本逐点输入。

已确认输入信息：

| 字段 | 建议类型 | 状态 | 说明 |
|---|---|---|---|
| `timestamp_ms` | `uint32_t` | 待 M2 冻结 | 外部时间戳，单位 ms |
| `ppg[4]` | `int32_t[4]` | 待 M2 冻结 | 4 路同步绿光 PPG |
| `allow_measure` | `bool` 或 uint8 enum | 待 M2 冻结 | 外部运动/测量门控 |

固定输入规格：

- 采样率：50 Hz；
- 时间戳期望步长：20 ms；
- raw 类型：signed `int32_t`；
- ADC 位宽：24 bit；
- 多通道严格同步。

## 输出需求

每检测到一个有效 IBI，输出一次 event。

已确认输出字段：

| 字段 | 建议类型 | 状态 | 说明 |
|---|---|---|---|
| `timestamp_ms` | `uint32_t` | 待 M2 冻结 | event 对应时间，建议后一个脉搏时间 |
| `ibi_ms` | `uint16_t` 或 `uint32_t` | 待 M2 冻结 | 当前 IBI，单位 ms |
| `confidence` | `float` | 待 M2 冻结 | 建议范围 0.0–1.0 |
| `signal_quality` | `float` | 待 M2 冻结 | 建议范围 0.0–1.0 |
| `selected_channel` | `uint8_t` | 待 M2 冻结 | 0–3 |
| `state` | enum | 待 M2 冻结 | 当前算法状态 |
| `reject_reason` | enum | 待 M2 冻结 | 拒绝或状态转换原因 |

## 状态机草案

建议状态：

```text
INIT
ACQUIRE
TRACK
REACQUIRE
INVALID
```

说明：不默认采用 HOLD 输出策略。

## reject_reason 草案

待 M2 冻结，候选项包括：

- `NONE`；
- `NOT_READY`；
- `MEASURE_NOT_ALLOWED`；
- `LOW_SIGNAL_QUALITY`；
- `TIMESTAMP_ERROR`；
- `IBI_OUT_OF_RANGE`；
- `CHANNEL_INVALID`；
- `STATE_INVALID`。

## API 设计约束

M2 设计 C API 时必须满足：

1. `init` / `reset` / `process` 接口清晰；
2. 不使用动态内存；
3. 所有算法状态由 caller 分配的 context 承载；
4. `process` 支持逐点调用；
5. 输出 event 应明确区分“本次有新 IBI”和“本次无输出”；
6. 不输出 HR 或 RMSSD；
7. 不改变 Project Brief 已确认字段。
