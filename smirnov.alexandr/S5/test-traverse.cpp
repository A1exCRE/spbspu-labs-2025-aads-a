#include <boost/test/unit_test.hpp>
#include <tree/avlTee.hpp>
#include <utils/stack.hpp>
#include <utils/queue.hpp>

struct TestData
{
  void operator()(const std::pair< const int, std::string > & kv)
  {
    keys.push_back(kv.first);
    values.push_back(kv.second);
  }
  std::vector< int > keys;
  std::vector< std::string > values;
};

BOOST_AUTO_TEST_CASE(test_lnr_traversal)
{
  smirnov::AvlTree< int, std::string > tree;
  tree[5] = "five";
  tree[3] = "three";
  tree[7] = "seven";
  tree[2] = "two";
  tree[4] = "four";
  tree[6] = "six";
  tree[8] = "eight";
  TestData data;
  tree.traverse_lnr(data);
  BOOST_TEST(data.keys.size() == 7u);
  BOOST_TEST(data.keys[0] == 2);
  BOOST_TEST(data.keys[1] == 3);
  BOOST_TEST(data.keys[2] == 4);
  BOOST_TEST(data.keys[3] == 5);
  BOOST_TEST(data.keys[4] == 6);
  BOOST_TEST(data.keys[5] == 7);
  BOOST_TEST(data.keys[6] == 8);
  BOOST_TEST(data.values.size() == 7u);
  BOOST_TEST(data.values[0] == "two");
  BOOST_TEST(data.values[1] == "three");
  BOOST_TEST(data.values[2] == "four");
  BOOST_TEST(data.values[3] == "five");
  BOOST_TEST(data.values[4] == "six");
  BOOST_TEST(data.values[5] == "seven");
  BOOST_TEST(data.values[6] == "eight");
}

BOOST_AUTO_TEST_CASE(test_rnl_traversal)
{
  smirnov::AvlTree< int, std::string > tree;
  tree[5] = "five";
  tree[3] = "three";
  tree[7] = "seven";
  tree[2] = "two";
  tree[4] = "four";
  tree[6] = "six";
  tree[8] = "eight";
  TestData data;
  tree.traverse_rnl(data);
  BOOST_TEST(data.keys.size() == 7u);
  BOOST_TEST(data.keys[0] == 8);
  BOOST_TEST(data.keys[1] == 7);
  BOOST_TEST(data.keys[2] == 6);
  BOOST_TEST(data.keys[3] == 5);
  BOOST_TEST(data.keys[4] == 4);
  BOOST_TEST(data.keys[5] == 3);
  BOOST_TEST(data.keys[6] == 2);
  BOOST_TEST(data.values.size() == 7u);
  BOOST_TEST(data.values[0] == "eight");
  BOOST_TEST(data.values[1] == "seven");
  BOOST_TEST(data.values[2] == "six");
  BOOST_TEST(data.values[3] == "five");
  BOOST_TEST(data.values[4] == "four");
  BOOST_TEST(data.values[5] == "three");
  BOOST_TEST(data.values[6] == "two");
}

BOOST_AUTO_TEST_CASE(test_breadth_traversal)
{
  smirnov::AvlTree< int, std::string > tree;
  tree[5] = "five";
  tree[3] = "three";
  tree[7] = "seven";
  tree[2] = "two";
  tree[4] = "four";
  tree[6] = "six";
  tree[8] = "eight";
  TestData data;
  tree.traverse_breadth(data);
  BOOST_TEST(data.keys.size() == 7u);
  BOOST_TEST(data.keys[0] == 5);
  BOOST_TEST(data.keys[1] == 3);
  BOOST_TEST(data.keys[2] == 7);
  BOOST_TEST(data.keys[3] == 2);
  BOOST_TEST(data.keys[4] == 4);
  BOOST_TEST(data.keys[5] == 6);
  BOOST_TEST(data.keys[6] == 8);
  BOOST_TEST(data.values.size() == 7u);
  BOOST_TEST(data.values[0] == "five");
  BOOST_TEST(data.values[1] == "three");
  BOOST_TEST(data.values[2] == "seven");
  BOOST_TEST(data.values[3] == "two");
  BOOST_TEST(data.values[4] == "four");
  BOOST_TEST(data.values[5] == "six");
  BOOST_TEST(data.values[6] == "eight");
}

BOOST_AUTO_TEST_CASE(test_empty_tree_traversal)
{
  smirnov::AvlTree< int, std::string > tree;
  TestData data;
  data = tree.traverse_lnr(data);
  BOOST_TEST(data.keys.empty());
  BOOST_TEST(data.values.empty());
  data = tree.traverse_rnl(data);
  BOOST_TEST(data.keys.empty());
  BOOST_TEST(data.values.empty());
  data = tree.traverse_breadth(data);
  BOOST_TEST(data.keys.empty());
  BOOST_TEST(data.values.empty());
}

BOOST_AUTO_TEST_CASE(test_single_node_traversal)
{
  smirnov::AvlTree< int, std::string > tree;
  tree[1] = "one";
  TestData data;
  data = tree.traverse_lnr(data);
  BOOST_TEST(data.keys.size() == 1u);
  BOOST_TEST(data.keys[0] == 1);
  BOOST_TEST(data.values[0] == "one");
  data.keys.clear();
  data.values.clear();
  data = tree.traverse_rnl(data);
  BOOST_TEST(data.keys.size() == 1u);
  BOOST_TEST(data.keys[0] == 1);
  BOOST_TEST(data.values[0] == "one");
  data.keys.clear();
  data.values.clear();
  data = tree.traverse_breadth(data);
  BOOST_TEST(data.keys.size() == 1u);
  BOOST_TEST(data.keys[0] == 1);
  BOOST_TEST(data.values[0] == "one");
}
