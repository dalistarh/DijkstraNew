#ifndef __SEQPQ_H
#define __SEQPQ_H

#include <cstring>

namespace kpqbench
{

/**
 * A custom heap implementation as used in klsm benchmarks. Never shrinks.
 */
template <class K, class V>
class SeqPQ
{
private:
    class entry_t
    {
    public:
        K key;
        V value;

        bool operator<(const entry_t &that) const
        {
            return this->key < that.key;
        }

        bool operator<=(const entry_t &that) const
        {
            return this->key <= that.key;
        }

        bool operator>=(const entry_t &that) const
        {
            return !operator<(that);
        }
    };

public:
    SeqPQ();
    virtual ~SeqPQ();

    void insert(const K &key, const V &value);
    bool delete_min(V &value, const int  &thread_id);
    void clear();

    void print() const;

    void init_thread(const size_t) const { }
    constexpr static bool supports_concurrency() { return true; }

private:
    void grow();
    void bubble_up(const size_t ix);
    void bubble_down(const size_t ix);

private:
    constexpr static size_t INITIAL_CAPACITY = 64;

    entry_t *m_array;
    size_t m_length;
    size_t m_capacity;
};

template <class K, class V>
SeqPQ<K, V>::SeqPQ() :
    m_array(new entry_t[INITIAL_CAPACITY]),
    m_length(0),
    m_capacity(INITIAL_CAPACITY)
{
}

template <class K, class V>
SeqPQ<K, V>::~SeqPQ()
{
    delete[] m_array;
}

template <class K, class V>
bool
SeqPQ<K, V>::delete_min(V &value, const int &thread_id)
{

    if (m_length == 0) {
        return false;
    }

    value = m_array[0].value;
    m_array[0] = m_array[m_length - 1];
    bubble_down(0);
    m_length--;

    return true;
}

template <class K, class V>
void
SeqPQ<K, V>::bubble_down(const size_t ix)
{
    size_t i = ix;
    size_t j = (i << 1) + 1;
    while (j < m_length) {
        const size_t k = j + 1;
        if (k < m_length && m_array[k] < m_array[j]) {
            j = k;
        }
        if (m_array[i] <= m_array[j]) {
            break;
        }
        std::swap(m_array[j], m_array[i]);
        i = j;
        j = (i << 1) + 1;
    }
}

template <class K, class V>
void
SeqPQ<K, V>::insert(const K &key,
                         const V &value)
{

    if (m_length == m_capacity) {
        grow();
    }

    m_array[m_length] = { key, value };
    bubble_up(m_length);
    m_length++;
}

template <class K, class V>
void
SeqPQ<K, V>::grow()
{
    const size_t new_capacity = m_capacity << 1;
    entry_t *new_array = new entry_t[new_capacity];

    memcpy(new_array, m_array, sizeof(m_array[0]) * m_capacity);
    delete[] m_array;

    m_array = new_array;
    m_capacity = new_capacity;
}

template <class K, class V>
void
SeqPQ<K, V>::bubble_up(const size_t ix)
{
    size_t i = ix;
    size_t j = (i - 1) >> 1;
    while (i > 0 && m_array[i] < m_array[j]) {
        std::swap(m_array[j], m_array[i]);
        i = j;
        j = (i - 1) >> 1;
    }
}

template <class K, class V>
void
SeqPQ<K, V>::clear()
{
    m_length = 0;
}

template <class K, class V>
void SeqPQ<K, V>::print() const
{
    /* NOP */
}

}

#endif /* __SEQPQ_H */
