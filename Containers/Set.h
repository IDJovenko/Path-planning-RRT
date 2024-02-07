//#define _SILENCE_CXX17_ITERATOR_BASE_CLASS_DEPRECATION_WARNING
//STL checks has been disabled

#pragma once
#include <iterator>
#include <memory>
#include <functional>
#include <iostream>

template <typename TS>
class Set;
template <typename TI>
class SetIterator;

template <typename TO>
struct object
{
    object() : prev(nullptr), next(nullptr), elem(nullptr) {}
    object(TO* t)
    {
        this->elem = t;
    }
    struct object* prev = 0;
    TO* elem;
    object* next = nullptr;

    friend class SetIterator<TO>;
    friend class Set<TO>;
};

template <typename TI>
class SetIterator : public std::iterator<std::bidirectional_iterator_tag, object<TI>>
{
private:
    SetIterator(object<TI>* p) : p(p) {}
public:
    friend class Set<TI>;

    SetIterator(const SetIterator& it) : p(it.p) {}

    bool operator!=(SetIterator const& other) const
    {
        return p != other.p;
    }
    bool operator==(SetIterator const& other) const
    {
        return p == other.p;
    }
    TI* operator*() const
    {
        TI* ptr = p->elem;
        return ptr;
    }
    SetIterator& operator++()
    {   
        if (p->next == nullptr)
            throw "tried to increment too far past the end";
        p = p->next;
        return *this;
    }
    SetIterator& operator--()
    {
        if (p == nullptr)
            throw "decremented an empty iterator";
        if (p->prev == nullptr)
            throw "tried to decrement to first element";

        p = p->prev;
        return *this;
    }
    SetIterator& operator++(int)
    {
        SetIterator temp(this->p);
        this->p = this->p->next;
        return temp;
    }
    SetIterator& operator--(int)
    {
        SetIterator temp(this->p);
        --(*this);
        return temp;
    }
private:
    object<TI>* p;
};

template <typename TS>
class Set
{
public:
    typedef SetIterator<TS> iterator;

    Set() : head(nullptr), tail(nullptr) {}
    Set(TS* value)
    {
        object<TS>* ptr = new object<TS>;
        ptr->prev = nullptr;
        head = ptr;
        tail = new object<TS>();
        tail->prev = ptr;
        ptr->next = tail;
        ptr->elem = value;
    }
    ~Set()
    {
        object<TS>* tmp = head;

        object<TS>* i = head;
        while (i != tail)
        {
            i = i->next;
            delete tmp;
            tmp = i;
        }

        delete tmp;
    }

    void push(TS* el) // помещение элемента в контейнер
    {
        if (head == nullptr)
        {
            object<TS>* ptr = new object<TS>;
            ptr->prev = nullptr;
            head = ptr;
            tail = new object<TS>();
            tail->prev = ptr; //head
            ptr->next = tail;
            ptr->elem = el;
        }
        else
        {
            auto it = begin();
            auto t = end();
            for (it; (it != t); it++) //for (it; (it != t) && (**it < *el); it++)
            {
                if (**it == *el) return;
            }
            tail->elem = el;
            object<TS>* ptr = new object<TS>;
            tail->next = ptr;
            ptr->prev = tail;
            tail = ptr;
        }
    }
    // TODO: remake from naive implementation
    template <typename TypeInputIter>
    void assign(SetIterator<TypeInputIter> first, SetIterator<TypeInputIter> second)
    {
        // 1) deleting content
        object<TS>* tmp = head;

        object<TS>* i = head;
        while (i != tail)
        {
            i = i->next;
            delete tmp;
            tmp = i;
        }

        delete tmp;

        // 2) assigns new values
        for (auto it = first; it != second; it++) {
            this->push(*it);
        }
    }

    TS* pop() // возвращение элемента с удалением из контейнера
    {
        if (head == nullptr)
        {
            throw "tried to pop empty Set";
        }
        else
        {
            if (head->next == tail)
            {
                TS* tmp = head->elem;
                delete(head);
                head = nullptr;
                tail->prev = nullptr;
                return tmp;
            }
            else
            {
                TS* tmp_el = tail->prev->elem;
                tail->prev->prev->next = tail;
                object<TS>* tmp_obj = tail->prev;
                tail->prev = tail->prev->prev;
                delete(tmp_obj);
                return tmp_el;
            }
        }
    }
    void erase(iterator Where) // удаление элемента по соответсвующему итератору
    {
        if (Where.p == tail)
        {
            throw "cannot erase Set end() iterator";
        }
        else
        {
            if (Where.p == head)
            {
                object<TS>* tmp = head->next;
                tmp->prev = nullptr;
                delete(head);
                head = tmp;
            }
            else
            {
                Where.p->prev->next = Where.p->next;
                Where.p->next->prev = Where.p->prev;
                delete(Where.p);
            }
        }
    }
    iterator find(TS* el) const// поиск элемента в контейнере
    {
        auto it = begin();
        for (it; (it != end()); it++) //for (it; (it != t) && (**it < *el); it++)
        {
            if (**it == *el) { /*iterator i(new object<TS>(el));*/ return it; }
        }
        return iterator(tail); //== return end();
    }
    iterator begin() const {
        return iterator(head);
    }
    iterator end() const {
        return iterator(tail);
    }
    bool is_empty() const {
        return (head == nullptr && tail == nullptr) ? true : false;
    }

    void clear()
    {
        object<TS>* tmp = head;

        object<TS>* i = head;
        while (i != tail)
        {
            i = i->next;
            delete tmp;
            tmp = i;
        }
        delete tmp;
        head = nullptr;
        tail = nullptr;
    }

    void direct_print(char delimiter)
    {
        {
            for (auto i = begin(); i != end(); i++)
            {
                std::cout << **i << delimiter;
            }
            std::cout << std::endl;
        }
    }
    void reverse_print(char delimiter)
    {
        for (auto i = --end(); i != begin(); i--)
        {
            std::cout << **i << delimiter;
        }
        std::cout << **iterator(head);
        std::cout << delimiter << std::endl;
    }
private:
    object<TS>* head;
    object<TS>* tail;
};



//#pragma once
//#include <iterator>
//#include <memory>
//#include <functional>
//#include <iostream>
//
//template <typename TS>
//class Set;
//template <typename TI>
//class SetIterator;
//
//template <typename TO>
//struct object
//{
//    object() : prev(nullptr), next(nullptr), elem(nullptr) {}
//    object(TO* t)
//    {
//        this->elem = t;
//    }
//    struct object* prev = 0;
//    TO* elem;
//    object* next = nullptr;
//
//    //убрать ?/////////////
//    friend class SetIterator<TO>;
//    friend class Set<TO>;
//};
//
//template <typename TI>
//class SetIterator : public std::iterator<std::bidirectional_iterator_tag, object<TI>>
//{
//private:
//    SetIterator(object<TI>* p): p(p) {}
//public:
//    friend class Set<TI>;
//
//    SetIterator(const SetIterator& it): p(it.p){}
//
//    bool operator!=(SetIterator const& other) const
//    {   return p != other.p;}
//    bool operator==(SetIterator const& other) const
//    {   return p == other.p;}
//    TI* operator*() const
//    {   TI* ptr = p->elem;
//        return ptr;         }
//    SetIterator& operator++()
//    {   //if (p == nullptr)
//        //    throw "incremented an empty iterator";
//        if (p->next == nullptr)
//            throw "tried to increment too far past the end";
//
//        p = p->next;
//        return *this;
//    }
//    SetIterator& operator--()
//    {
//        if (p == nullptr)
//            throw "decremented an empty iterator";
//        if (p->prev == nullptr)
//            throw "tried to decrement to first element";
//
//        p = p->prev;
//        return *this;
//    }
//    SetIterator& operator++(int)
//    {
//        SetIterator temp(this->p);
//        this->p = this->p->next;
//        return temp;
//    }
//    SetIterator& operator--(int)
//    {
//        SetIterator temp(this->p);
//        --(*this);
//        return temp;
//    }
//private:
//    object<TI>* p;
//};
//
//template <typename TS>
//class Set
//{
//public:
//    typedef SetIterator<TS> iterator;
//
//    Set() : head(nullptr), tail(nullptr) {}
//    Set(TS* value)
//    {
//        object<TS>* ptr = new object<TS>;
//        ptr->prev = nullptr;
//        head = ptr;
//        tail = new object<TS>();
//        tail->prev = ptr;
//        ptr->next = tail;
//        ptr->elem = value;
//    }
//    ~Set()
//    {
//        object<TS>* tmp = head;
//
//        object<TS>* i = head;
//        while (i != tail)
//        {
//            i = i->next;
//            delete tmp;
//            tmp = i;
//        }
//
//        delete tmp;
//    }
//
//    void push(TS* el) // помещение элемента в контейнер
//    {
//        if (head == nullptr)
//        {
//            object<TS>* ptr = new object<TS>;
//            ptr->prev = nullptr;
//            head = ptr;
//            tail = new object<TS>();
//            tail->prev = ptr; //head
//            ptr->next = tail;
//            ptr->elem = el;
//        }
//        else
//        {
//            auto it = begin();
//            auto t = end();
//            for (it; (it != t); it++) //for (it; (it != t) && (**it < *el); it++)
//            {
//                if (**it == *el) return;
//            }
//            tail->elem = el;
//            object<TS>* ptr = new object<TS>;
//            tail->next = ptr;
//            ptr->prev = tail;
//            tail = ptr;
//        }
//    }
//    TS* pop() // возвращение элемента с удалением из контейнера
//    {
//        if (head == nullptr)
//        {
//            throw "tried to pop empty Set";
//        }
//        else
//        {
//            if (head->next == tail)
//            {
//                TS* tmp = head->elem;
//                delete(head);
//                head = nullptr;
//                tail->prev = nullptr;
//                return tmp;
//            }
//            else
//            {
//                TS* tmp_el = tail->prev->elem;
//                tail->prev->prev->next = tail;
//                object<TS>* tmp_obj = tail->prev;
//                tail->prev = tail->prev->prev;
//                delete(tmp_obj);
//                return tmp_el;
//            }
//        }
//    }
//    void erase(iterator Where) // удаление элемента по соответсвующему итератору
//    {
//        if (Where.p == tail)
//        {
//            throw "cannot erase Set end() iterator";
//        }
//        else
//        {
//            if (Where.p == head)
//            {
//                object<TS>* tmp = head->next;
//                tmp->prev = nullptr;
//                delete(head);
//                head = tmp;
//            }
//            else
//            {
//                Where.p->prev->next = Where.p->next;
//                Where.p->next->prev = Where.p->prev;
//                delete(Where.p);
//            }
//        }
//    }
//    iterator find(TS* el) // поиск элемента в контейнере
//    {
//        auto it = begin();
//        for (it; (it != end()); it++) //for (it; (it != t) && (**it < *el); it++)
//        {
//            if (**it == *el) { /*iterator i(new object<TS>(el));*/ return it; }
//        }
//        return iterator(tail); //== return end();
//    }
//    iterator begin(){  
//        return iterator(head);}
//    iterator end(){  
//        return iterator(tail);}
//    bool is_empty() {
//        return (head == nullptr && tail == nullptr) ? true : false;
//    }
//
//    void direct_print(char delimiter)
//    {
//        {
//            for (auto i = begin(); i != end(); i++)
//            {
//                std::cout << **i << delimiter;
//            }
//            std::cout << std::endl;
//        }
//    }
//    void reverse_print(char delimiter)
//    {
//        for (auto i = --end(); i != begin(); i--)
//        {
//            std::cout << **i << delimiter;
//        }
//        std::cout << **iterator(head);
//        std::cout << delimiter << std::endl;
//    }
//private:
//    object<TS>* head;
//    object<TS>* tail;
//};