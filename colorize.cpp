#include <fstream>
#include <iostream>
#include <string>

#include "print_ansi.h"

bool starts_with(const std::string& line, const char* prefix)
{
  return line.find(prefix) == 0;
}

std::string colorize_trailing_whitespace(std::string line, const char *color)
{
  // If there is only a single character it's OK if it's a space.
  // (diffs have their first character either ' ', '-' or '+')
  size_t last = line.length() - 1;
  if (last == 0)
    return line;

  // Find the last non-whitespace character
  size_t i = line.find_last_not_of("\t ");
  if (i == std::string::npos)
  {
    // All whitespace (this shouldn't happen in a diff, but let's
    // entertain the possibility)
    line = color + line;
  }
  else if (i < last)
  {
    // Trailing whitespace.
    line = line.substr(0, i + 1) + color + line.substr(i + 1);
  }

  return line;
}

std::string colorize_line(std::string line)
{
  if (!line.empty())
  {
    line = colorize_trailing_whitespace(line, "\033[41m");

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

  return line;
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
