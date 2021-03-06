#pragma once

#include <util/Collection.hpp>

namespace sys {

/**
 * A Queue is a FIFO container.
 * @tparam T The type of object the Queue contains.
 */
template <typename T>
concept Queue = DynamicCollection<T> && requires(T a)
{
    /**
     * Adds an element to the back of the Queue.
     * @param obj The object to add.
     * @return `true` if an object was added. `false` otherwise.
     */
    { a.enqueue(std::declval<typename T::value_type>()) } -> std::same_as<bool>;

    /**
     * Removes an element from the front of the queue.
     * @return The object. If the queue is empty, the return value is undefined.
     */
    { a.pop() } -> std::same_as<typename T::value_type>;

    /**
     * Returns the element at the front of the queue without removing it.
     * @return The object. If the queue is empty, the return value is undefined.
     */
    { a.peek() } -> std::same_as<typename T::value_type const &>;
};

} // namespace concepts
