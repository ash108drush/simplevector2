#pragma once

#include <cassert>
#include <initializer_list>
#include <stdexcept>
#include <algorithm>
#include <iostream>
#include "array_ptr.h"
using namespace std;


class ReserveProxyObj{
public:
    ReserveProxyObj()= delete;
    ReserveProxyObj(size_t capacity_to_reserve):capacity_(capacity_to_reserve){
    }
    size_t GetCapacity(){
        return capacity_;
    }
private:
    size_t capacity_=0;

};


ReserveProxyObj Reserve(size_t capacity_to_reserve) {
    return ReserveProxyObj(capacity_to_reserve);
};

template <typename Type>
class SimpleVector {
public:
    using Iterator = Type*;
    using ConstIterator = const Type*;


    SimpleVector() noexcept = default;

    explicit SimpleVector(size_t size):items_(size),size_(size),capacity_(size) {
        fill(begin(),end(),Type{});

    }

    SimpleVector(size_t size, const Type& value):items_(size), size_(size),capacity_(size){
        fill(begin(),begin()+size,value);
    }

    SimpleVector(std::initializer_list<Type> init):items_(init.size()),size_(init.size()),capacity_(init.size()){
        if(init.size() >0){
             copy(init.begin(),init.end(),begin());
        }

    }

    SimpleVector(ReserveProxyObj obj):items_(obj.GetCapacity()){
        capacity_=0;
        size_=0;
        Reserve(obj.GetCapacity());
    }


    size_t GetSize() const noexcept {
        return size_;
    }

    size_t GetCapacity() const noexcept {
        return capacity_;
    }

    bool IsEmpty() const noexcept {
        return (size_==0);
    }

    Type& operator[](size_t index) noexcept {
        return items_[index];
    }

    const Type& operator[](size_t index) const noexcept {
        return items_[index];
    }

    Type& At(size_t index) {
        if(index >= size_){
            throw std::out_of_range("index out of range");
        }
        return items_[index];
    }

    const Type& At(size_t index) const {
        if(index >= size_){
            throw std::out_of_range("index out of range");
        }
        return items_[index];
    }

    void Clear() noexcept {
        size_=0;
    }

    void Resize(size_t new_size) {
        if(new_size==size_){
            return;
        }
        if(new_size < size_){
                size_ = new_size;
        }else{
            capacity_=max(new_size,capacity_*2);
            ArrayPtr<Type> new_items(capacity_);
            fill(new_items.Get(),new_items.Get()+capacity_,Type{});
            copy(begin(),begin()+size_,new_items.Get());
            items_.swap(new_items);
            size_=new_size;
        }
    }

    Iterator begin() noexcept {
        return size_ == 0 ? nullptr : items_.Get();
    }

    Iterator end() noexcept {
        return size_ == 0 ? nullptr : items_.Get()+size_;

    }

    ConstIterator begin() const noexcept {
        return items_.Get();
    }

    ConstIterator end() const noexcept {
        return size_ == 0 ? nullptr : items_.Get()+size_;
    }

    ConstIterator cbegin() const noexcept {
        return size_ == 0 ? nullptr : items_.Get();
    }

    ConstIterator cend() const noexcept {
        return size_ == 0 ? nullptr : items_.Get()+size_;
    }

    SimpleVector(const SimpleVector& other) {
        assert(size_ == 0);
        SimpleVector tmp(other.size_);
        copy(other.begin(),other.end(),tmp.begin());
        swap(tmp);
     }

     SimpleVector& operator=(const SimpleVector& rhs) {
         SimpleVector s_copy(rhs);
         swap(s_copy);
         return *this;
     }

     void PushBack(const Type& item) {
         if(size_ < capacity_){
             items_[size_++]=item;
          }else{
             Resize(size_+1);
             items_[size_-1]=item;
         }
     }

     void Reserve(size_t new_capacity){
         if(new_capacity > capacity_){
             ArrayPtr<Type> new_items(new_capacity);
             fill(new_items.Get(),new_items.Get()+new_capacity,Type{});
             copy(begin(),begin()+size_,new_items.Get());
             items_.swap(new_items);
            capacity_ = new_capacity;
         }


     }


     Iterator Insert(ConstIterator pos, const Type& value) {
            int iterator_shift=pos-begin();
             ArrayPtr<Type> new_items(size_+1);
             if(capacity_==0){
                 capacity_=1;
             }else if(size_ == capacity_){
                 capacity_=capacity_*2;
             }
             copy(begin(),begin()+iterator_shift,new_items.Get());
             copy_backward(begin()+iterator_shift,end(),new_items.Get()+(size_+1));
             new_items[iterator_shift]=value;
             items_.swap(new_items);
             ++size_;
             return (begin()+iterator_shift);

     }

     void PopBack() noexcept {
         if(size_ ==0){
             return;
         }
         --size_;

     }


      Iterator Erase(ConstIterator pos) {
         ArrayPtr<Type> new_items(size_-1);
         int iterator_shift=pos-begin();
         copy(begin(),begin()+iterator_shift,new_items.Get());
         copy(begin()+iterator_shift+1,end(),new_items.Get()+iterator_shift);
         items_.swap(new_items);
         --size_;
         return (begin()+iterator_shift);
     }

     void swap(SimpleVector& other) noexcept {
         items_.swap(other.items_);
         std::swap(size_,other.size_);
         std::swap(capacity_,other.capacity_);
     }

private:


    ArrayPtr<Type> items_;
    size_t size_=0;
    size_t capacity_=0;
};



template <typename Type>
inline bool operator==(const SimpleVector<Type>& lhs, const SimpleVector<Type>& rhs) {
    if(&lhs==&rhs){
        return true;
    }
    if(lhs.GetSize() != rhs.GetSize()){
        return false;
    }
    return std::equal(lhs.begin(),lhs.end(),rhs.begin(),rhs.end());
}

template <typename Type>
inline bool operator!=(const SimpleVector<Type>& lhs, const SimpleVector<Type>& rhs) {
    return !(lhs==rhs);
}

template <typename Type>
inline bool operator<(const SimpleVector<Type>& lhs, const SimpleVector<Type>& rhs) {
    return std::lexicographical_compare(lhs.begin(),lhs.end(),rhs.begin(),rhs.end());

}

template <typename Type>
inline bool operator<=(const SimpleVector<Type>& lhs, const SimpleVector<Type>& rhs) {
    return !(rhs<lhs);

}

template <typename Type>
inline bool operator>(const SimpleVector<Type>& lhs, const SimpleVector<Type>& rhs) {
    return (rhs<lhs);
}

template <typename Type>
inline bool operator>=(const SimpleVector<Type>& lhs, const SimpleVector<Type>& rhs) {
    return !(rhs>lhs);
}
