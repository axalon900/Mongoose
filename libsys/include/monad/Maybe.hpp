#pragma once

#include <util/TypeTraits.hpp>
#include <util/Void.hpp>
#include <Object.hpp>

#include <stdint.h>

template <typename T, typename = IfValue<T>>
class Maybe : public Object
{
  public:
    // our custom types
    using RegularType = regular_t<T>;
    template <bool Const> struct BasicIterator;
    using Iterator = BasicIterator<false>;
    using ConstIterator = BasicIterator<true>;
    using RawType = typename std::decay<RegularType>::type;
    using ValueType = typename std::remove_reference<RegularType>::type;
    using MutableType = typename std::remove_const<RegularType>::type;
    using RValueReference = ValueType &&;
    using Reference = ValueType &;
    using ConstReference = const RegularType &;
    using Pointer = ValueType *;
    using ConstPointer = const ValueType *;

    // std container member types
    using iterator = Iterator;
    using const_iterator = ConstIterator;
    using value_type = ValueType;
    using reference = Reference;
    using const_reference = ConstReference;
    using pointer = Pointer;
    using const_pointer = ConstPointer;
    using difference_type = int64_t;
    using size_type = size_t;

    Maybe() = default;
    Maybe(Void) : Maybe() {}

    template <typename U = T, typename = IfCopyConstructible<U>>
    Maybe(T const &v) : _store(v), _set(true) {}

    template <typename U = T, typename = IfMoveConstructible<U>>
    Maybe(T &&v) : _store(std::move(v)), _set(true) {}

    /** Returns whether this contains a value. */
    constexpr operator bool() const noexcept { return _set; }

    /** Returns whether this contains a value. */
    constexpr bool isPresent() const noexcept { return _set; }

    /**
     * Accesses the contained value.
     * @return A const pointer to the value if set, nullptr otherwise.
     */
    constexpr T const * operator->() const { return _set ? &ref_() : nullptr; }

    /**
     * Accesses the contained value.
     * @return A pointer to the value if set, nullptr otherwise.
     */
    constexpr T * operator->() { return _set ? &ref_() : nullptr; }

    /**
     * Accesses the contained value.
     * @return A const ref to the value. Return value is undefined if not set.
     */
    constexpr T const & operator*() const { return ref_(); }

    /**
     * Accesses the contained value.
     * @return A reference to the value. Return value is undefined if not set.
     */
    constexpr T & operator*() { return ref_(); }

    /**
     * Returns the contained value.
     * @return A const ref to the value. Return value is undefined if not set.
     */
    constexpr T const & get() const { return ref_(); }

    /**
     * Returns the contained value.
     * @return A reference to the value. Return value is undefined if not set.
     */
    constexpr T & get() { return ref_(); }

    /**
     * Returns a copy of the contained value, or a default value if not set.
     * @param other The value to default to.
     * @return A copy of the value, or the default value if this is not set.
     */
    constexpr T getOr(T &&other) const &
    {
        return _set ? ref_() : std::forward<T>(other);
    }

    /**
     * Moves out the contained value or returns a default value if not set.
     * @param other The value to default to.
     * @return A copy of the value, or the default value if this is not set.
     */
    constexpr T getOr(T &&other) &&
    {
        return _set ? std::move(takeValue_()) : std::forward<T>(other);
    }

    /** @name STL container interface */
    /** @{ */

    bool empty() const { return empty(); }
    size_type size() const { return isPresent() ? 1 : 0; }
    size_type max_size() const { return 1; }

    const_iterator cbegin() const { return isPresent() ? const_iterator{this} : cend(); }
    const_iterator cend() const { return const_iterator{this, const_iterator::kEndIterator}; }
    const_iterator begin() const { return cbegin(); }
    const_iterator end() const { return cend(); }
    iterator begin() { return isPresent() ? iterator{this} : end(); }
    iterator end() { return iterator{this, iterator::kEndIterator}; }

    reference front() { return get(); }
    const_reference front() const { return get(); }
    reference back() { return get(); }
    const_reference back() const { return get(); }

    /** @} */

  private:
    inline constexpr decltype(auto) takeValue_() noexcept { return _store.rvRef(); }
    inline constexpr decltype(auto) ref_() noexcept { return _store.ref(); }
    inline constexpr decltype(auto) ref_() const noexcept { return _store.ref(); }

    template<typename Ts>
    union Storage
    {
        Storage() : _nothing() {}
        Storage(Ts const &aValue) : _value(aValue) {}
        Storage(Ts &&aValue) : _value(std::move(aValue)) {}
        ~Storage() {}

        void Destroy(bool isSet) { if (isSet) { _value.~Ts(); } }
        void Init(const Ts &val) { new (&_value) Ts{val}; }
        void Init(Ts &&val) { new (&_value) Ts{std::move(val)}; }

        value_type&& rvRef() noexcept { return std::move(_value); }

        value_type& ref() noexcept { return _value; }
        value_type const & ref() const noexcept { return _value; }

      private:
        Void _nothing;
        Ts _value;
    };

    template<typename Ts>
    union Storage<Ts &>
    {
        Storage()  noexcept : _nothing() {}
        Storage(Reference aValue) noexcept : _value(&aValue) {}

        void Destroy(bool) noexcept {}
        void Init(Reference val) noexcept { _value = &val; }
        auto rvRef() noexcept { return *_value; }
        value_type& ref() const noexcept { return *_value; }

      private:
        Void _nothing;
        value_type *_value;
    };

    Storage<MutableType> _store{};
    bool _set{false};
  public:
    template <bool Const>
    struct BasicIterator
    {
        static constexpr const bool kValueIterator = false;
        static constexpr const bool kEndIterator = true;
//        using iterator_category = std::bidirectional_iterator_tag;
        using value_type = RegularType;
        using difference_type = Maybe::difference_type;
        using reference = const_t<RegularType, Const>;
        using pointer = typename std::conditional<Const, RegularType const *, RegularType *>::type;

        // maintain is_trivially_copy_constructible by defining the Iterator->ConstIterator constructor as a template.
        template <bool OtherConst, typename = typename std::enable_if<Const || !OtherConst>::type>
        constexpr BasicIterator(const BasicIterator<OtherConst> &other) : parent_(other.parent_), end_(other.end_) {}

        explicit BasicIterator(const_ptr_t<Maybe, Const> parent) : parent_(parent), end_(kValueIterator) {}
        constexpr BasicIterator(const_ptr_t<Maybe, Const> parent, bool end) : parent_(parent), end_(end) {}

        constexpr reference operator*() const { return parent_->GetValue(); }

        constexpr BasicIterator& operator++() { end_ = true; return *this; }
        constexpr BasicIterator operator++(int) { auto copy = *this; operator++(); return copy; }
        constexpr BasicIterator& operator--() { end_ = false; return *this; }
        constexpr BasicIterator operator--(int) { auto copy = *this; operator--(); return copy; }

        constexpr bool operator==(BasicIterator const &rhs) const { return parent_ == rhs.parent_ && end_ == rhs.end_; }
        constexpr bool operator!=(BasicIterator const &rhs) const { return !(rhs == *this); }

        constexpr bool end() const { return end_; }

      private:
        const_ptr_t<Maybe, Const> parent_;
        bool end_;
    };
};

/**
 * Constructs and "promotes" an object to a Maybe<T>.
 * @tparam T The type of Maybe.
 * @tparam Args The types for one of T's constructors. (deducible)
 * @param args The arguments for one of T's constructors.
 * @return The Maybe.
 */
template <typename T, typename... Args>
static Maybe<T> AsMaybe(Args && ...args)
{
    return Maybe<T>(T(std::forward<Args>(args)...));
}
