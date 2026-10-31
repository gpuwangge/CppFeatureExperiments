#include <iostream>
#include <list>
#include <string>
#include <unordered_map>

//LRU（Least Recently Used，最近最少使用）是一种缓存淘汰策略：当缓存满了时，优先删除最久没有被访问的数据项。
//要点
// 1. 使用 std::list 保存 cache entry 的使用顺序。(push_front, pop_back)
// 2. 使用 std::unordered_map 保存 key -> key 在 list 中的位置。
// 3. 使用 std::list::splice 可以把一个已有节点移动到指定位置，不复制字符串，不重新分配该节点。
// 4. 只需要实现access()方法，返回 cache hit 或 miss，并在 cache miss 时驱逐最久未使用的元素。
// 5. 打印当前状态时，输出从左到右的顺序为：MRU -> LRU。

class LRUCache {
private:
    std::size_t m_capacity;

    /*
        保存 cache entry 的使用顺序。
        示例： A B C
        表示：
            A 是最近使用的
            C 是最久未使用的
    */
    //std::list<std::string> m_order;
    std::list<std::pair<std::string, int>> m_entry; //key,value

    /*
        key -> 对应 entry 在 m_entry 中的位置
        有了它，就不必每次 hit 时 O(N) 遍历 list 来找 key；
        可以 O(1) 找到，再 O(1) 移动到 list 头部。
    */
    std::unordered_map<std::string, std::list<std::pair<std::string, int>>::iterator> m_positions;


public:
	//所以加 explicit 表达的 API 意图是： 
	//你可以用 128 来创建容量为 128 的 LRU cache，但我不允许编译器在任何需要 LRUCache 的地方，把 128 悄悄解释成一个缓存。
    explicit LRUCache(std::size_t capacity): m_capacity(capacity) {}

    LRUCache(const LRUCache&) = delete;
    LRUCache& operator=(const LRUCache&) = delete;

    /*
        访问一个 key。
        命中：只更新 LRU 顺序，忽略传入的 value。
        未命中：插入 {key, value}，必要时淘汰。
        返回 true  : cache hit
        返回 false : cache miss
    */
    bool access(const std::string& key, int value) {
        if (m_capacity == 0) return false; // 永远 miss，也不保存任何 entry


        // 1. 查询 key 是否已经在 cache 中。
        auto it = m_positions.find(key);

        if (it != m_positions.end()) {
            // ---------------- Cache hit ----------------
            //
            // key 已存在。它刚被使用，所以不应再保持原来的旧位置。
            //
            // list 从前到后：
            //   front = 最近使用（MRU）
            //   back  = 最久未使用（LRU）
            //
            // std::list::splice 可以把一个已有节点移动到指定位置，
            // 不复制字符串，不重新分配该节点。
            // 目标list.splice(插入位置, 来源list, 要转移的节点iterator);
            m_entry.splice(m_entry.begin(), m_entry, it->second);

            // splice 后，该元素已在 list 的 begin()，更新 map 中 iterator。
            //it->second = m_entry.begin(); //多余
            // splice 不会使该节点的 iterator 失效，无需更新 m_positions。

            return true;
        }

        // ---------------- Cache miss ----------------
        //
        // key 不在 cache 内。如果 cache 已满，驱逐最久未使用的元素。
        if (m_entry.size() == m_capacity) {
            // list 尾部是最久未使用的 key。
            const std::string& lruKey = m_entry.back().first;
            //std::pair<std::string, int> &lruEntry = m_entry.back();

            // map 里也必须删除它，否则 map 会保存无效 iterator。
            m_positions.erase(lruKey);

            // 从访问顺序表中删除 LRU key。
            m_entry.pop_back();
        }

        // 新 key 被访问，因此插到最前面，成为 MRU。
        //m_entry.push_front(std::make_pair(key,value));
        m_entry.emplace_front(key, value);//原地构造元素

        // 记录该 key 在 list 中的位置，以支持后续 O(1) 查找。
        m_positions[key] = m_entry.begin();

        return false;
    }

    void print() const {
        std::cout << "Cache [MRU -> LRU]: ";
        for (const auto& [key, value] : m_entry) std::cout << '{' << key << ": " << value << "} ";
        std::cout << '\n';
    }

};

int main() {
    LRUCache cache(2);

    cache.access("A", 100); // MISS
    cache.access("B", 200); // MISS
    cache.access("A", 999); // HIT：A 的 value 仍然是 100
    cache.access("C", 300); // MISS：淘汰 B

    cache.print();
    // Cache [MRU -> LRU]: {C: 300} {A: 100}
}



/*
| 操作               | 结果         | 缓存状态：MRU → LRU    |
| ---------------- | ---------- | ----------------- |
| access("A", 100) | MISS       | {A: 100}          |
| access("B", 200) | MISS       | {B: 200} {A: 100} |
| access("A", 999) | HIT，忽略 999 | {A: 100} {B: 200} |
| access("C", 300) | MISS，淘汰 B  | {C: 300} {A: 100} |
*/