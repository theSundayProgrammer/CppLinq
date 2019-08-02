#include <linq/core.hpp>
#include <linq/query.hpp>

#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include <catch2/catch.hpp>
using namespace std;
using namespace linq;
#define are_equal(x,y) REQUIRE(x==y)
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

TEST_CASE ("test","[enumerable_test]")
{
    int a1[]{ 1, 2, 3, 4, 5, 6 };
    auto e1{ get_enumerable(a1) };
    auto e2{ get_enumerable(vector<int>{ 1, 2, 3, 4, 5, 6 }) };
    auto e3{ get_enumerable(begin(a1), end(a1)) };
    test_equals(e2, e1);
    test_equals(e3, e2);
}

TEST_CASE ("range_test","[construct_test]")
{
    int a1[]{ 1, 2, 3, 4 };
    auto e1{ range(1, 5) };
    test_equals(a1, e1);

    int a2[]{ 2, 4, 6, 8 };
    auto e2{ range(2, 10, [](int i) { return i + 2; }) };
    test_equals(a2, e2);
}

TEST_CASE ("repeat_test","[construct_test]")
{
    int a2[]{ 1, 1, 1, 1, 1 };
    auto e{ repeat(1, 5) };
    test_equals(a2, e);
}

TEST_CASE ("concat_test","[calculate_test]")
{
    int a1[]{ 1, 2, 3, 4, 5, 6 };
    int a2[]{ 1, 3, 5, 2, 4, 6 };
    auto e{ a1 >> where([](int& a) { return a % 2 != 0; }) >> concat(a1 >> where([](int& a) { return a % 2 == 0; })) };
    test_equals(a2, e);
}

TEST_CASE ("pend_test","[calculate_test]")
{
    int a1[]{ 2, 3, 4, 5 };
    int a2[]{ 1, 2, 3, 4, 5, 6 };
    auto e{ a1 >> prepend(1) >> append(6) };
    test_equals(a2, e);
}

TEST_CASE ("insert_item_test","[calculate_test]")
{
    {
        int a1[]{ 1, 2, 3, 4, 5, 6 };
        int a2[]{ 1, 2, 3, 0, 4, 5, 6 };
        auto e{ a1 >> insert(0, 3) };
        test_equals(a2, e);
    }
    {
        int a1[]{ 1, 2, 3, 4, 5, 6 };
        int a2[]{ 1, 2, 3, 4, 5, 6, 0 };
        auto e{ a1 >> insert(0, 6) };
        test_equals(a2, e);
    }
}

TEST_CASE ("insert_enumerable_test","[calculate_test]")
{
    {
        int a1[]{ 1, 2, 3, 4, 5, 6 };
        int a2[]{ 1, 2, 3, 0, 0, 0, 4, 5, 6 };
        auto e{ a1 >> insert(vector<int>{ 0, 0, 0 }, 3) };
        test_equals(a2, e);
    }
    {
        int a1[]{ 1, 2, 3, 4, 5, 6 };
        int a2[]{ 1, 2, 3, 4, 5, 6, 0, 0, 0 };
        auto e{ a1 >> insert(vector<int>{ 0, 0, 0 }, 6) };
        test_equals(a2, e);
    }
}
