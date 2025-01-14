#include <ctime>
#include <optional>
#include <random>
#include <vector>
#include <linq/core.hpp>
#include <iostream>
#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include <catch2/catch.hpp>
using namespace std;
using namespace linq;
/*This test demonstrates a custom enumerator that empties itself
 * as is moves next
 * The random part is distracting
 */

template <typename T>
class random_enumerator
{
private:
    vector<T> m_vec;
    optional<T> m_current;
    mt19937 rnd;

    void move_next()
    {
        if (m_vec.empty())
        {
            m_current = nullopt;
        }
        else
        {
            uniform_int_distribution<size_t> d{ 0, m_vec.size() - 1 };
            size_t index{ d(rnd) };
            m_current = m_vec[index];
            m_vec.erase(m_vec.begin() + index);
        }
    }

public:
    template <typename Eter>
    constexpr random_enumerator(Eter&& eter) : rnd((unsigned int)time(nullptr))
    {
        for (; eter; ++eter)
        {
            m_vec.emplace_back(*eter);
        }
        if(m_vec.size()>0)
          m_current = m_vec[0];
    }

    constexpr operator bool() const { return (bool)m_current; }
    constexpr random_enumerator& operator++()
    {
        move_next();
        return *this;
    }
    constexpr decltype(auto) operator*() { return *m_current; }
};

constexpr auto as_random()
{
    return [](auto e) {
        using T = remove_cref<decltype(*e.enumerator())>;
        return enumerable(random_enumerator<T>(e.enumerator()));
    };
}
#define is_true(X) REQUIRE(X)
TEST_CASE ("test","[extension_test]")
{
    auto e{ range(0, 10) >> as_random() };
    for(auto& item :e)
    {
      is_true(true);
    }
}
