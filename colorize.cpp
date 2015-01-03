#include <algorithm>
#include <cassert>
#include <fstream>
#include <iostream>
#include <string>

#include "print_ansi.h"

bool is_all_whitespace(const std::string& line)
{
  assert(!line.empty());

  // All lines in an SVN diff are prefixed with a space, + or -
  std::size_t length_without_prefix = line.length() - 1;

  // Empty string has no whitespace
  if (length_without_prefix == 0)
    return false;

  std::string::const_iterator begin = line.begin();
  std::size_t whitespace_count = std::count_if(++begin, line.end(), isspace);
  return (length_without_prefix == whitespace_count);
}

bool starts_with(const std::string& line, const char* prefix)
{
  std::string::const_iterator i = line.begin();
  for (const char* p = prefix; *p != 0; ++p, ++i)
  {
    if (i == line.end())
      return false;

    if (*i != *p)
      return false;
  }

  return true;
}

std::string colorize_line(const std::string& line)
{
  if (!line.empty())
  {
    // Whitespace
    if (is_all_whitespace(line))
      return "\033[41m" + line + "\033[0m";

    // Diff metadata
    if (starts_with(line, "Index: "))
      return "\033[1;37m" + line + "\033[0m";

    if (starts_with(line, "======="))
      return "\033[1;37m" + line + "\033[0m";

    if (starts_with(line, "---"))
      return "\033[1;37m" + line + "\033[0m";

    if (starts_with(line, "+++"))
      return "\033[1;37m" + line + "\033[0m";

    // Change markers
    if (starts_with(line, "@@"))
      return "\033[36m" + line + "\033[0m";

    if (starts_with(line, "-"))
      return "\033[31m" + line + "\033[0m";

    if (starts_with(line, "+"))
      return "\033[32m" + line + "\033[0m";
  }

  return "\033[0m" + line;
}

void colorize(std::istream& stream)
{
  int default_color = get_console_color();
  while (stream)
  {
    std::string line;
    getline(stream, line);
    line = colorize_line(line);

    print_ansi_line(line.c_str(), default_color);
  }
}

int main(int argc, char* argv[])
{
  if (argc > 1)
  {
    std::ifstream f(argv[1]);
    if (!f)
    {
      std::cout << "Cannot open file: '" << argv[1] << "'" << std::endl;
      return 1;
    }

    colorize(f);
  }
  else
  {
    colorize(std::cin);
  }

  return 0;
}
