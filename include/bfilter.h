#include <functional>
#include <vector>

namespace bloom
{
    template <class T>
    struct hash
    {
        std::size_t seed;

        inline std::size_t operator()(const T &v)
        {
            return seed ^ std::hash<T>{}(v) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        }
    };

    template <typename Value>
    class bfilter
    {
        std::vector<bool> filter_;
        int hashes_;

    public:
        using value_type = Value;
        using size_type = std::vector<bool>::size_type;

        explicit bfilter(int hashes, size_type size)
            : hashes_(hashes), filter_(size, false)
        {
        }
        /*
         * Insert value into the filter
         */
        void insert(const value_type &value)
        {
            for (unsigned int i = 0; i < hashes_; ++i)
            {
                filter_[hash<value_type>{i}(value) % filter_.size()] = true;
            }
        }
        /*
         * Insert value into the filter
         */
        void insert(value_type &&value)
        {
            for (unsigned int i = 0; i < hashes_; ++i)
            {
                filter_[hash<value_type>{i}(value) % filter_.size()] = true;
            }
        }
        /*
         * Check whether the filter possibly contains value or whether
         * it definitely doesn't
         */
        bool contains(const value_type &value) const
        {
            for (unsigned int i = 0; i < hashes_; ++i)
            {
                if (!filter_[hash<value_type>{i}(value) % filter_.size()])
                {
                    return false;
                }
            }
            return true;
        }
    };
} // namespace bloom
