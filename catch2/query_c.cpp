#include <cmath>
#include <linq/query.hpp>
#include "query_test.hpp"
#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include <catch2/catch.hpp>

#define are_equal(x,y) REQUIRE(x==y)
using namespace std;
using namespace linq;

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
TEST_CASE ("where_test","[where_select_test]")
{
    int a1[]{ 1, 2, 3, 4, 5, 6 };
    int a2[]{ 2, 4, 6 };
    auto e{ a1 >> where([](int& a) { return a % 2 == 0; }) };
    test_equals(a2, e);
}

TEST_CASE ("where_index_test","[where_select_test]")
{
    int a1[]{ 1, 1, 2, 4, 4, 5 };
    int a2[]{ 1, 2, 4, 5 };
    auto e{ a1 >> where_index([](int& a, size_t i) { return a == (int)i; }) };
    test_equals(a2, e);
}

TEST_CASE ("select_test","[where_select_test]")
{
    int a1[]{ 1, 4, 9, 16, 25 };
    double a2[]{ 1, 2, 3, 4, 5 };
    auto e{ a1 >> select([](int& a) { return sqrt(a); }) };
    test_equals(a2, e);
}

TEST_CASE ("select_index_test","[where_select_test]")
{
    int a1[]{ 1, 4, 9, 16, 25 };
    pack a2[]{ { 0, 1 }, { 1, 2 }, { 2, 3 }, { 3, 4 }, { 4, 5 } };
    auto e{ a1 >> select_index([](int& a, size_t i) { return pack{ (int)i, (int)sqrt(a) }; }) };
    test_equals(a2, e);
}

TEST_CASE ("select_many_test","[where_select_test]")
{
    pack a1[]{ { 1, 1 }, { 2, 4 }, { 3, 9 } };
    int a2[]{ 1, 1, 4, 16, 9, 81 };
    auto e{ a1 >> select_many([](pack& a) { return vector<int>{ a.arr[0], a.arr[1] }; }, [](pack&, int a) { return a * a; }) };
    test_equals(a2, e);
}

TEST_CASE ("select_many_index_test","[where_select_test]")
{
    pack a1[]{ { 1, 1 }, { 2, 4 }, { 3, 9 } };
    int a2[]{ 0, 1, 1, 1, 4, 16, 4, 9, 81 };
    auto e{ a1 >> select_many_index([](pack& a, size_t i) { return vector<int>{ (int)i, a.arr[0], a.arr[1] }; }, [](pack&, int a) { return a * a; }) };
    test_equals(a2, e);
}

TEST_CASE ("skip_test","[skip_take_test]")
{
    int a1[]{ 1, 2, 3, 4, 5, 6 };
    int a2[]{ 4, 5, 6 };
    auto e{ a1 >> skip(3) };
    test_equals(a2, e);
}

TEST_CASE ("skip_while_test","[skip_take_test]")
{
    int a1[]{ 1, 2, 3, 4, 5, 6 };
    int a2[]{ 4, 5, 6 };
    auto e{ a1 >> skip_while([](int& i) { return i < 4; }) };
    test_equals(a2, e);
}

TEST_CASE ("skip_while_index_test","[skip_take_test]")
{
    int a1[]{ 1, 2, 3, 4, 5, 6 };
    int a2[]{ 4, 5, 6 };
    auto e{ a1 >> skip_while_index([](int&, size_t i) { return i < 3; }) };
    test_equals(a2, e);
}

TEST_CASE ("take_test","[skip_take_test]")
{
    int a1[]{ 1, 2, 3, 4, 5, 6 };
    int a2[]{ 1, 2, 3 };
    auto e{ a1 >> take(3) };
    test_equals(a2, e);
}

TEST_CASE ("take_while_test","[skip_take_test]")
{
    int a1[]{ 1, 2, 3, 4, 5, 6 };
    int a2[]{ 1, 2, 3 };
    auto e{ a1 >> take_while([](int& i) { return i < 4; }) };
    test_equals(a2, e);
}

TEST_CASE ("take_while_index_test","[skip_take_test]")
{
    int a1[]{ 1, 2, 3, 4, 5, 6 };
    int a2[]{ 1, 2, 3 };
    auto e{ a1 >> take_while_index([](int&, size_t i) { return i < 3; }) };
    test_equals(a2, e);
}

TEST_CASE ("zip_test","[linear_test]")
{
    int a1[]{ 1, 2, 3, 4, 5, 6 };
    int a2[]{ 101, 202, 303, 404, 505, 606 };
    auto e{ a1 >> zip([](int i1, int i2) { return i1 + i2; }, vector<int>{ 100, 200, 300, 400, 500, 600 }) };
    test_equals(a2, e);
}

TEST_CASE ("zip_index_test","[linear_test]")
{
    int a1[]{ 1, 2, 3, 4, 5, 6 };
    int a2[]{ 1, 4, 9, 16, 25, 36 };
    auto e{ a1 >> zip_index([](int i1, int i2, size_t index) { return (int)(i1 + i2 * index); }, vector<int>{ 1, 2, 3, 4, 5, 6 }) };
    test_equals(a2, e);
}

TEST_CASE ("cast_test","[linear_test]")
{
    int a1[]{ 1, 4, 9, 16, 25 };
    int a2[]{ 1, 2, 3, 4, 5 };
    auto e{ a1 >> select([](int& a) { return sqrt(a); }) >> cast<int>() };
    test_equals(a2, e);
}
