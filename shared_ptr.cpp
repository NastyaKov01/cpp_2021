#include <iostream>
#include <memory>

class A {
public:
    static void * operator new(size_t size) {
        std::cout << "operator new!" << std::endl;
        return ::operator new(size);
    }

    static void operator delete(void *p, size_t size) {
        std::cout << "operator delete!" << std::endl;
        return ::operator delete(p);
    }
};

template <class T1, class T2>
struct custom_allocator 
{
    using value_type = T1;
    custom_allocator() noexcept {}
    template <class U, class V> 
    custom_allocator (const custom_allocator<U, V>&) noexcept {}

    T1 * allocate (std::size_t n)
    {
        return reinterpret_cast<T1*>(T2::operator new(n));
    }

    void deallocate (T1* p, std::size_t n)
    {
        return T2::operator delete(p, n);
    }
};


int main()
{
    custom_allocator<A, A> alloc;
    auto sp = std::allocate_shared<A>(alloc);
}
