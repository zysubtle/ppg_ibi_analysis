# 09_CODEX_RUNBOOK.md：Codex 运行与测试说明 v0.1

## Codex 基本指令

Owner 确认进入某个里程碑后，给 Codex 的默认一句话指令是：

```text
请读取 docs/10_CODEX_NEXT_TASK.md，并严格执行。
```

## Codex 必须读取的文件

每轮任务前，Codex 至少应读取：

```text
AGENTS.md
docs/00_PROJECT_BRIEF.md
docs/01_DECISION_LOG.md
docs/02_MILESTONE_PLAN.md
docs/04_IO_CONTRACT.md
docs/06_RESOURCE_BUDGET.md
docs/07_TEST_STRATEGY.md
docs/10_CODEX_NEXT_TASK.md
```

## 示例数据路径

固定路径：

```text
tests/fixtures/sample_ppg.csv
```

不得引用 ChatGPT 附件、sources 或本地绝对路径。

## 当前 M1 状态

M1 仅包含文档和 fixture，尚无源码与构建系统。因此当前没有 `make test` 或 C 编译测试命令。

## 后续测试命令示例

M2/M3 之后可逐步引入：

```text
make test
```

或：

```text
./build/ppg_ibi_eval tests/fixtures/sample_ppg.csv
```

具体命令必须由当轮 `docs/10_CODEX_NEXT_TASK.md` 明确指定。

## Codex 输出摘要要求

每轮完成后请报告：

```text
Summary
Changed files
Test commands
Test results
Known limitations
是否改变 API
是否改变参数
是否引入动态内存
是否引入外部依赖
是否使用 tests/fixtures/sample_ppg.csv
```

## 不足材料处理

如果无法执行测试，必须说明：

- 缺少什么；
- 为什么无法执行；
- 哪些部分已经检查；
- 是否修改了 API、资源约束或依赖。
