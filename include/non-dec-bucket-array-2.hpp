
#include <cstdint>

#include <stdexcept>
#include <vector>
#include <stack>

namespace local {

    template<typename T>
    struct nd_bucket_array {
        nd_bucket_array(uint32_t size, int32_t (*mapping_function)(T &));

        T *get_from_highest_bucket();

        T *pop_from_highest_bucket();

        uint32_t get_size();

        bool is_empty();

        void push(T *new_element);

        static std::string get_report_header();

        std::string get_report();

    private:
        std::vector<std::stack<T *>> buckets;

        int32_t (*_bucket_mapper)(T &element);

        int32_t _size, _biggest_index;
        uint64_t pushes, pops, lapses;
    };

    template<typename T>
    std::string nd_bucket_array<T>::get_report() {
        std::stringstream ss;
        ss << pushes << ";";
        ss << pops << ";";
        ss << lapses;
        return ss.str();
    }

    template<typename T>
    std::string nd_bucket_array<T>::get_report_header() {
        return "nd-bucket-arr_pushes;nd-bucket-arr_pops;nd-bucket-arr_lapses";
    }

    template<typename T>
    bool nd_bucket_array<T>::is_empty() {
        return _size == 0;
    }

    template<typename T>
    nd_bucket_array<T>::nd_bucket_array(uint32_t size, int32_t (*mapping_function)(T &)) :
            buckets(size),
            _bucket_mapper(mapping_function),
            _size(0),
            _biggest_index(-1),
            pushes(0),
            pops(0),
            lapses(0) {}

    template<typename T>
    T *nd_bucket_array<T>::get_from_highest_bucket() {
        if (_size == 0)
            throw std::runtime_error("trying to get element from empty bucket list");
        else
            return buckets[_biggest_index].top();
    }

    template<typename T>
    T *nd_bucket_array<T>::pop_from_highest_bucket() {
        if (_size == 0)
            throw std::runtime_error("trying to pop element from empty bucket list");

        if (buckets[_biggest_index].empty())
            for (--_biggest_index; _biggest_index >= 0 && buckets[_biggest_index].size() == 0; --_biggest_index)
                ++lapses;

        T *top = buckets[_biggest_index].top();
        buckets[_biggest_index].pop();
        _size -= 1;
        ++pops;
        return top;
    }

    template<typename T>
    uint32_t nd_bucket_array<T>::get_size() {
        return _size;
    }

    template<typename T>
    void nd_bucket_array<T>::push(T *new_element) {
        int32_t selected_bucket = _bucket_mapper(*new_element);
        buckets[selected_bucket].push(new_element);
        if (selected_bucket > _biggest_index)
            _biggest_index = selected_bucket;
        _size += 1;
        ++pushes;
    }
}
