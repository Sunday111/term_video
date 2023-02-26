#include <cmath>
#include <iostream>
#include <thread>

#include "fmt/chrono.h"
#include "fmt/color.h"
#include "fmt/core.h"
#include "fmt/format.h"
#include "fmt/printf.h"

int main() {
  std::ios_base::sync_with_stdio(false);

  constexpr size_t width = 150;
  constexpr size_t height = 80;
  constexpr size_t frames = 60;

  std::cout << "\x1B[3;" << 120 << "h";

  const auto carriage_up_and_return = [](size_t n) { fmt::print("\033[{}A", n); };

  std::string buffer;
  buffer.reserve(width * height * 20);

  const auto t0 = std::chrono::high_resolution_clock::now();
  for (size_t f = 0; f != frames; ++f) {
    buffer.clear();
    auto buffer_inserter = std::back_inserter(buffer);

    const auto k = static_cast<uint8_t>(std::abs(std::sin(static_cast<float>(f) / 300.f)) * 255.0);

    for (size_t y = 0; y != height; ++y) {
      for (size_t x = 0; x != width; ++x) {
        const auto foreground = fmt::fg(fmt::rgb(static_cast<uint8_t>(x), static_cast<uint8_t>(y), k));
        fmt::format_to(buffer_inserter, foreground, "O");
      }
      fmt::format_to(buffer_inserter, "\n");
    }
    printf("%s", buffer.c_str());
    carriage_up_and_return(height);
  }

  const auto t1 = std::chrono::high_resolution_clock::now();
  const auto dt = std::chrono::duration_cast<std::chrono::milliseconds>(t1 - t0);
  fmt::print("time: {}", dt);

  return 0;
}