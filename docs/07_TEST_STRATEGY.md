# 07_TEST_STRATEGY.md：测试策略 v0.1

## 测试目标

当前测试目标是工程 smoke test 和接口一致性测试，不是准确性证明。

## 示例数据 fixture

固定路径：

```text
tests/fixtures/sample_ppg.csv
```

字段：

```text
timestamp_ms, PPG_G1, PPG_G2, PPG_G3, PPG_G4
```

初步检查：

| 项目 | 结果 |
|---|---|
| 行数 | 25001 |
| 时间范围 | 0 ms 到 500000 ms |
| 时间戳步长 | 20 ms |
| 采样率 | 50 Hz |
| PPG 通道 | 4 路 |
| 是否包含 ACC | 否 |
| 是否包含 allow_measure | 否 |
| 是否包含 ECG / 参考 IBI | 否 |
| SHA256 | `fd8e84f361676ecaba9343eb24723044fb47bb1f18dc14513723388b1c33f900` |

通道范围：

| 通道 | min | max |
|---|---:|---:|
| PPG_G1 | 3251182 | 3881597 |
| PPG_G2 | 3643219 | 4312345 |
| PPG_G3 | 3495489 | 4078891 |
| PPG_G4 | 3091484 | 3677423 |

## 当前可验收内容

1. 文件可读取；
2. 字段完整；
3. 时间戳严格 20 ms 递增；
4. 4 路 PPG 可逐点输入；
5. 输出 event 字段完整；
6. IBI 基本范围合理；
7. 门控禁止时不输出；
8. 恢复后进入 `REACQUIRE`；
9. 不使用动态内存；
10. 不引入第三方核心算法库。

## 当前不可验收内容

当前不能证明：

- PPG-IBI 总体准确性；
- RMSSD 准确性；
- 医疗级可靠性；
- 运动场景准确性；
- 不同人群泛化能力。

## 后续建议测试层次

### M2：API 级测试

- header 可被 C 编译器包含；
- 结构体大小可检查；
- enum 和字段符合文档；
- 不引入动态内存。

### M3：输入与时间戳测试

- 正常 20 ms 输入；
- 跳点 / 重复 timestamp；
- sample counter 与 timestamp 不一致；
- reset 后状态恢复。

### M4–M6：算法 smoke test

- 可用 fixture 逐点处理；
- 输出 IBI event；
- IBI 范围合理；
- 输出字段完整；
- `selected_channel` 合法。

### M7：门控测试

- `allow_measure = false` 立即停止输出；
- 恢复后进入 `REACQUIRE`；
- 禁止期间不产生有效 IBI。

### M8：人工可视化检验

- 输出候选峰和 event 表；
- 支持人工检查峰位置和 IBI 合理性。
