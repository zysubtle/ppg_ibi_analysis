# 08_RISK_REVIEW.md：风险记录 v0.2（M2 更新）

| ID | 风险 | 等级 | 状态 | 处理建议 |
|---|---|---:|---|---|
| R001 | 无 ECG / 人工标注参考 | S2 | 已记录 | 当前只能 smoke test，后续做人工可视化检验 |
| R002 | 无 ACC，仅依赖外部 `allow_measure` | S2 | 已记录 | 算法不声称能自行判断运动；API 已冻结每样本 `allow_measure` |
| R003 | 50 Hz 时间分辨率为 20 ms | S2 | 已记录 | M2 已冻结 50 Hz / 20 ms；后续评估对 HRV / RMSSD 的影响 |
| R004 | PPG 波形峰值受形态和噪声影响 | S2 | 已记录 | 后续加入 SQI、通道选择、人工检验 |
| R005 | 4 路通道质量可能差异大 | S2 | 已记录 | 需要主通道选择和质量评价；M2 不实现算法 |
| R006 | timestamp 可能异常 | S1/S2 | 已记录 | M2 已冻结 `PPG_IBI_TIMESTAMP_TOLERANCE_MS = 2`；M3 实现 sample counter 校验 |
| R007 | RAM < 20 KB | S1 | M2 已建账 | 当前 `ppg_ibi_context_t` 仅含标量字段，测试检查小于 `PPG_IBI_RAM_BUDGET_BYTES` |
| R008 | MISRA 风格限制 | S2 | 已记录 | 禁止动态内存、大数组上栈、隐式不安全转换；后续需更严格静态审查 |
| R009 | Codex 可能擅自引入第三方库 | S0/S1 | 已记录 | AGENTS.md 和任务文件明确禁止；M2 未引入外部依赖 |
| R010 | 单个 fixture 不能代表准确性 | S2 | 已记录 | 仅作为 smoke test，不作为准确性证明 |
| R011 | M2 API 冻结后发生字段漂移 | S0/S1 | 新增 | 后续改变公开 API、输入/输出字段、采样率或资源约束必须走 S0 |

## 风险分级说明

- S0：必须 Owner 决策；
- S1：阻塞实现，但 Architect 可给 Codex 修复任务；
- S2：技术风险，记录但不阻塞当前里程碑；
- S3：风格、注释、命名、轻微文档问题。

## M2 风险结论

M2 只建立 MCU C API 骨架和编译测试，未实现完整 PPG-IBI 检测，因此不能证明准确性。当前主要收益是把 IO Contract、状态枚举、reject reason、无动态内存策略和 RAM 台账固定到可审查状态。
