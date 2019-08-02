#include <iostream>
#include <linq/aggregate.hpp>
#include <linq/query.hpp>
#include <linq/to_container.hpp>


#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include <catch2/catch.hpp>
using namespace std;
using namespace linq;
    struct pack
    {
        int index;
        int score;
    };
    struct pack2
    {
        int index;
        std::string name;
    };
    struct pack3
    {
        std::string name;
        int score;
    };
#define are_equal(x,y) REQUIRE(x==y)
#define is_true(x) REQUIRE(x)
template <typename E1, typename E2>
void test_equals(E1&& e1, E2&& e2)
{
  auto eter1{ linq::get_enumerator(e1) };
  auto eter2{ linq::get_enumerator(e2) };
  for (; eter1 && eter2; ++eter1, ++eter2)
  {
    are_equal(*eter1, *eter2);
  }
}

inline std::ostream& operator<<(std::ostream& stream, const pack& p)
{
    return stream << '(' << p.index << ", " << p.score << ')';
}

inline bool operator==(const pack& p1, const pack& p2)
{
    return p1.index == p2.index && p1.score == p2.score;
}
inline bool operator!=(const pack& p1, const pack& p2) { return !(p1 == p2); }

inline std::ostream& operator<<(std::ostream& stream, const pack3& p)
{
    return stream << '(' << p.name << ", " << p.score << ')';
}

inline bool operator==(const pack3& p1, const pack3& p2)
{
    return p1.name == p2.name && p1.score == p2.score;
}
inline bool operator!=(const pack3& p1, const pack3& p2) { return !(p1 == p2); }

TEST_CASE ("group_method_test","[group_test]")
{
    pack a1[]{ { 2, 88 }, { 1, 92 }, { 2, 78 }, { 1, 66 }, { 3, 85 }, { 3, 61 } };
    pack a2[]{ { 1, 79 }, { 2, 83 }, { 3, 73 } };
    auto e{ a1 >>
            group([](pack& a) { return a.index; },
                  [](pack& a) { return a.score; },
                  [](int key, auto e) { return pack{ key, e >> average() }; }) };
    test_equals(a2, e);
}
TEST_CASE ("group_join_test","[group_test]")
{
    pack2 a1[]{ { 1, "Gates" }, { 2, "Jobs" }, { 3, "Trump" } };
    pack a2[]{ { 2, 88 }, { 1, 92 }, { 2, 78 }, { 1, 66 }, { 3, 85 }, { 3, 61 } };
    pack3 a3[]{ { "Gates", 79 }, { "Jobs", 83 }, { "Trump", 73 } };
    auto e{ a1 >>
            group_join(a2,
                       [](pack2& a) { return a.index; },
                       [](pack& a) { return a.index; },
                       [](pack& a) { return a.score; },
                       [](pack2& a, auto e) { return pack3{ a.name, e >> average() }; }) };
    test_equals(a3, e);
}

TEST_CASE ("join_test","[group_test]")
{
    pack2 a1[]{ { 1, "Gates" }, { 2, "Jobs" }, { 3, "Trump" } };
    pack a2[]{ { 2, 88 }, { 1, 92 }, { 3, 61 } };
    pack3 a3[]{ { "Gates", 92 }, { "Jobs", 88 }, { "Trump", 61 } };
    auto e{ a1 >>
            join(a2,
                 [](pack2& a) { return a.index; },
                 [](pack& a) { return a.index; },
                 [](pack& a) { return a.score; },
                 [](pack2& a, auto e) { return pack3{ a.name, e }; }) };
    test_equals(a3, e);
}

TEST_CASE ("to_list_test","[to_container_test]")
{
    int a1[]{ 1, 2, 3, 4, 5, 6 };
    list<int> a2{ 2, 4, 6 };
    auto e{ a1 >> where([](int& a) { return a % 2 == 0; }) >> to_list<int>() };
    test_equals(a2, e);
}

TEST_CASE ("to_set_test","[to_container_test]")
{
    int a1[]{ 1, 1, 2, 3, 2, 4, 3, 5, 6, 2, 4, 3, 5, 2, 6, 1 };
    int a2[]{ 2, 4, 6 };
    auto s{ a1 >> to_set<int>() };
    auto e{ s >> where([](int a) { return a % 2 == 0; }) };
    test_equals(a2, e);
}

TEST_CASE ("to_multiset_test","[to_container_test]")
{
    int a1[]{ 1, 1, 2, 3, 2, 4, 3, 5, 6, 2, 4, 3, 5, 2, 6, 1 };
    int a2[]{ 2, 2, 2, 2, 4, 4, 6, 6 };
    auto s{ a1 >> to_multiset<int>() };
    auto e{ s >> where([](int a) { return a % 2 == 0; }) };
    test_equals(a2, e);
}

TEST_CASE ("to_vector_test","[to_container_test]")
{
    int a1[]{ 1, 2, 3, 4, 5, 6 };
    vector<int> a2{ 2, 4, 6 };
    auto e{ a1 >> where([](int& a) { return a % 2 == 0; }) >> to_vector<int>() };
    test_equals(a2, e);
}

TEST_CASE ("to_map_test","[to_container_test]")
{
    int a1[]{ 1, 2, 3 };
    map<int, int> a2{
        { 1, 1 }, { 2, 4 }, { 3, 9 }
    };
    auto e{ a1 >> to_map<int, int>([](int i) { return i; }, [](int i) { return i * i; }) };
    is_true(e >> equals(a2));
}

TEST_CASE ("to_multimap_test","[to_container_test]")
{
    pack a1[]{ { 1, 90 }, { 1, 78 }, { 3, 89 }, { 2, 68 }, { 2, 94 }, { 4, 79 } };
    multimap<int, int> a2{ { 1, 90 }, { 1, 78 }, { 2, 68 }, { 2, 94 }, { 3, 89 }, { 4, 79 } };
    auto e{ a1 >> to_multimap<int, int>([](pack& a) { return a.index; }, [](pack& a) { return a.score; }) };
    is_true(e >> equals(a2));
}
