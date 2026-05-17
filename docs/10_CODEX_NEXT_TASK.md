# 10_CODEX_NEXT_TASK.md：Codex 当前任务文件

## 任务状态

本文件为下一轮 M2 任务草案。只有 Owner / Architect 明确确认“进入 M2”后，Codex 才应执行。

## 当前里程碑

M2：IO Contract、MCU API 与资源约束冻结。

## 本轮目标

在不实现完整算法的前提下，建立 MCU C API 草案/骨架，并把 IO Contract 与资源约束冻结到可审查状态。

## 本轮非目标

不得实现：

1. 复杂滤波；
2. SQI 公式；
3. 主通道选择算法；
4. 脉搏峰检测算法；
5. IBI 计算算法；
6. 状态机完整逻辑；
7. host CSV 评估工具完整算法；
8. RMSSD 或 HRV 指标。

## 允许修改范围

建议允许：

```text
include/ppg_ibi.h
include/ppg_ibi_config.h
src/ppg_ibi.c
src/ppg_ibi_internal.h
tests/test_api_compile.c
Makefile
docs/04_IO_CONTRACT.md
docs/06_RESOURCE_BUDGET.md
docs/08_RISK_REVIEW.md
```

如果仓库尚无 `include/`、`src/`、`tests/`，可以创建。

## 不允许修改范围

不得：

1. 改变 Project Brief 已确认的输入/输出字段；
2. 改变采样率 50 Hz；
3. 改变 4 路 PPG、signed `int32_t`、24 bit 条件；
4. 改变 `allow_measure` 门控策略；
5. 改变目标 MCU；
6. 引入任何第三方算法库；
7. 使用 `malloc` / `calloc` / `realloc`；
8. 输出 HR 或 RMSSD；
9. 删除 `tests/fixtures/sample_ppg.csv`；
10. 把 Python 脚本当作算法实现。

## API 设计要求

请设计最小 MCU API，至少包含：

1. context 类型；
2. config 类型；
3. input sample 类型；
4. output event 类型；
5. init/reset/process 函数；
6. 状态 enum；
7. reject reason enum；
8. 版本宏或版本函数。

要求：

- context 由调用者分配；
- 不使用动态内存；
- `process` 逐点输入；
- 通过返回值或 out 参数明确本次是否产生新 IBI event；
- event 字段必须包含：`timestamp_ms`, `ibi_ms`, `confidence`, `signal_quality`, `selected_channel`, `state`, `reject_reason`；
- API 中不得输出 `hr_bpm` 或 `rmssd`。

## 测试要求

如果本轮创建 C API 和 Makefile，应至少提供 API 编译测试：

```text
make test
```

测试至少验证：

1. header 可编译；
2. init/reset/process 可链接；
3. `ppg_ibi_process()` 在空实现/占位实现下不会崩溃；
4. 不使用动态内存；
5. 输出结构体字段存在；
6. `tests/fixtures/sample_ppg.csv` 路径保留。

## 通过标准

本轮通过需满足：

1. 未引入第三方核心算法库；
2. 未使用动态内存；
3. 未改变 Project Brief 已确认字段；
4. C API 清晰，适合 MCU；
5. 文档与 header 一致；
6. 测试命令通过，或明确说明尚无构建系统的原因；
7. 所有新增实现都保持算法占位，不抢做完整 PPG-IBI 检测。

## 失败时必须报告

如果不能完成，请报告：

- 哪些文件未能生成；
- 哪些测试未能运行；
- 是否遇到 API 决策冲突；
- 是否需要 Owner 做 S0 决策；
- 是否意外需要外部库或动态内存。
