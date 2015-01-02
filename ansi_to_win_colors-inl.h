// Assumes _WIN32
#include <cstdint>
#include <utility>
#include <cassert>
#include <cstdio>
#include <windows.h>

/// \brief Mapping table between ANSI color codes per
/// http://en.wikipedia.org/wiki/ANSI_escape_code#Colors
/// and Windows' console colors.
static const int ANSI_WINDOWS_COLORS[] = {
    0,  // Black
    4,  // Red
    2,  // Green
    6,  // Yellow
    1,  // Blue
    5,  // Magenta
    3,  // Cyan
    7   // White 
};

static size_t find_first_bit(uint64_t mask)
{
  for (size_t i = 0; i < sizeof(mask) * 8; ++i)
  {
    if (mask & (uint64_t(1) << i))
      return i;
  }

  assert(false);
  return 0;
}

static int translate_color(uint64_t color_mask, int intensity)
{
   size_t color_code = find_first_bit(color_mask);
   assert(color_code <= 7);
   return intensity | ANSI_WINDOWS_COLORS[color_code];
}

/// \brief Parse ANSI escape sequence into a bit mask.
/// \returns a pair containing ANSI params mask and position of first
/// character after ANSI sequence.
/// If p does not point to a valid escape sequence (by my preference),
/// behavior is undefined.
static std::pair<uint64_t, const char*> parse_ansi_sequence(const char* p)
{
  uint64_t ansi_params = 0;

  // Skip over the two-character "\033[" prefix.
  int param = 0;
  for (p = p + 2; *p != 'm'; ++p)
  {
    if (isdigit(*p))
    {
      param = param * 10 + *p - '0';
    }
    else
    {
      assert(*p == ';');
      assert(param < 63);
      ansi_params |= (uint64_t(1) << param);
      param = 0;
    }
  }

  if (param > 0)
  {
    assert(param < 63);
    ansi_params |= (uint64_t(1) << param);
  }

  return std::make_pair(ansi_params, p);
}

static int translate(uint64_t ansi_params, int default_console_color)
{
  // Reset -- return default color
  if (ansi_params & 1)
    return default_console_color;

  // Intensity/bold
  int intensity = 0;
  if (ansi_params & 2)
    intensity = 1;

  // Foreground color
  static const uint64_t fg_mask = uint64_t(0xFF) << 30;
  uint64_t fg_color = (ansi_params & fg_mask) >> 30;
  if (fg_color > 0) {
    return translate_color(fg_color, intensity * FOREGROUND_INTENSITY);
  }
  
  // Background color
  static const uint64_t bg_mask = uint64_t(0xFF) << 40;
  uint64_t bg_color = (ansi_params & bg_mask) >> 40;
  if (bg_color > 0) {
    int base_color =
        translate_color(bg_color, intensity * BACKGROUND_INTENSITY);
    return base_color << 4;
  }

  // We haven't seen any ANSI params we recognize, so just stay at default.
  return default_console_color;
}

static void ansi_aware_puts(const char* line, int default_console_color)
{
  for (const char* p = line; *p != '\0'; ++p)
  {
    if (*p != '\033')
    {
      putchar(*p);
      continue;
    }

    std::fflush(stdout);

    // An escape sequence, decode it and apply color to console.
    std::pair<uint64_t, const char*> parsed = parse_ansi_sequence(p);
    int color = translate(parsed.first, default_console_color);
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
    p = parsed.second;
  }

  std::putchar('\n');
}
