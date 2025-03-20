#include "input.hpp"

karnauhova::Queue< std::string > karnauhova::split_str(std::string str)
{
  karnauhova::Queue< std::string > inf;
  std::string element;
  char separator = ' ';
  int i = 0; 
  while (str[i] != '\0')
  {
    if (str[i] != separator)
    {
    element += str[i]; 
    }
    else
    {
      inf.push(element);
      element.clear();
    }
    i++;
  }
  inf.push(element);
  return inf;
}


karnauhova::Queue< std::string > karnauhova::to_post(karnauhova::Queue< std::string > inf)
{
  karnauhova::Stack< std::string > symbols;
  karnauhova::Queue< std::string > post;
  while (!inf.empty())
  {
    std::string element = inf.front();
    if (element == "+" || element == "/" || element == "-" || element == "*" || element == "%")
    {
      if (!symbols.empty())
      {
        std::string last = symbols.top();
        if (last == "+" || last == "/" || last == "-" || last == "*" || last == "%")
        {
          post.push(last);
          symbols.pop();
        }
      }
      symbols.push(element);
      inf.pop();
      continue;
    }
    if (element == "(")
    {
      symbols.push(element);
      inf.pop();
      continue;
    }
    if (element == ")")
    {
      if (symbols.empty())
      {
        throw std::logic_error("Incorrect 1");
      }
      std::string last = symbols.top();
      if (last != "(" || last != "+" || last != "/" || last != "-" || last != "*" || last != "%")
      {
        throw std::logic_error("Incorrect 2");
      }
      if (last != "(")
      {
        post.push(last);
        symbols.pop();
      }
      symbols.push(element);
      inf.pop();
      continue;
    }
    try
    {
      std::stoll(element);
    }
    catch(const std::exception& e)
    {
      throw std::logic_error("No number");
    }
    post.push(element);
    inf.pop();
  }
  if (symbols.empty())
  {
    throw std::logic_error("Incorrect 3");
  }
  post.push(symbols.top());
  symbols.pop();
  if (!symbols.empty())
  {
    throw std::logic_error("Error in input");
  }
  return post;
}