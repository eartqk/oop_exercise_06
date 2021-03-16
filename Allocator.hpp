#ifndef ALLOCATOR_HPP
#define ALLOCATOR_HPP

#include <stdlib.h>
#include <utility>
#include <iostream>
#include <list>

template <class T, size_t ALLOC_SIZE> // ALLOC_SIZE - размер, который нужно выделить
struct TAllocator
{
private:
    char *blocks_begin; // Указатель на начало выделенной памяти
    char *blocks_end;   // Указатель на конец выделенной памяти
    char *blocks_tail;  // Указатель на конец заполненного пространства
    std::list<char *> free_blocks;

public:
    using value_type = T;
    using size_type = size_t;
    using pointer = T *;
    using const_pointer = const T *;
    using difference_type = std::ptrdiff_t;

    template <class U>
    struct rebind
    {
        using other = TAllocator<U, ALLOC_SIZE>;
    };

    // Выделение памяти
    T *allocate(std::size_t n)
    {
        if (n != 1)
        {
            throw std::logic_error("Что-то пошло не так");
        }
        if (size_t(blocks_end - blocks_tail) < sizeof(T))
        {
            if (free_blocks.size())
            { // Ищем свободное место
                char *ptr = free_blocks.front();
                free_blocks.pop_front();
                return reinterpret_cast<T *>(ptr);
            }
            std::cout << "Bad try to allocate" << '\n';
            throw std::bad_alloc();
        }
        T *result = reinterpret_cast<T *>(blocks_tail); // Приведение к типу
        blocks_tail += sizeof(T);
        return result;
    }

    // Освобождаем память
    void deallocate(T *ptr, std::size_t n)
    {
        if (n != 1)
        {
            throw std::logic_error("Не получается выделить память");
        }
        if (ptr == nullptr)
        {
            return;
        }
        free_blocks.push_back(reinterpret_cast<char *>(ptr));
    }

    TAllocator(const TAllocator &) = delete;
    TAllocator(TAllocator &&) = delete;
    ~TAllocator()
    {
        delete[] blocks_begin;
    }

    // Строится объект типа U, преобразет свои аргументы к соответствующему контруктору
    template <typename U, typename... Args>
    void construct(U *p, Args &&... args)
    {
        new (p) U(std::forward<Args>(args)...);
    }
};

#endif