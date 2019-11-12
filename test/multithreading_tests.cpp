#include "cache.hpp"
#include "cache_policy.hpp"
#include "fifo_cache_policy.hpp"
#include "lfu_cache_policy.hpp"
#include "lru_cache_policy.hpp"

#include <gtest/gtest.h>

template <typename Key, typename Value>
using lfu_cache_t =
    typename caches::fixed_sized_cache<Key, Value, caches::LFUCachePolicy<Key>>;

template <typename Key, typename Value>
using lru_cache_t =
    typename caches::fixed_sized_cache<Key, Value, caches::LRUCachePolicy<Key>>;

template <typename Key, typename Value>
using fifo_cache_t =
    typename caches::fixed_sized_cache<Key, Value,
                                       caches::FIFOCachePolicy<Key>>;

template <typename Key, typename Value>
using nopolicy_cache_t = typename caches::fixed_sized_cache<Key, Value>;

template <class Policy> class CacheTest : public ::testing::Test
{
};
typedef ::testing::Types<lfu_cache_t<int, size_t>, lru_cache_t<int, size_t>,
                         fifo_cache_t<int, size_t>,
                         nopolicy_cache_t<int, size_t>>
    CacheTypes;

TYPED_TEST_CASE(CacheTest, CacheTypes);

TYPED_TEST(CacheTest, Multithreaded)
{
    TypeParam cache(10);

#pragma omp parallel for num_threads(4)
    for (size_t i = 0; i < 1000000; ++i)
    {
        try
        {
            cache.Get(i % 100);
        }
        catch (...)
        {
            cache.Put(i % 100, i);
        }
    }
}
