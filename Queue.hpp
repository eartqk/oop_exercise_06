#ifndef QUEUE_HPP
#define QUEUE_HPP

#include <memory>

#include "Allocator.hpp"

using namespace std;

// Реализация очереди
template <class T, class Allocator = std::allocator<T>>
class Queue {
private:
    class Queue_el;
    unique_ptr<Queue_el> first;
    Queue_el *tail = nullptr;
    size_t size = 0;
public:
    class Forward_iterator {
    public:
        using value_type = T;
        using reference = value_type & ;
        using pointer = value_type * ;
        using difference_type = std::ptrdiff_t;
        using iterator_category = std::forward_iterator_tag;
        explicit Forward_iterator(Queue_el *ptr) {
            it_ptr = ptr;
        }
        T &operator*() {
            return this->it_ptr->value;
        }
        Forward_iterator &operator++() {
            if (it_ptr == nullptr)
                throw std::length_error("Вышли за пределы очереди");
            *this = it_ptr->next();
            return *this;
        }
        Forward_iterator operator++(int) {
            Forward_iterator old = *this;
            ++*this;
            return old;
        }
        bool operator==(const Forward_iterator &other) const {
            return it_ptr == other.it_ptr;
        }
        bool operator!=(const Forward_iterator &other) const {
            return it_ptr != other.it_ptr;
        }
    private:
        Queue_el *it_ptr;
        friend Queue;
    };

    Forward_iterator begin() {
        return Forward_iterator(first.get());
    }
    Forward_iterator end() {
        return Forward_iterator(nullptr);
    }

    // Взять вверхний элемент (последний) из очереди
    T top() {
        if (size == 0) { throw length_error("Нельзя взять элемент из пустой очереди."); }
        Forward_iterator it = this->begin();
        for (size_t i = 0; i < this->size - 1; ++i) {
            ++it;
        }
        return *it;
    }

    // Удаление элемента из очереди
    void pop() { 
        Forward_iterator it = this->begin();
        for (size_t i = 0; i < this->size - 1; ++i) {
            ++it;
        }
        Forward_iterator begin = this->begin(), end = this->end();
        if (it == end) { throw length_error("Вышли за пределы очереди."); }
        if (it == begin) {
            // удаление из начала очереди
            if (size == 0) { cout << "Нельзя удалить элемент из пустой очереди.\n"; return;}
            if (size == 1) {
                first = nullptr;
                tail = nullptr;
                --size;
                return;
            }
            unique_ptr<Queue_el> tmp = std::move(first->next_el);
            first = std::move(tmp);
            first->prev_el = nullptr;
            --size;
            return;
        }
        // удаление из конца очереди
        if (it.it_ptr == tail) {
            if (size == 0) { cout << "Нельзя удалить элемент из пустой очереди.\n"; return;}
            if (tail->prev_el) {
                Queue_el *tmp = tail->prev_el;
                tail->prev_el->next_el = nullptr;
                tail = tmp;
            }
            else {
                first = nullptr;
                tail = nullptr;
            }
            --size;
            return;
        }
        if (it.it_ptr == nullptr) { throw std::length_error("Вышли за пределы очереди."); }

        auto tmp = it.it_ptr->prev_el;
        unique_ptr<Queue_el> temp1 = move(it.it_ptr->next_el);
        it.it_ptr = it.it_ptr->prev_el;
        it.it_ptr->next_el = move(temp1);
        it.it_ptr->next_el->prev_el = tmp;
        --size;
    }

    // Вставка элемента в начало очереди
    void push(T &value) { 
        size_t index = 0;
        Forward_iterator it = this->begin();
        if (index >= this->size) { it = this->end(); }
        else {
            for (size_t i = 0; i < index; ++i) {
                ++it;
            }
        }
        unique_ptr<Queue_el> tmp = make_unique<Queue_el>(value);
        // вставка в начало очереди
        if (it == this->begin()) {
            size++;
            unique_ptr<Queue_el> tmp = move(first);
            first = make_unique<Queue_el>(value);
            first->next_el = move(tmp);
            if (first->next_el != nullptr) {
                first->next_el->prev_el = first.get();
            }
            if (size == 1) {
                tail = first.get();
            }
            if (size == 2) {
                tail = first->next_el.get();
            }
            return;
        }
        // вставка в конец очереди
        if (it.it_ptr == nullptr) {
            if (!size) {
                first = make_unique<Queue_el>(value);
                tail = first.get();
                size++;
                return;
            }
            tail->next_el = make_unique<Queue_el>(value);
            Queue_el *tmp = tail;
            tail = tail->next_el.get();
            tail->prev_el = tmp;
            size++;
            return;
        }
        tmp->prev_el = it.it_ptr->prev_el;
        it.it_ptr->prev_el = tmp.get();
        tmp->next_el = std::move(tmp->prev_el->next_el);
        tmp->prev_el->next_el = std::move(tmp);
        size++;
    }

    Queue &operator=(Queue &other) {
        size = other.size;
        first = std::move(other.first);
    }

    T &operator[](size_t index) {
        if (index < 0 || index >= size) {
            throw std::out_of_range("out of queue");
        }
        Forward_iterator it = this->begin();
        for (size_t i = 0; i < index; i++) {
            it++;
        }
        return *it;
    }
private:
    using allocator_type = typename Allocator::template rebind<Queue_el>::other;
    struct deleter {
    private:
        allocator_type *allocator_;
    public:
        deleter(allocator_type *allocator) : allocator_(allocator) {}
        void operator()(Queue_el *ptr) {
            if (ptr != nullptr) {
                std::allocator_traits<allocator_type>::destroy(*allocator_, ptr);
                allocator_->deallocate(ptr, 1);
            }
        }
    };
    struct Queue_el {
        T value;
        unique_ptr<Queue_el> next_el;
        Queue_el *prev_el = nullptr;
        Queue_el() = default;
        Queue_el(const T &new_value) : value(new_value) {}
        Forward_iterator next() {
            return Forward_iterator(this->next_el.get());
        }
    };
};

#endif