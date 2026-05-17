# 00_PROJECT_BRIEF.md：Project Brief v0.1

## 1. 项目目标

在 Ambiq Apollo3.5 / Cortex-M4F 级 MCU 上，从 4 路严格同步绿光 PPG 中计算逐搏 IBI，供外部模块进一步计算 HRV / RMSSD。

本项目关注：

- 逐点输入 PPG 原始数据；
- 在 `allow_measure = true` 时检测有效脉搏；
- 每检测到一个有效搏动间隔，输出一次 IBI event；
- 对低质量、门控禁止、异常时间戳等情况进行拒绝或重新捕获；
- 面向 MCU 实现，强调 RAM、接口稳定性、可测试性和可移植性。

## 2. 项目非目标

当前明确不做：

1. 不作为医疗诊断软件。
2. 不输出临床诊断结论。
3. 不在算法内部计算 RMSSD。
4. 不在算法内部计算长期 HRV 指标。
5. 不使用第三方 PPG / IBI / HR 算法库作为核心实现。
6. 不使用 ACC 做运动识别。
7. 当前不输出实时 HR。
8. 当前不设置硬性 ROM 预算和单次 `process()` 时间预算。
9. 当前不做总体准确性证明；后期先做人工可视化检验。

## 3. 输入数据与字段

### MCU 实时输入

| 项目 | 已确认规格 |
|---|---|
| PPG 通道数 | 4 路 |
| 光源 | 绿光 |
| 采样率 | 50 Hz |
| 输入方式 | 逐点输入 |
| 多通道同步 | 严格同步 |
| PPG raw 类型 | `int32_t` |
| ADC 位宽 | 24 bit |
| raw 极性 | signed |
| timestamp | 每个样本带 `timestamp_ms` |
| 内部 sample counter | 需要，用于校验外部时间戳错误 |
| ACC | 无 |
| 运动门控 | 外部传入 `allow_measure` |

### 示例 CSV

固定仓库路径：

```text
tests/fixtures/sample_ppg.csv
```

字段：

```text
timestamp_ms, PPG_G1, PPG_G2, PPG_G3, PPG_G4
```

说明：示例 CSV 中没有 `allow_measure` 字段。host smoke test 默认可模拟 `allow_measure = true`；门控测试应通过合成样本或测试参数构造 `allow_measure = false`。

## 4. 输出结果与字段

每检测到一个有效 IBI，输出一次逐搏 IBI event。

已确认输出字段：

| 字段 | 含义 |
|---|---|
| `timestamp_ms` | 当前 IBI event 对应时间戳，建议为后一个有效搏动时间 |
| `ibi_ms` | 当前搏动与上一有效搏动之间的间隔 |
| `confidence` | 当前 IBI 可信度 |
| `signal_quality` | 当前选中通道或融合后的信号质量 |
| `selected_channel` | 当前使用的 PPG 通道 |
| `state` | 当前算法状态 |
| `reject_reason` | 拒绝或状态转换原因 |

当前不输出：

- `hr_bpm`；
- `rmssd`；
- 长期 HRV 指标。

## 5. PPG-IBI 算法范围

本项目算法范围包括：

1. 逐点输入接口；
2. 4 路 PPG 数据接收；
3. `timestamp_ms` 与内部 sample counter 校验；
4. PPG 预处理；
5. SQI 信号质量评估；
6. 主通道选择；
7. 脉搏候选检测；
8. IBI 合法性检查；
9. 外部 `allow_measure` 门控；
10. 状态机；
11. 逐搏 IBI event 输出；
12. host 端 CSV smoke test；
13. 后期人工可视化检验辅助输出。

## 6. 自研算法策略

当前确认：

- 核心算法从零自研；
- 不使用第三方 PPG / IBI / HR / HRV 算法库；
- 不使用 Python 第三方库替代 C 算法；
- 不引入外部峰值检测库；
- 不引入嵌入式第三方信号处理库作为核心实现；
- CMSIS-DSP、厂商 DSP 库或芯片 SDK 特殊数学函数默认不使用；如后续确需使用，作为 S0 决策提交；
- 算法核心语言为 C；
- 可使用 PC host 编译和测试辅助工具，但不得替代 MCU C 算法。

初始模块建议：

1. `input / counter`：输入检查、sample counter、timestamp 校验；
2. `preprocess`：基础滤波、去直流、尺度处理；
3. `sqi`：每通道信号质量评估；
4. `channel_select`：主通道选择；
5. `detector`：脉搏候选检测；
6. `ibi_validate`：IBI 合法性与异常剔除；
7. `state`：状态机；
8. `output`：IBI event 输出；
9. `debug`：必要调试信息。

## 7. MCU 平台与工程约束

| 项目 | 已确认 |
|---|---|
| 目标 MCU | Ambiq Apollo3.5 |
| 架构假设 | Cortex-M4F 级 MCU |
| 是否允许 float | 允许 |
| 是否允许 malloc | 禁止 |
| 是否允许 calloc/realloc | 禁止 |
| RAM 预算 | 小于 20 KB |
| ROM 预算 | 暂不硬性要求 |
| 单次 process 时间 | 暂不硬性要求 |
| MISRA 风格 | 需要 |
| 大数组上栈 | 不允许 |
| 递归 | 不允许 |
| 参数管理 | 集中管理，避免魔法数散落 |
| PC host 测试 | 需要 |

## 8. RAM / ROM / 运行时间预算

- RAM hard target：算法总 RAM 使用小于 20 KB；
- ROM target：TBD；
- process runtime target：TBD；
- 禁止动态内存；
- 后续每个算法模块应记录主要状态量和 buffer 预算。

## 9. 运动门控策略

- 本项目无 ACC 输入；
- 运动状态由外部模块判断；
- 算法通过 `allow_measure` 接收是否允许测量；
- `allow_measure = false` 时，立即停止输出 IBI；
- 恢复 `allow_measure = true` 后，进入 `REACQUIRE`；
- 不在运动期间输出有效 IBI；
- 不默认保留短时 HOLD 输出策略。

## 10. 状态机默认假设

建议初始状态机为：

```text
INIT
ACQUIRE
TRACK
REACQUIRE
INVALID
```

不建议把 `HOLD` 作为核心输出状态，因为 Owner 已确认：`allow_measure = false` 时立即停止输出，恢复后进入 `REACQUIRE`。

## 11. 测试与验收标准

当前由于没有 ECG、人工标注或参考 IBI，只能先做 smoke test 和工程一致性测试。

当前阶段可测试：

1. CSV 可读取；
2. 必要字段存在；
3. 时间戳间隔符合 50 Hz；
4. 4 路 PPG 可逐点输入；
5. 输出字段完整；
6. IBI 基本范围合理；
7. `confidence` / `signal_quality` 字段存在且范围合理；
8. `allow_measure = false` 时不输出；
9. 恢复后进入 `REACQUIRE`；
10. 不使用 `malloc` / `calloc` / `realloc`；
11. RAM 使用符合小于 20 KB 的设计目标。

当前不能证明：

- 算法总体准确性；
- 所有人群适用性；
- 运动场景可靠性；
- 医疗级可靠性；
- RMSSD 结果准确性。

## 12. 示例数据安排

示例 CSV 已允许纳入仓库作为测试 fixture：

```text
tests/fixtures/sample_ppg.csv
```

后续 Codex 任务不得写：

```text
使用 ChatGPT sources 中的 CSV
使用用户上传的 CSV
使用附件里的 CSV
```

必须写成仓库相对路径：

```text
tests/fixtures/sample_ppg.csv
```

## 13. 当前待确认默认假设

这些需要在 M2 冻结：

1. `timestamp_ms` 使用 `uint32_t`；
2. PPG 输入结构体包含 4 路 `int32_t ppg[4]`；
3. `allow_measure` 作为每个样本输入的一部分；
4. 输出 event 只在有效 IBI 出现时返回；
5. `confidence` 使用 0.0–1.0 float；
6. `signal_quality` 使用 0.0–1.0 float；
7. `selected_channel` 使用 0–3；
8. `reject_reason` 使用 enum；
9. 时间戳期望间隔为 20 ms；
10. 如果 timestamp 与 sample counter 明显不一致，优先进入 `REACQUIRE`，不输出 IBI。

## 14. 后置问题

1. 是否需要 debug frame 导出？
2. 是否需要 host 端输出可视化 CSV，供人工检查峰值位置？
3. 是否需要支持不同佩戴形态：手环 / 戒指？
4. 是否需要区分佩戴不良、低灌注、饱和、强噪声等 reject reason？
5. 是否未来支持 25 Hz / 100 Hz？
6. 是否未来加入 ECG 或人工标注作为评价基准？
7. 是否需要输出最近 N 秒内部状态用于固件调试？

## 15. 风险清单

| 风险 | 等级 | 说明 |
|---|---:|---|
| 无 ECG / 人工标注参考 | S2 | 当前只能做 smoke test，不能证明准确性 |
| 无 ACC，仅依赖外部 allow_measure | S2 | 算法自身不能判断运动，只能执行门控 |
| 50 Hz 对 IBI 精度有量化限制 | S2 | 时间分辨率为 20 ms，RMSSD 可能受影响 |
| PPG 峰值受波形形态影响 | S2 | 需要后期人工可视化检验 |
| 4 路通道质量可能差异大 | S2 | 需要 SQI 和主通道选择 |
| timestamp 可能异常 | S1/S2 | 已要求 sample counter 校验 |
| RAM < 20 KB | S1 | 后续实现必须控制 buffer 和状态结构体 |
| MISRA 风格 | S2 | 需要限制动态内存、栈数组、隐式类型转换等 |

## 16. 里程碑草案

| Milestone | 名称 | 目标 |
|---|---|---|
| M0 | 项目启动问诊 | 已完成 |
| M1 | 项目启动包与文档骨架 | 当前文档包与 fixture |
| M2 | IO Contract、MCU API 与资源约束冻结 | 冻结 C API、结构体、状态枚举、reject reason |
| M3 | 输入、sample counter 与时间戳校验 | 实现逐点输入和基础异常检查 |
| M4 | 预处理与基础 SQI | 实现 MCU 可用的基础信号质量评估 |
| M5 | 主通道选择与脉搏候选检测 | 输出候选搏动 |
| M6 | IBI 计算、合法性检查与状态机 | 输出逐搏 IBI event |
| M7 | allow_measure 门控与异常处理 | 完成运动门控、REACQUIRE 逻辑 |
| M8 | host 端 CSV smoke test 与人工检验辅助 | 使用 fixture 生成事件输出 |
| M9 | 资源审查与 MCU 移植准备 | RAM、接口、MISRA 风格、移植风险复盘 |

## 17. 当前不处理的问题

当前暂不处理：

1. 具体滤波器系数；
2. 峰值检测算法细节；
3. SQI 公式；
4. C API 具体代码；
5. 可视化工具；
6. RMSSD 计算；
7. 医疗级验证；
8. MCU 实测性能。
