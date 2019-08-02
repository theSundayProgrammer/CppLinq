#include <linq/string.hpp>

#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include <catch2/catch.hpp>

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
using namespace std;
using namespace linq;
#define is_true(x) REQUIRE(x)
#define is_false(x) REQUIRE(x==false)
TEST_CASE ("split_test","[string_test]")
{
    string str{ "Hello world !" };
    string_view views[]{ "Hello", "world", "!" };
    auto e{ str >> split(' ') };
    test_equals(views, e);
}

TEST_CASE ("joinstr_test","[string_test]")
{
    {
        string str{ "123456" };
        int a1[]{ 1, 2, 3, 4, 5, 6 };
        auto s{ a1 >> joinstr<char>() };
        are_equal(str, s);
    }
    {
        string str{ "Hello world !" };
        string_view views[]{ "Hello", "world", "!" };
        auto s{ views >> joinstr<char>(' ') };
        are_equal(str, s);
    }
}

TEST_CASE ("instr_test","[string_test]")
{
    is_true("Hello world!" >> instr<char>('o'));
    is_true("Hello world!" >> instr<char>("world"));
}

TEST_CASE ("replace_test","[string_test]")
{
    string str{ "Hello world!o" };
    string_view str2{ "Hellooo wooorld!ooo" };
    auto e{ str >> replace('o', "ooo") };
    are_equal(str2, e);
}

TEST_CASE ("remove_test","[string_test]")
{
    string str{ "Hello world!o" };
    string_view str2{ "Hell wrld!" };
    auto e{ str >> remove<char>("o") };
    are_equal(str2, e);
    auto e2{ str >> remove('o') };
    are_equal(str2, e2);
}

constexpr string_view test_str{ "123456" };

TEST_CASE ("starts_with_test","[string_test]")
{
    is_true(test_str >> starts_with('1'));
    is_true(test_str >> starts_with<char>("123"));
    is_false(test_str >> starts_with('2'));
    is_false(test_str >> starts_with<char>("456"));
}

TEST_CASE ("ends_with_test","[string_test]")
{
    is_true(test_str >> ends_with('6'));
    is_true(test_str >> ends_with<char>("456"));
    is_false(test_str >> ends_with('5'));
    is_false(test_str >> ends_with<char>("123"));
}

TEST_CASE ("trim_test","[string_test]")
{
    string str{ "   123456   " };
    auto tl{ str >> trim_left<char>() };
    are_equal("123456   ", tl);
    auto tr{ str >> trim_right<char>() };
    are_equal("   123456", tr);
    auto tall{ str >> trim<char>() };
    are_equal("123456", tall);
}

TEST_CASE ("io_lines_test","[string_test]")
{
    string str{ R"(Twinkle, twinkle, little star,
How I wonder what you are.
Up above the world so high,
Like a diamond in the sky.
)" };
    istringstream iss{ str };
    ostringstream oss;
    write_lines(oss, read_lines(iss));
    are_equal(str, oss.str());
}
