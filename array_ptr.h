#include <cstdlib>
#include<algorithm>
template <typename Type>
class ArrayPtr {
public:
     ArrayPtr() = default;
    explicit ArrayPtr(size_t size) {
        (size==0)? raw_ptr_=nullptr : raw_ptr_ = new Type[size];
    }

    explicit ArrayPtr(Type* raw_ptr) noexcept {
        (raw_ptr==nullptr)? raw_ptr_=nullptr : raw_ptr_=raw_ptr;
    }

    ArrayPtr(const ArrayPtr&) = delete;

    ~ArrayPtr() {
        delete[] raw_ptr_;
    }

    ArrayPtr& operator=(const ArrayPtr&) = delete;

    [[nodiscard]] Type* Release() noexcept {
        Type *ptr=raw_ptr_;
        raw_ptr_=nullptr;
        return ptr;
    }

    Type& operator[](size_t index) noexcept {
        Type* ptr= &raw_ptr_[index];
        return  *ptr;
    }


    const Type& operator[](size_t index) const noexcept {
        const Type* ptr= &raw_ptr_[index];
        return  *ptr;
    }

    explicit operator bool() const {
        return (raw_ptr_!=nullptr);
    }

    Type* Get() const noexcept {
        return raw_ptr_;
    }

    void swap(ArrayPtr& other) noexcept {
         std::swap(raw_ptr_,other.raw_ptr_);
    }

private:
    Type* raw_ptr_ = nullptr;
};
