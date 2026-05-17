# AGENTS.md：PPG-IBI MCU 项目协作规则

## 项目角色

- Owner：确认目标、约束、S0 决策和里程碑推进。
- Architect / Reviewer：维护项目规格、任务文件、验收标准和审查结论。
- Runner / Codex：只按 `docs/10_CODEX_NEXT_TASK.md` 执行，不得擅自扩展范围。

## 必须遵守的硬约束

1. 核心算法使用 C 语言，面向 MCU。
2. 不得引入第三方 PPG / IBI / HR / HRV 算法库。
3. 不得用 Python、NumPy、SciPy、NeuroKit2、HeartPy、BioSPPy、WFDB 等替代 C 算法实现。
4. 不得擅自引入 CMSIS-DSP、厂商 DSP 库、芯片 SDK 特殊数学函数或外部依赖；如确需使用，必须作为 S0 事项提交 Owner 决策。
5. 禁止使用 `malloc`、`calloc`、`realloc` 或动态容器。
6. 避免大数组上栈；算法状态和 buffer 应显式放入上下文结构体或静态/外部分配对象。
7. 允许 `float`，但不得因此忽略 MCU 资源约束。
8. 公开 API、输入字段、输出字段、采样率、状态机主策略、资源预算不得擅自改变。
9. 示例数据必须使用仓库相对路径：`tests/fixtures/sample_ppg.csv`。
10. 当前项目不是医疗诊断软件，不得输出医疗诊断结论。

## S0 事项

以下变更必须停止执行并提交 Owner 决策：

- 改变核心算法策略或 PPG-IBI 主流程；
- 改变输入 API 或输出 API；
- 改变结构体字段；
- 改变采样率；
- 改变 MCU 平台；
- 改变 RAM / ROM / 运行时间 / 功耗约束；
- 改变是否允许 float；
- 改变是否允许动态内存；
- 引入外部库、DSP 库或 Python 第三方库；
- 将研究 / 工程工具扩展为医疗诊断用途。

## Codex 执行要求

每轮执行后，必须报告：

- Summary；
- Changed files；
- Test commands；
- Test results；
- Known limitations；
- 是否改变 API；
- 是否引入动态内存；
- 是否引入外部依赖；
- 是否修改资源约束；
- 是否使用 `tests/fixtures/sample_ppg.csv` 完成要求的测试。
