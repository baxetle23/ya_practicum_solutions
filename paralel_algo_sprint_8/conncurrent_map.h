#include <cstdlib>
#include <future>
#include <map>
#include <numeric>
#include <random>
#include <string>
#include <vector>
#include <algorithm>
#include <mutex>

template <typename Key, typename Value>
class ConcurrentMap {
public:

    static_assert(std::is_integral_v<Key>, "ConcurrentMap supports only integer keys");

    struct Bucket
    {
        std::map<Key, Value> map;
        std::mutex map_mutex;
    };

    struct Access {
        Access(Bucket& bucket, const Key& key) :  guard(bucket.map_mutex), ref_to_value(bucket.map[key]) {
        }
        std::lock_guard<std::mutex> guard;
        Value& ref_to_value;

    };

    explicit ConcurrentMap(size_t bucket_count) : maps_(bucket_count), count_maps_(bucket_count) {
    }

    Access operator[](const Key& key) {
        auto index = key % count_maps_;
        return {maps_[index], key};
    }

    std::map<Key, Value> BuildOrdinaryMap() {
        std::map<Key, Value> result;
        for(size_t i = 0; i < count_maps_; ++i) {
            std::lock_guard guard(maps_[i].map_mutex);
            result.insert(maps_[i].map.begin(), maps_[i].map.end());
        }
        return result;
    }

private:
    std::vector<Bucket> maps_;
    size_t count_maps_;
};