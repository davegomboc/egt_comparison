#if !defined(EGT_COMPARISON_DEFAULT_INIT_TYPE_HPP)
#define EGT_COMPARISON_DEFAULT_INIT_TYPE_HPP


#include <memory>
#include <utility>


// Adapted from https://stackoverflow.com/questions/21028299#2108912.
template <typename pod_type, typename allocator_type = std::allocator<pod_type>>
class default_init_type: public allocator_type {
public:
    template <typename other_type>
    struct rebind {
        using other = default_init_type<other_type,
         typename std::allocator_traits<allocator_type>::template
         rebind_alloc<other_type>>;
    };

    // Make the various constructors defined by the allocator_type that is being
    // adapted available to this allocator type also.
    using allocator_type::allocator_type;

    // Redefines zero-argument initialization so that zero-initialization is NOT
    // performed.
    template <typename other_type>
    void construct(other_type * ptr)
     noexcept(std::is_nothrow_default_constructible_v<other_type>) {
        // It is the absence of an initializer in this new-expression that
        // causes default initialization to occur.
        ::new(static_cast<void *>(ptr)) (other_type);
    }

    // Forward non-zero-argument constructor calls to the equivalent
    // constructors from the allocator traits that are associated with the
    // underlying allocator_type, so that proper initialization is performed.
    template <typename other_type, typename... argument_types>
    void construct(other_type * ptr, argument_types && ... arguments) {
        std::allocator_traits<allocator_type>::construct(
         static_cast<allocator_type &>(*this), ptr,
         std::forward<argument_types>(arguments)...);
    }
};


#endif // EGT_COMPARISON_DEFAULT_INIT_TYPE_HPP
