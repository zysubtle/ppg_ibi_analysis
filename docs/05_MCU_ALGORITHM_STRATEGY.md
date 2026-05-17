# 05_MCU_ALGORITHM_STRATEGY.md：自研 PPG-IBI 算法策略 v0.1

## 总体策略

本项目采用自研 MCU 端 PPG-IBI 算法，不依赖第三方 PPG / IBI / HR / HRV 算法库。

算法设计优先级：

1. 接口稳定；
2. 可在 MCU 逐点运行；
3. RAM 可控；
4. 可在 PC host 复现测试；
5. 状态机可审查；
6. 输出 event 可追踪；
7. 后续便于人工可视化检验。

## 禁止策略

不得：

- 使用 NeuroKit2、HeartPy、BioSPPy、WFDB、SciPy、NumPy 等作为核心算法实现；
- 使用 Python 代码替代 C 算法；
- 引入外部峰值检测库；
- 擅自使用 CMSIS-DSP 或厂商 DSP 库；
- 使用动态内存；
- 把复杂黑盒模型作为 MCU 核心算法；
- 为了测试方便改变输入/输出 API。

## 推荐模块划分

| 模块 | 职责 | M 阶段 |
|---|---|---|
| input/counter | 逐点输入、sample counter、timestamp 校验 | M3 |
| preprocess | 基础去直流、滤波、尺度处理 | M4 |
| sqi | 每通道信号质量评估 | M4 |
| channel_select | 主通道选择 | M5 |
| detector | 脉搏候选检测 | M5 |
| ibi_validate | IBI 合法性检查 | M6 |
| state | 状态机 | M6/M7 |
| output | event 输出 | M6 |
| debug | 调试状态和人工检查辅助 | M8/M9 |

## 当前不冻结的算法细节

以下事项待后续里程碑处理：

- 滤波器类型和系数；
- 峰值检测方法；
- SQI 公式；
- 通道选择评分；
- IBI 合理范围；
- confidence 计算；
- timestamp 异常容忍阈值；
- debug frame 内容。

## 状态机策略

门控禁止时立即停止输出，恢复后进入 `REACQUIRE`。不使用 HOLD 作为输出保持状态。

## 人工可视化检验方向

后续 M8 可生成用于人工检查的 host 输出，例如：

- 每个样本的时间戳；
- 选中通道；
- SQI；
- 候选峰标志；
- event 输出；
- reject reason。

该输出只用于工程验证，不构成医疗准确性证明。
