# 04_IO_CONTRACT.md：IO Contract v0.2（M2 冻结）

> 本文件冻结 M2 MCU C API、输入结构体、输出 event 结构体、状态 enum 与 reject reason enum。后续如需改变公开 API、输入/输出字段、采样率或状态机主策略，必须作为 S0 事项提交 Owner 决策。

## 固定输入规格

| 项目 | M2 冻结值 |
|---|---|
| PPG 通道数 | 4 路 |
| 光源 | 绿光 |
| 采样率 | 50 Hz |
| 时间戳期望步长 | 20 ms |
| timestamp 类型 | `uint32_t`，单位 ms |
| PPG raw 类型 | signed `int32_t` |
| ADC 位宽 | 24 bit |
| 多通道同步 | 严格同步 |
| 运动门控 | 每个样本输入 `allow_measure` |

固定 fixture 路径保持为：

```text
tests/fixtures/sample_ppg.csv
```

## C API 文件

| 文件 | 作用 |
|---|---|
| `include/ppg_ibi_config.h` | 固定通道数、采样率、ADC 位宽、RAM 预算等宏 |
| `include/ppg_ibi.h` | MCU 公开 API |
| `src/ppg_ibi_internal.h` | 内部占位实现宏 |
| `src/ppg_ibi.c` | M2 API 骨架实现 |

## 公开宏

| 宏 | 冻结值 | 说明 |
|---|---:|---|
| `PPG_IBI_CHANNEL_COUNT` | 4 | PPG 通道数 |
| `PPG_IBI_SAMPLE_RATE_HZ` | 50 | 固定采样率 |
| `PPG_IBI_SAMPLE_PERIOD_MS` | 20 | 固定采样周期 |
| `PPG_IBI_ADC_BITS` | 24 | 输入 ADC 位宽 |
| `PPG_IBI_TIMESTAMP_TOLERANCE_MS` | 2 | timestamp 步长容差，M3 起用于异常检测 |
| `PPG_IBI_RAM_BUDGET_BYTES` | 20480 | 算法总 RAM hard target |
| `PPG_IBI_SELECTED_CHANNEL_NONE` | 255 | 无有效 event 时的占位 selected_channel |

版本宏：

```c
PPG_IBI_VERSION_MAJOR
PPG_IBI_VERSION_MINOR
PPG_IBI_VERSION_PATCH
PPG_IBI_VERSION_U32
```

版本函数：

```c
uint32_t ppg_ibi_version(void);
```

## config 类型

```c
typedef struct
{
    uint16_t sample_rate_hz;
    uint16_t sample_period_ms;
    uint16_t timestamp_tolerance_ms;
} ppg_ibi_config_t;
```

约束：

1. `sample_rate_hz` 必须为 `PPG_IBI_SAMPLE_RATE_HZ`。
2. `sample_period_ms` 必须为 `PPG_IBI_SAMPLE_PERIOD_MS`。
3. 默认 `timestamp_tolerance_ms` 为 `PPG_IBI_TIMESTAMP_TOLERANCE_MS`。
4. 当前 API 不支持运行时切换 25 Hz / 100 Hz。

## input sample 类型

```c
typedef struct
{
    uint32_t timestamp_ms;
    int32_t ppg[PPG_IBI_CHANNEL_COUNT];
    bool allow_measure;
} ppg_ibi_sample_t;
```

字段含义：

| 字段 | 类型 | 说明 |
|---|---|---|
| `timestamp_ms` | `uint32_t` | 外部时间戳，单位 ms |
| `ppg[4]` | `int32_t[4]` | 4 路严格同步绿光 PPG raw |
| `allow_measure` | `bool` | 外部门控；`false` 时立即停止输出有效 IBI |

## output event 类型

```c
typedef struct
{
    uint32_t timestamp_ms;
    uint32_t ibi_ms;
    float confidence;
    float signal_quality;
    uint8_t selected_channel;
    ppg_ibi_state_t state;
    ppg_ibi_reject_reason_t reject_reason;
} ppg_ibi_event_t;
```

字段含义：

| 字段 | 类型 | 说明 |
|---|---|---|
| `timestamp_ms` | `uint32_t` | event 对应时间，后续有效 IBI event 建议使用后一个有效搏动时间 |
| `ibi_ms` | `uint32_t` | 当前 IBI，单位 ms |
| `confidence` | `float` | 当前 IBI 可信度，目标范围 0.0–1.0 |
| `signal_quality` | `float` | 当前选中通道或融合后的信号质量，目标范围 0.0–1.0 |
| `selected_channel` | `uint8_t` | 有效 event 时为 0–3；无有效 event 占位为 `PPG_IBI_SELECTED_CHANNEL_NONE` |
| `state` | `ppg_ibi_state_t` | 当前算法状态 |
| `reject_reason` | `ppg_ibi_reject_reason_t` | 拒绝或状态转换原因 |

API 不输出 `hr_bpm`、`rmssd` 或长期 HRV 指标。

## context 类型

`ppg_ibi_context_t` 由调用者分配并传入 API，不使用动态内存。调用者不得直接修改 context 内部字段；字段公开仅用于 MCU 静态分配和 M2 编译期可见性。

## 状态 enum

```c
typedef enum
{
    PPG_IBI_STATE_INIT = 0,
    PPG_IBI_STATE_ACQUIRE,
    PPG_IBI_STATE_TRACK,
    PPG_IBI_STATE_REACQUIRE,
    PPG_IBI_STATE_INVALID
} ppg_ibi_state_t;
```

说明：不采用 `HOLD` 作为核心输出状态；`allow_measure = false` 时不输出有效 IBI，恢复允许测量后进入 `REACQUIRE`。

## reject reason enum

```c
typedef enum
{
    PPG_IBI_REJECT_NONE = 0,
    PPG_IBI_REJECT_NOT_READY,
    PPG_IBI_REJECT_MEASURE_NOT_ALLOWED,
    PPG_IBI_REJECT_LOW_SIGNAL_QUALITY,
    PPG_IBI_REJECT_TIMESTAMP_ERROR,
    PPG_IBI_REJECT_IBI_OUT_OF_RANGE,
    PPG_IBI_REJECT_CHANNEL_INVALID,
    PPG_IBI_REJECT_STATE_INVALID
} ppg_ibi_reject_reason_t;
```

## 函数契约

```c
void ppg_ibi_config_default(ppg_ibi_config_t *config);

ppg_ibi_status_t ppg_ibi_init(ppg_ibi_context_t *ctx,
                              const ppg_ibi_config_t *config);

ppg_ibi_status_t ppg_ibi_reset(ppg_ibi_context_t *ctx);

ppg_ibi_status_t ppg_ibi_process(ppg_ibi_context_t *ctx,
                                  const ppg_ibi_sample_t *sample,
                                  ppg_ibi_event_t *event,
                                  bool *event_generated);
```

`ppg_ibi_process()` 契约：

1. 逐点输入，每次处理一个 `ppg_ibi_sample_t`。
2. `event_generated = true` 仅表示本次产生新的有效 IBI event。
3. `event_generated = false` 时，不得把 `event->ibi_ms` 当作有效 IBI。
4. M2 占位实现永远不产生有效 IBI event。
5. 后续算法不得通过 Python 或第三方库替代 MCU C 实现。

## M2 非实现内容

M2 只冻结 API 与占位骨架，不实现：

1. 复杂滤波；
2. SQI 公式；
3. 主通道选择；
4. 脉搏峰检测；
5. IBI 计算；
6. 完整状态机；
7. RMSSD 或 HRV 指标。
