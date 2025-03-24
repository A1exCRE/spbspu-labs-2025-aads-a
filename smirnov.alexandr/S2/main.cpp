#include <iostream>
#include "read_expressions.hpp"
#include "conversion.hpp"
#include <list>

int main(int argc, char * argv[])
{
  std::string filename;
  if (argc > 1)
  {
    filename = argv[1];
  }

  std::list< std::string > expressions = smirnov::readExpressions(filename);
  std::list< double > results;

  std::list< std::string >::iterator it;
  for (it = expressions.begin(); it != expressions.end(); ++it)
  {
    std::string expr = *it;
    try
    {
      smirnov::Queue< std::string > postfix = smirnov::infixToPostfix(expr);
      double result = evaluatePostfix(postfix);
      results.push_front(result);
    }
    catch (const std::exception & e)
    {
      std::cerr << e.what() << "\n";
      return 1;
    }
  }
  if (!results.empty())
  {
    std::list< double >::iterator rit = results.begin();
    std::cout << *rit;
    ++rit;
    for (; rit != results.end(); ++rit)
    {
      std::cout << " " << *rit;
    }
  }
  std::cout << "\n";
}
