
#include <cstdint>
#include <stdexcept>

#include <vector>
#include <stack>

namespace local {

    template<typename T>
    struct nd_bucket_array {
        nd_bucket_array(uint32_t size, uint32_t (*mapping_function)(T &));

        T get_from_highest_bucket();

        T pop_from_highest_bucket();

        uint32_t get_size();

        bool is_empty();

        void push(T &new_element);

    private:
        std::stack<T> history;
        std::vector<std::stack<T>> buckets;

        uint32_t (*_bucket_mapper)(T &element);

        uint32_t _size, _biggest_index;
    };

    template<typename T>
    bool nd_bucket_array<T>::is_empty() {
        return _size == 0;
    }

    template<typename T>
    nd_bucket_array<T>::nd_bucket_array(uint32_t size, uint32_t (*mapping_function)(T &)) :
            _bucket_mapper(mapping_function),
            _biggest_index(0),
            _size(0) {
        buckets.resize(size);
    }

    template<typename T>
    T nd_bucket_array<T>::get_from_highest_bucket() {
        if (_size == 0)
            throw std::runtime_error("trying to get element from empty bucket list");
        else
            return buckets[_biggest_index].top();
    }

    template<typename T>
    T nd_bucket_array<T>::pop_from_highest_bucket() {
        if (_size == 0)
            throw std::runtime_error("trying to pop element from empty bucket list");
        T &top = buckets[_biggest_index].top();
        buckets[_biggest_index].pop();
        _size -= 1;
        if (buckets[_biggest_index].empty()) {
            history.pop();
            if (!history.empty())
                _biggest_index = history.top();
        }
        return top;
    }

    template<typename T>
    uint32_t nd_bucket_array<T>::get_size() {
        return _size;
    }

    template<typename T>
    void nd_bucket_array<T>::push(T &new_element) {
        uint32_t selected_bucket = _bucket_mapper(new_element);
        buckets[selected_bucket].push(new_element);
        if (selected_bucket > _biggest_index) {
            _biggest_index = selected_bucket;
            history.push(selected_bucket);
        }
        _size += 1;
    }
}
