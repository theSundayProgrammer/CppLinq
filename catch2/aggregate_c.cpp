#include <iostream>
#include <linq/aggregate.hpp>
#include <linq/query.hpp>
#include <linq/to_container.hpp>


#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include <catch2/catch.hpp>
using namespace std;
using namespace linq;
    struct pack {
        std::string name;
        int score;
    };
inline bool operator==(const pack& p1, const pack& p2) {
    return p1.name == p2.name && p1.score == p2.score;
}
inline bool operator!=(const pack& p1, const pack& p2) {
  return !(p1 == p2);
}
#define are_equal(x,y) REQUIRE(x==y)
#define is_true(x) REQUIRE(x)
template<class S, class T>
void test_equals(S a[], T&& e){
  int i=0;
  for(auto const & item:e)
  {
    are_equal(a[i],item);
    ++i;
  }
}
TEST_CASE( "Array Count","[count_test]" ) {
    int a1[]{ 1, 2, 3, 4, 5, 6 };
    auto c1{ a1 >> count() };
    REQUIRE(6ULL== c1);
    auto c2{ a1 >> count([](int& a) { return a % 2 == 0; }) };
    REQUIRE(3ULL==c2);
}

TEST_CASE( "Array All Any ","[all_any]" ) 
{
    int a1[]{ 1, 2, 3, 4, 5, 6 };
    bool b1{ a1 >> any([](int& a) { return a % 2 == 0; }) };
    bool b2{ a1 >> all([](int& a) { return a < 7; }) };
    REQUIRE(b1);
    REQUIRE(b2);
}

TEST_CASE( "aggregate_method_test","[aggregate_test]")
{
    int a1[]{ 1, 2, 3, 4, 5 };
    auto i1{ a1 >> average() };
    are_equal(3, i1);
    auto i2{ a1 >> sum() };
    are_equal(15, i2);
    auto i3{ a1 >> aggregate(0, [](int& i1, int& i2) { return i1 + i2 * i2; }) };
    are_equal(55, i3);
}
TEST_CASE ("reverse_test","[aggregate_test]")
{
    int a1[]{ 1, 2, 3, 4, 5, 6 };
    int a2[]{ 6, 5, 4, 3, 2, 1 };
    auto e= a1 >> reverse();
    test_equals(a2, e);
}

TEST_CASE ("sort_test","[aggregate_test]")
{
    int a1[]{ 6, 5, 4, 3, 2, 1 };
    int a2[]{ 1, 2, 3, 4, 5, 6 };
    auto e{ a1 >> sort() };
    test_equals(a2, e);
}

TEST_CASE ("sort_custom_test","[aggregate_test]")
{
    pack a1[]{ { "Zh.F. Ren", 92 }, { "Jobs", 78 }, { "Trump", 85 }, { "Gates", 85 } };
    pack a2[]{ { "Zh.F. Ren", 92 }, { "Gates", 85 }, { "Trump", 85 }, { "Jobs", 78 } };
    auto e{ a1 >>
            sort(make_comparer([](pack& p) { return p.score; }, descending{}),
                 make_comparer([](pack& p) { return p.name; }, string_ascending<char>{})) };
    test_equals(a2, e);
}

TEST_CASE ("sorter_test","[aggregate_test]")
{
    list<pack> a1{ { "Zh.F. Ren", 92 }, { "Jobs", 78 }, { "Trump", 85 }, { "Gates", 85 } };
    pack a2[]{ { "Zh.F. Ren", 92 }, { "Gates", 85 }, { "Trump", 85 }, { "Jobs", 78 } };
    a1.sort(make_sorter(make_comparer([](pack& p) { return p.score; }, descending{}),
                        make_comparer([](pack& p) { return p.name; }, string_ascending<char>{})));
    test_equals(a2, a1);
}


TEST_CASE ("min_max_test","[aggregate_test]")
{
    int a1[]{ 3, 4, 2, 6, 1, 5 };
    auto minimum{ a1 >> min() };
    are_equal(1, minimum);
    auto maximum{ a1 >> max(7) };
    are_equal(7, maximum);
}

TEST_CASE ("for_each_test","[aggregate_test]")
{
    int a1[]{ 1, 2, 3 };
    int a2[]{ 2, 4, 6 };
    a1 >> for_each([](int& i) { i *= 2; });
    test_equals(a2, a1);
}

TEST_CASE ("for_each_index_test","[aggregate_test]")
{
    int a1[]{ 1, 2, 3 };
    int a2[]{ 0, 2, 6 };
    a1 >> for_each_index([](int& i, size_t index) { i *= (int)index; });
    test_equals(a2, a1);
}

TEST_CASE ("peek_test","[aggregate_test]")
{
    int a1[]{ 1, 2, 3 };
    int a2[]{ 2, 4, 6 };
    auto e{ a1 >> peek([](int& i) { i *= 2; }) };
//    test_noteqs(a2, a1);
    test_equals(a2, e);
}

TEST_CASE ("peek_index_test","[aggregate_test]")
{
    int a1[]{ 1, 2, 3 };
    int a2[]{ 0, 2, 6 };
    auto e{ a1 >> peek_index([](int& i, size_t index) { i *= (int)index; }) };
    //test_noteqs(a2, a1);
    test_equals(a2, e);
}

TEST_CASE ("get_at_test","[find_test]")
{
    int a1[]{ 1, 2, 3, 4, 5, 6 };
    are_equal(3, a1 >> get_at(2));
    are_equal(1, get_enumerable<int*>(nullptr, nullptr) >> get_at(2, 1));
}

TEST_CASE ("index_of_test","[find_test]")
{
    int a1[]{ 1, 2, 3, 4, 5, 6 };
    are_equal(3ULL, a1 >> index_of([](int& a) { return a == 4; }));
}

TEST_CASE ("front_back_test","[find_test]")
{
    int a1[]{ 3, 4, 2, 6, 1, 5 };
    are_equal(3, a1 >> front());
    are_equal(4, a1 >> front([](int& i) { return i % 2 == 0; }, 0));
    are_equal(7, a1 >> front([](int& i) { return i > 7; }, 7));
    are_equal(5, a1 >> back());
    are_equal(6, a1 >> back([](int& i) { return i % 2 == 0; }, 0));
}

TEST_CASE ("default_test","[find_test]")
{
    int a1[]{ 0 };
    auto e1{ get_enumerable<int*>(nullptr, nullptr) >> default_if_empty() };
    test_equals(a1, e1);
    int a2[]{ 1 };
    auto e2{ get_enumerable<int*>(nullptr, nullptr) >> default_if_empty(1) };
    test_equals(a2, e2);
}

TEST_CASE ("single_test","[find_test]")
{
    {
        int a1[]{ 1, 2, 3 };
        auto e{ a1 >> single([](int& i) { return i % 2 == 0; }) };
        are_equal(2, e);
    }
    {
        int a1[]{ 1, 3, 5 };
        auto e{ a1 >> single([](int& i) { return i % 2 == 0; }, 2) };
        are_equal(2, e);
    }
    {
        int a1[]{ 2, 4, 6 };
        REQUIRE_THROWS_AS(  a1 >> single([](int& i) { return i % 2 == 0; },0),more_than_one);
    }
}
TEST_CASE ("distinct_test","[set_test]")
{
    int a1[]{ 1, 1, 2, 3, 3, 4, 5, 6 };
    int a2[]{ 1, 2, 3, 4, 5, 6 };
    auto e{ a1 >> distinct() };
    test_equals(a2, e);
}

TEST_CASE ("union_set_test","[set_test]")
{
    int a1[]{ 1, 1, 2, 3, 3, 4, 5, 6 };
    int a2[]{ 3, 4, 5, 6, 7, 7, 8 };
    int a3[]{ 1, 2, 3, 4, 5, 6, 7, 8 };
    auto e{ a1 >> union_set(a2) };
    test_equals(a3, e);
}

TEST_CASE ("intersect_test","[set_test]")
{
    int a1[]{ 1, 1, 2, 3, 3, 4, 5, 6 };
    int a2[]{ 3, 4, 5, 6, 7, 7, 8 };
    int a3[]{ 3, 4, 5, 6 };
    auto e{ a1 >> intersect(a2) };
    test_equals(a3, e);
}

TEST_CASE ("except_test","[set_test]")
{
    int a1[]{ 1, 1, 2, 3, 3, 4, 5, 6 };
    int a2[]{ 3, 4, 5, 6, 7, 7, 8 };
    int a3[]{ 1, 2 };
    auto e{ a1 >> except(a2) };
    test_equals(a3, e);
}
