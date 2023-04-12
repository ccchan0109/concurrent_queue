# concurrent_queue

I implemented a simple thread-safe queue for myself.

This `ConcurrentQueue` class is really easy. It just inherits `std::queue` and have a `std::mutex` member to make it thread-safe. I know maybe it's not the best implementation like some well-known libraries, but for me it is enough. And it is simple, containing only one header.

Unit tests are also added to make sure its functionality by `gtest` framework.

To run the test, please make sure `gtest` has correctly installed and have `-lgtest` flag in the linker argument.

If you have any issue while using it, just fire a issue or tell me.
