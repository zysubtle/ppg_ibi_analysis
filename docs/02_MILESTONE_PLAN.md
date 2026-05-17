# 02_MILESTONE_PLAN.md：Milestone Plan v0.1

## M0：项目启动问诊

状态：已完成。

目标：收集生成 Project Brief v0.1 所必需的信息，不写代码，不生成完整设计文档。

## M1：项目启动包与文档骨架

状态：当前完成。

目标：把 Project Brief v0.1、决策、风险、测试 fixture 和 Codex 协作规则沉淀为仓库文件。

本阶段不做：算法源码、最终 C API、滤波器、SQI、峰值检测、准确性验证。

## M2：IO Contract、MCU API 与资源约束冻结

目标：冻结公开 API、输入结构体、输出 event、状态 enum、reject reason enum、配置参数入口和资源预算表达方式。

预期产物：

- `include/ppg_ibi.h` 草案或最终版；
- `include/ppg_ibi_config.h` 草案或最终版；
- 更新 `docs/04_IO_CONTRACT.md`；
- 更新 `docs/06_RESOURCE_BUDGET.md`；
- API 级 host 编译测试计划。

不得做：完整算法实现、复杂滤波、复杂峰值检测、状态机细节优化。

## M3：输入、sample counter 与时间戳校验

目标：实现逐点输入、sample counter、timestamp 校验和基本状态初始化/重置。

## M4：预处理与基础 SQI

目标：实现 MCU 可用的基础预处理和通道质量评估。

## M5：主通道选择与脉搏候选检测

目标：从多通道 PPG 中选择主通道并产生脉搏候选。

## M6：IBI 计算、合法性检查与状态机

目标：输出有效逐搏 IBI event，并处理明显异常 IBI。

## M7：allow_measure 门控与异常处理

目标：实现门控禁止时立即停止输出、恢复后 REACQUIRE、异常状态处理。

## M8：host 端 CSV smoke test 与人工检验辅助

目标：基于 `tests/fixtures/sample_ppg.csv` 生成事件输出，辅助人工检查。

## M9：资源审查与 MCU 移植准备

目标：复盘 RAM、栈、API、MISRA 风格、移植风险和后续 MCU 集成事项。
