#include "../exercise.h"
#include <memory>

// READ: `std::shared_ptr` <https://zh.cppreference.com/w/cpp/memory/shared_ptr>
// READ: `std::weak_ptr` <https://zh.cppreference.com/w/cpp/memory/weak_ptr>

// TODO: 将下列 `?` 替换为正确的值
int main(int argc, char **argv) {
    auto shared = std::make_shared<int>(10);
    std::shared_ptr<int> ptrs[]{shared, shared, shared};

    std::weak_ptr<int> observer = shared;
    ASSERT(observer.use_count() == 3, ""); // Initial count should be 3

    ptrs[0].reset();
    ASSERT(observer.use_count() == 2, ""); // After resetting ptrs[0], count should be 2

    ptrs[1] = nullptr;
    ASSERT(observer.use_count() == 1, ""); // After resetting ptrs[1], count should be 1

    ptrs[2] = std::make_shared<int>(*shared);
    ASSERT(observer.use_count() == 1, ""); // No increase in count, since new object is created

    ptrs[0] = shared;
    ptrs[1] = shared;
    ptrs[2] = std::move(shared);
    ASSERT(observer.use_count() == 3, ""); // After moving shared, count should be 3

    std::ignore = std::move(ptrs[0]);
    ptrs[1] = std::move(ptrs[1]);
    ptrs[1] = std::move(ptrs[2]);
    ASSERT(observer.use_count() == 3, ""); // Count remains the same

    shared = observer.lock();
    ASSERT(observer.use_count() == 4, ""); // Locking observer should increase count to 4

    shared = nullptr;
    for (auto &ptr : ptrs) ptr = nullptr;
    ASSERT(observer.use_count() == 0, ""); // After resetting all, count should be 0

    shared = observer.lock();
    ASSERT(observer.use_count() == 1, ""); // After locking again, count should be 1

    return 0;
}
