# 03_ALGORITHM_SCOPE.md：Algorithm Scope v0.1

## 当前算法范围

本项目只关注从 PPG 输出逐搏 IBI event。

包含：

1. 逐点输入；
2. 4 路同步 PPG；
3. `timestamp_ms` 校验；
4. sample counter；
5. 外部 `allow_measure` 门控；
6. 预处理；
7. SQI；
8. 主通道选择；
9. 脉搏候选检测；
10. IBI 合法性判断；
11. 状态机；
12. event 输出；
13. host 端 smoke test；
14. 后期人工可视化辅助。

## 当前不包含

1. RMSSD 计算；
2. 长期 HRV 指标；
3. 实时 HR 输出；
4. ACC 运动识别；
5. 医疗诊断结论；
6. 第三方算法库；
7. ECG 参考误差评价；
8. 医疗级准确性证明。

## 算法输入

- 采样率：50 Hz；
- PPG：4 路，`int32_t`，signed，24 bit；
- timestamp：`timestamp_ms`；
- 门控：`allow_measure`；
- ACC：无。

## 算法输出

有效 IBI event：

- `timestamp_ms`；
- `ibi_ms`；
- `confidence`；
- `signal_quality`；
- `selected_channel`；
- `state`；
- `reject_reason`。

## 拒绝输出场景

至少包括：

- `allow_measure = false`；
- 信号质量明显不足；
- 时间戳异常；
- 尚未完成 ACQUIRE / REACQUIRE；
- IBI 明显超出生理合理范围；
- 内部状态无效。

具体 reject reason 在 M2 冻结。
