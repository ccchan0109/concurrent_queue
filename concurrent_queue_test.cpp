#include <gtest/gtest.h>
#include "concurrent_queue.h"

TEST(ConcurrentQueueTest, Basic)
{
    ConcurrentQueue<int> q;

    EXPECT_TRUE(q.empty());

    q.push(1);
    q.push(2);
    q.push(3);

    EXPECT_FALSE(q.empty());
    EXPECT_EQ(q.size(), 3);
    EXPECT_EQ(q.front(), 1);
    EXPECT_EQ(q.back(), 3);

    q.pop();

    EXPECT_EQ(q.size(), 2);
    EXPECT_EQ(q.front(), 2);
    EXPECT_EQ(q.back(), 3);

    q.back() = 8;

    EXPECT_EQ(q.size(), 2);
    EXPECT_EQ(q.front(), 2);
    EXPECT_EQ(q.back(), 8);
}

TEST(ConcurrentQueueTest, Emplace)
{
    ConcurrentQueue<std::pair<int, std::string>> q;

    q.emplace(1, "Taiwan");

    EXPECT_EQ(q.size(), 1);

    using pair_t = typename std::pair<int, std::string>;
    EXPECT_EQ(q.front(), pair_t(1, "Taiwan"));
    EXPECT_NE(q.front(), pair_t(2, "Taiwan"));
}

TEST(ConcurrentQueueTest, ConcurrentAccess) {
    ConcurrentQueue<int> q;
    const int num_threads = 100;
    const int num_ops_per_thread = 1000;

    // Define worker function for threads
    auto fn_worker_push = [&](int id) {
        for (int i = 0; i < num_ops_per_thread; ++i) {
            q.push(id*num_ops_per_thread + i);
        }
    };

    // Create threads
    std::vector<std::thread> threads;
    for (int i = 0; i < num_threads; ++i) {
        threads.emplace_back(fn_worker_push, i);
    }

    // Wait for threads to finish
    for (auto &t : threads) {
        t.join();
    }

    // Check queue size
    ASSERT_EQ(q.size(), num_threads * num_ops_per_thread);

    // Define worker function for threads
    auto fn_worker_pop = [&]([[maybe_unused]] int id) {
        for (int i = 0; i < num_ops_per_thread; ++i) {
            q.pop();
        }
    };

    // Create threads
    threads.clear();
    for (int i = 0; i < num_threads; ++i) {
        threads.emplace_back(fn_worker_pop, i);
    }

    // Wait for threads to finish
    for (auto &t : threads) {
        t.join();
    }

    // Check queue size
    ASSERT_EQ(q.size(), 0);
}

int main(int argc, char** argv) {
    testing::InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();
}
