# Toy DataBase 🧸


> **"What I cannot create, I do not understand."** — Richard Feynman
> (我无法创造的东西，我就不理解。——理查德·费曼)

**Toy DataBase** 是一个从零开始用 C++ 编写的轻量级、持久化键值（Key-Value）存储引擎。

与复杂的商业数据库不同，Toy DataBase 剥离了 SQL 解析器、网络层等复杂逻辑，只专注于 **核心存储引擎架构** 的实现。它的设计初衷是**“代码简单到能在一个下午读完”**，但其功能足以演示现代数据库（如 LevelDB 或 RocksDB）是如何在磁盘上可靠地存储数据的。

## 项目目标

- **数据持久化 (Persistence)**: 确保程序退出后数据不丢失 (内存 → 磁盘)。
- **崩溃可靠性 (Durability)**: 即使发生断电或进程崩溃，也能通过日志保证数据安全 (WAL)。
- **核心架构 (Architecture)**: 实现简化版的 **LSM-Tree** (Log-Structured Merge-Tree) 存储结构。

## 功能特性 & 架构 (硬核部分)

本项目旨在实现工业级存储引擎的核心组件：

### 1. 存储引擎 (LSM-Tree 架构)
- [ ] **MemTable (内存表)**: 基于内存的数据结构（计划使用 `std::map` 或跳表 SkipList），提供极快的写入速度。
- [ ] **SSTable (有序字符串表)**: 不可变（Immutable）的磁盘文件格式，用于高效的数据持久化。
- [ ] **Level Compaction (层级合并)**: 后台线程自动合并多个 SSTable 文件并清理无效数据（垃圾回收机制）。

### 2. 可靠性与恢复
- [ ] **WAL (预写日志)**: 采用 Append-only（追加写）模式记录日志，确保崩溃后的数据持久性。
- [ ] **Crash Recovery (崩溃恢复)**: 数据库启动时自动重放 WAL 日志，将数据库恢复到崩溃前的状态。

### 3. 性能优化
- [ ] **Bloom Filters (布隆过滤器)**: 概率型数据结构，用于快速判断 Key 是否不存在，减少无效的磁盘 I/O。
- [ ] **Block Cache (块缓存)**: 使用 LRU 策略缓存频繁访问的数据块。

### 4. 并发控制 (可选)
- [ ] **Thread Safety (线程安全)**: 使用 `std::shared_mutex` 实现“单写多读”的并发控制。

## 快速开始

### 环境要求
- C++ 编译器 (GCC 8+ 或 Clang 10+)，需支持 **C++17**
- CMake (3.10+)

### 构建项目
```bash
git clone [https://github.com/your-username/ToyDataBase.git](https://github.com/your-username/ToyDataBase.git)
cd ToyDataBase
mkdir build && cd build
cmake ..
make
