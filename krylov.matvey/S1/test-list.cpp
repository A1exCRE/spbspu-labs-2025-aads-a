#include <boost/test/unit_test.hpp>
#include "list.hpp"

using namespace krylov;

BOOST_AUTO_TEST_CASE(test_push_back)
{
  List< int > list;
  list.push_back(10);
  BOOST_TEST(list.front() == 10);
  BOOST_TEST(list.back() == 10);
  list.push_back(20);
  BOOST_TEST(list.front() == 10);
  BOOST_TEST(list.back() == 20);
}

BOOST_AUTO_TEST_CASE(test_fill_constructor)
{
  List< int > list(5, 27);
  BOOST_TEST(list.front() == 27);
  BOOST_TEST(list.back() == 27);
  BOOST_TEST(list.size() == 5);
}

BOOST_AUTO_TEST_CASE(test_assign)
{
  List< int > list;
  list.assign(17, 52);
  BOOST_TEST(list.size() == 17);
  BOOST_TEST(list.front() == 52);
  BOOST_TEST(list.back() == 52);
  list.assign(21, 44);
  BOOST_TEST(list.size() == 21);
  BOOST_TEST(list.front() == 44);
  BOOST_TEST(list.back() == 44);
}

BOOST_AUTO_TEST_CASE(test_pop_back)
{
  List< int > list;
  list.push_back(10);
  list.push_back(20);
  list.pop_back();
  BOOST_TEST(list.back() == 10);
  list.pop_back();
  BOOST_TEST(list.empty());
}

BOOST_AUTO_TEST_CASE(test_front_back)
{
  List< int > list;
  list.push_back(42);
  BOOST_TEST(list.front() == 42);
  BOOST_TEST(list.back() == 42);
  list.push_back(34);
  BOOST_TEST(list.front() == 42);
  BOOST_TEST(list.back() == 34);
}

BOOST_AUTO_TEST_CASE(test_empty)
{
  List< int > list;
  BOOST_TEST(list.empty());
  list.push_back(5);
  BOOST_TEST(!list.empty());
}

BOOST_AUTO_TEST_CASE(test_size)
{
  List< int > list;
  BOOST_TEST(list.size() == 0);
  list.push_back(1);
  list.push_back(2);
  BOOST_TEST(list.size() == 2);
}

BOOST_AUTO_TEST_CASE(test_clear)
{
  List< int > list;
  list.push_back(1);
  list.push_back(2);
  BOOST_TEST(!list.empty());
  list.clear();
  BOOST_TEST(list.empty());
}

BOOST_AUTO_TEST_CASE(test_swap)
{
  List< int > list1, list2;
  list1.push_back(10);
  list2.push_back(20);
  list1.swap(list2);
  BOOST_TEST(list1.front() == 20);
  BOOST_TEST(list2.front() == 10);
}

BOOST_AUTO_TEST_CASE(test_iterator_increment)
{
  List< int > list;
  list.push_back(1);
  list.push_back(2);
  list.push_back(3);
  auto it = list.begin();
  BOOST_TEST(*it == 1);
  ++it;
  BOOST_TEST(*it == 2);
  ++it;
  BOOST_TEST(*it == 3);
}

BOOST_AUTO_TEST_CASE(test_iterator_decrement)
{
  List<int> list;
  list.push_back(1);
  list.push_back(2);
  list.push_back(3);
  auto it = list.end();
  --it;
  BOOST_TEST(*it == 3);
  --it;
  BOOST_TEST(*it == 2);
  --it;
  BOOST_TEST(*it == 1);
}
