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
public:
	//所以加 explicit 表达的 API 意图是： 
	//你可以用 128 来创建容量为 128 的 LRU cache，但我不允许编译器在任何需要 LRUCache 的地方，把 128 悄悄解释成一个缓存。
    explicit LRUCache(std::size_t capacity): capacity_(capacity) {}

    /*
        访问一个 key。

        返回 true  : cache hit
        返回 false : cache miss

        注意：这个最小示例只缓存 key，不保存真正的数据 value。
        重点是展示 LRU replacement policy。
    */
    bool access(const std::string& key) {
        // 1. 查询 key 是否已经在 cache 中。
        auto it = positions_.find(key);

        if (it != positions_.end()) {
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
            order_.splice(order_.begin(), order_, it->second);

            // splice 后，该元素已在 list 的 begin()，更新 map 中 iterator。
            it->second = order_.begin();

            return true;
        }

        // ---------------- Cache miss ----------------
        //
        // key 不在 cache 内。如果 cache 已满，驱逐最久未使用的元素。
        if (order_.size() == capacity_) {
            // list 尾部是最久未使用的 key。
            const std::string& lruKey = order_.back();

            // map 里也必须删除它，否则 map 会保存无效 iterator。
            positions_.erase(lruKey);

            // 从访问顺序表中删除 LRU key。
            order_.pop_back();
        }

        // 新 key 被访问，因此插到最前面，成为 MRU。
        order_.push_front(key);

        // 记录该 key 在 list 中的位置，以支持后续 O(1) 查找。
        positions_[key] = order_.begin();

        return false;
    }

    void print() const {
        std::cout << "Cache [MRU -> LRU]: ";
        for (const std::string& key : order_) std::cout << key << ' ';
        std::cout << '\n';
    }

private:
    std::size_t capacity_;

    /*
        保存 cache entry 的使用顺序。
        示例： A B C
        表示：
            A 是最近使用的
            C 是最久未使用的
    */
    std::list<std::string> order_;

    /*
        key -> key 在 order_ 中的位置
        有了它，就不必每次 hit 时 O(N) 遍历 list 来找 key；
        可以 O(1) 找到，再 O(1) 移动到 list 头部。
    */
    std::unordered_map<std::string, std::list<std::string>::iterator> positions_;
};

int main() {
    LRUCache cache(3);

    const std::string accesses[] = {
        "A", "B", "C",
        "A",
        "D",
        "C",
        "E"
    };

    for (const std::string& key : accesses) {
        const bool hit = cache.access(key);
        std::cout << "access(" << key << "): " << (hit ? "HIT" : "MISS") << '\n';
        cache.print();
    }
}