# LSM-StorageEngine

> "What I cannot create, I do not understand." — Richard Feynman


**LSM-StorageEngine** 是一个高性能、持久化的键值（Key-Value）存储引擎，基于 **LSM-Tree (Log-Structured Merge-Tree)** 架构设计并完全使用 Modern C++ (C++17) 实现。

本项目旨在剥离 SQL 解析、网络协议等外围逻辑，**深入探索数据库内核最底层的存储与检索机制**。是一个具备工业级核心组件（WAL、MemTable、SSTable、Compaction）的轻量级存储库。

---

##  核心架构

LSM-StorageEngine 遵循经典的 LSM-Tree 读写路径，将随机写转换为顺序写以最大化磁盘吞吐量。

### 1. 存储层 (Storage Layer)
* **MemTable (内存表)**: 
    * 基于 **SkipList (跳表)** 实现，支持 $O(\log n)$ 复杂度的并发写入与读取。
    * 作为读写缓冲，当达到阈值（如 4MB）时转为 Immutable MemTable 并触发刷盘。
* **SSTable (磁盘文件)**:
    * 采用分层结构（Level-0, Level-1...），文件内部有序。
    * 包含数据块（Data Blocks）和索引块（Index Blocks），支持高效的二分查找。

### 2. 可靠性 (Durability & Recovery)
* **WAL (Write-Ahead Log)**:
    * 遵循 "Log is Database" 哲学。所有写入操作在进入内存前，先以 Append-only 模式写入磁盘日志。
    * 支持崩溃恢复（Crash Recovery），重启时自动重放日志恢复数据。

### 3. 性能优化 (Performance)
* **Bloom Filter (布隆过滤器)**:
    * 在内存中快速过滤不存在的 Key，极大减少无效的磁盘 I/O 操作。
* **Level Compaction (层级合并)**:
    * 后台线程自动合并重叠的 SSTable 文件，回收旧版本数据和已删除的数据（GC），维持读取性能。

---

## 快速开始

### 环境要求
* **OS**: Linux / macOS / Windows (WSL2 推荐)
* **Compiler**: GCC 8+ / Clang 10+ / MSVC 2019+ (需支持 C++17)
* **Build System**: CMake 3.10+

### 构建与运行
```bash
# 1. 克隆项目
git clone [https://github.com/your-username/LSM-StorageEngine.git](https://github.com/your-username/LSM-StorageEngine.git)
cd LSM-StorageEngine

# 2. 创建构建目录
mkdir build && cd build

# 3. 编译
cmake ..
make -j4

# 4. 运行示例/测试
./lsm_test


### 使用示例 (API Example)
```cpp
#include "db.h"

int main() {
    LSM::DB* db;
    LSM::Options options;
    options.create_if_missing = true;
    
    // 打开数据库
    LSM::DB::Open(options, "/tmp/testdb", &db);

    // 写入数据
    db->Put("key1", "value1");
    
    // 读取数据
    std::string value;
    db->Get("key1", &value);
    
    // 删除数据
    db->Delete("key1");

    delete db;
    return 0;
}
```

## 技术扩展点 (Advanced Features)

除了基础功能，本项目还涵盖了以下进阶 C++ 工程实践：

- **内存管理**: 使用 **Arena Allocation (内存池)** 优化 SkipList 节点分配，减少 `new`/`delete` 系统调用开销并提升缓存命中率。
- **缓存机制**: 实现 **Sharded LRU Cache**，支持高并发下的 Block 缓存。
- **并发控制**: 使用 `std::shared_mutex` 和 `std::atomic` 实现高效的读写分离。
- **数据校验**: 使用 **CRC32** 校验和，防止磁盘位翻转导致的数据损坏。

## 参考文献

- [BigTable: A Distributed Storage System for Structured Data](https://static.googleusercontent.com/media/research.google.com/en//archive/bigtable-osdi06.pdf)
- [LevelDB Architecture](https://github.com/google/leveldb/blob/main/doc/index.md)
- [RocksDB Wiki](https://github.com/facebook/rocksdb/wiki)
