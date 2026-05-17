# 06_RESOURCE_BUDGET.md：资源预算 v0.2（M2 冻结）

## 已确认资源约束

| 项目 | 约束 |
|---|---|
| MCU | Ambiq Apollo3.5 / Cortex-M4F 级 |
| RAM | 算法总 RAM < 20 KB |
| ROM | 暂不硬性要求 |
| 单次 process 时间 | 暂不硬性要求 |
| float | 允许 |
| malloc/calloc/realloc | 禁止 |
| MISRA 风格 | 需要 |

## M2 API 骨架 RAM 台账

M2 只引入 API context、少量状态字段和计数器，不引入滤波、SQI、峰检或 IBI buffer。

| 模块 | 主要 buffer / 状态 | 估算字节 | 状态 |
|---|---|---:|---|
| config | `sample_rate_hz`, `sample_period_ms`, `timestamp_tolerance_ms` | < 16 | M2 冻结 |
| input/counter | `sample_count`, `last_timestamp_ms`, timestamp flag | < 16 | M2 骨架，M3 完成校验 |
| state/output | `state`, `last_reject_reason`, gate flag | < 16 | M2 骨架，M6/M7 完成逻辑 |
| preprocess | 无 | 0 | M4 |
| sqi | 无 | 0 | M4 |
| detector | 无 | 0 | M5 |
| debug | 无 | 0 | M8/M9 |
| total | `ppg_ibi_context_t` 当前仅含标量字段 | < 64 | 满足 < 20 KB |

说明：`enum` 大小与编译器 ABI 有关，因此文档只冻结预算级别；`make test` 使用 `sizeof(ppg_ibi_context_t) < PPG_IBI_RAM_BUDGET_BYTES` 做编译测试。

## RAM 管理原则

1. 不使用动态内存。
2. 避免大数组上栈。
3. 算法状态放入 `ppg_ibi_context_t`。
4. 所有 buffer 长度通过配置宏集中定义。
5. 每个后续算法模块实现后必须更新 RAM 台账。
6. 后续若接近 20 KB hard target，必须拆分模块预算并复审。

## 栈使用约束

- 不在函数内部定义大数组；
- 不使用递归；
- 临时变量数量受控；
- 后续测试或静态审查应关注 stack usage。

## ROM / 运行时间

当前不硬性约束，但不得实现明显不适合 MCU 的高复杂度算法。

后续如需增加硬性限制，必须作为 S0 决策记录。
