# 01_DECISION_LOG.md：Decision Log v0.1

| ID | 决策 | 等级 | 状态 | 说明 |
|---|---|---:|---|---|
| D001 | IBI 仅用于 HRV / RMSSD | S0 | Owner 已确认 | 算法只输出逐搏 IBI，不内部计算 RMSSD |
| D002 | 当前不作为医疗诊断软件 | S0 | Owner 已确认 | 仅研究、工程验证、可穿戴健康监测算法原型 |
| D003 | PPG 为 4 路绿光，50 Hz | S0 | Owner 已确认 | 多通道严格同步 |
| D004 | PPG raw 为 signed `int32_t`，24 bit | S0 | Owner 已确认 | MCU 输入规格 |
| D005 | 逐点输入，每个样本带 `timestamp_ms` | S0 | Owner 已确认 | 内部 sample counter 用于校验时间戳错误 |
| D006 | 无 ACC | S0 | Owner 已确认 | 运动判断不由本算法内部完成 |
| D007 | 运动门控由外部 `allow_measure` 提供 | S0 | Owner 已确认 | `false` 时立即停止输出 |
| D008 | 恢复允许测量后进入 `REACQUIRE` | S0 | Owner 已确认 | 不采用短时 HOLD 输出 |
| D009 | 目标 MCU 为 Ambiq Apollo3.5 | S0 | Owner 已确认 | Cortex-M4F 级 MCU 假设 |
| D010 | RAM 小于 20 KB | S0 | Owner 已确认 | ROM 与单次 process 时间暂不硬性要求 |
| D011 | 允许 `float` | S0 | Owner 已确认 | 后续仍需关注 MCU 资源 |
| D012 | 禁止 `malloc` / `calloc` / `realloc` | S0 | Owner 已确认 | 禁止动态内存 |
| D013 | 需要 MISRA 风格限制 | S0 | Owner 已确认 | 后续实现需避免常见嵌入式风险 |
| D014 | 输出字段为 7 个核心字段 | S0 | Owner 已确认 | `timestamp_ms`, `ibi_ms`, `confidence`, `signal_quality`, `selected_channel`, `state`, `reject_reason` |
| D015 | 当前无 ECG / 人工标注 / 参考 IBI | S2 | Owner 已确认 | 后期做人工可视化检验 |
| D016 | 示例 CSV 允许纳入仓库 fixture | S0 | Owner 已确认 | 固定路径 `tests/fixtures/sample_ppg.csv` |
| D017 | 当前不使用第三方 PPG/IBI/HR/HRV 算法库 | S0 | Project Brief 已确认 | 如需外部依赖必须 S0 决策 |

## 待 M2 冻结事项

1. C 公开 API 名称；
2. 输入结构体字段类型；
3. 输出 event 结构体字段类型；
4. 状态 enum；
5. reject reason enum；
6. timestamp 异常处理阈值；
7. confidence / signal_quality 数值范围和语义；
8. 是否需要 debug frame。
