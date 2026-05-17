# PPG-IBI MCU 项目启动包 M1

本包用于启动嵌入式 MCU 端 PPG-IBI 自研算法项目。当前 M1 只沉淀文档、规则、风险和测试 fixture，不包含算法源码实现。

## 当前阶段

- 已确认：Project Brief v0.1。
- 当前里程碑：M1，项目启动包与文档骨架。
- 下一建议里程碑：M2，IO Contract、MCU API 与资源约束冻结。

## 目录

```text
AGENTS.md
README.md
docs/
  00_PROJECT_BRIEF.md
  01_DECISION_LOG.md
  02_MILESTONE_PLAN.md
  03_ALGORITHM_SCOPE.md
  04_IO_CONTRACT.md
  05_MCU_ALGORITHM_STRATEGY.md
  06_RESOURCE_BUDGET.md
  07_TEST_STRATEGY.md
  08_RISK_REVIEW.md
  09_CODEX_RUNBOOK.md
  10_CODEX_NEXT_TASK.md
  11_M1_ACCEPTANCE_REPORT.md
tests/
  fixtures/
    sample_ppg.csv
```

## 重要约束

- 核心算法使用 C 语言，面向 MCU。
- 当前不使用第三方 PPG / IBI / HR / HRV 算法库。
- 默认不使用 CMSIS-DSP、厂商 DSP 库或 Python 第三方库作为核心算法依赖。
- 禁止 `malloc` / `calloc` / `realloc`。
- 允许 `float`。
- 目标 RAM：算法总 RAM 小于 20 KB。
- 当前仅作为研究、工程验证和可穿戴健康监测算法原型，不作为医疗诊断软件。

## 示例数据

固定测试 fixture：

```text
tests/fixtures/sample_ppg.csv
```

字段：

```text
timestamp_ms, PPG_G1, PPG_G2, PPG_G3, PPG_G4
```

CSV 初步检查：

- 行数：25001
- 时间范围：0 ms 到 500000 ms
- 时间戳步长：20 ms，共 25000 个间隔
- 采样率：50 Hz
- SHA256：`fd8e84f361676ecaba9343eb24723044fb47bb1f18dc14513723388b1c33f900`

## 给 Codex 的一句话指令

Owner 确认进入 M2 后，可给 Codex：

```text
请读取 docs/10_CODEX_NEXT_TASK.md，并严格执行。
```
