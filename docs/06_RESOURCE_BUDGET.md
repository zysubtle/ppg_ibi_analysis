# 06_RESOURCE_BUDGET.md：资源预算 v0.1

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

## RAM 管理原则

1. 不使用动态内存。
2. 避免大数组上栈。
3. 算法状态放入 context 结构体。
4. 所有 buffer 长度通过配置宏集中定义。
5. 每个模块在实现后记录自身 RAM 预算。
6. 后续需要估算 context 总大小。

## 建议 RAM 台账格式

| 模块 | 主要 buffer / 状态 | 估算字节 | 状态 |
|---|---|---:|---|
| input/counter | TBD | TBD | M3 |
| preprocess | TBD | TBD | M4 |
| sqi | TBD | TBD | M4 |
| detector | TBD | TBD | M5 |
| state/output | TBD | TBD | M6 |
| debug | TBD | TBD | M8/M9 |
| total | TBD | < 20 KB | 待审查 |

## 栈使用约束

- 不在函数内部定义大数组；
- 不使用递归；
- 临时变量数量受控；
- 后续测试或静态审查应关注 stack usage。

## ROM / 运行时间

当前不硬性约束，但不得实现明显不适合 MCU 的高复杂度算法。

后续如需增加硬性限制，必须作为 S0 决策记录。
