# 11_M1_ACCEPTANCE_REPORT.md：M1 验收报告

## M1 目标

生成项目启动包与文档骨架，把 Project Brief v0.1、决策、风险、测试 fixture 和 Codex 协作规则沉淀为仓库文件。

## M1 产物

已生成：

- `README.md`
- `AGENTS.md`
- `docs/00_PROJECT_BRIEF.md`
- `docs/01_DECISION_LOG.md`
- `docs/02_MILESTONE_PLAN.md`
- `docs/03_ALGORITHM_SCOPE.md`
- `docs/04_IO_CONTRACT.md`
- `docs/05_MCU_ALGORITHM_STRATEGY.md`
- `docs/06_RESOURCE_BUDGET.md`
- `docs/07_TEST_STRATEGY.md`
- `docs/08_RISK_REVIEW.md`
- `docs/09_CODEX_RUNBOOK.md`
- `docs/10_CODEX_NEXT_TASK.md`
- `tests/fixtures/sample_ppg.csv`

## fixture 检查

| 项目 | 结果 |
|---|---|
| 路径 | `tests/fixtures/sample_ppg.csv` |
| 字段 | `timestamp_ms`, `PPG_G1`, `PPG_G2`, `PPG_G3`, `PPG_G4` |
| 行数 | 25001 |
| 时间范围 | 0 ms 到 500000 ms |
| 时间戳步长 | 20 ms |
| 采样率 | 50 Hz |
| SHA256 | `fd8e84f361676ecaba9343eb24723044fb47bb1f18dc14513723388b1c33f900` |

## 本阶段未做

- 未写完整算法源码；
- 未冻结最终 C API；
- 未实现滤波、SQI、峰值检测或 IBI 计算；
- 未做准确性验证；
- 未引入第三方库；
- 未使用动态内存。

## 验收结论

M1 文档包和 fixture 已满足进入 M2 的前置条件。

M2 前需要 Owner 确认：是否进入 M2，并由 Codex 按 `docs/10_CODEX_NEXT_TASK.md` 执行 API/IO Contract 冻结任务。
