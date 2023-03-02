#include <cmath>
#include <exception>
#include <iostream>
#include <thread>

#include "fmt/chrono.h"
#include "fmt/color.h"
#include "fmt/core.h"
#include "fmt/format.h"

// clang-format off
#pragma warning( push )
#pragma warning( disable : 5054 )
#pragma warning( disable : 4263 )
#pragma warning( disable : 4127 )
#include "opencv2/opencv.hpp"
#pragma warning( pop )

// clang-format on

int main(int argc, char** argv) {
  std::ios_base::sync_with_stdio(false);

  if (argc < 2) {
    fmt::print("Invalid program args. It expects path to mp4 video as its only parameter.");
    return -1;
  }

  const std::string path_to_video = argv[1];

  // open video file
  cv::VideoCapture video_capture(path_to_video);
  if (!video_capture.isOpened()) {
    fmt::print("Failed to open {}\n", path_to_video);
    std::fflush(nullptr);
    return -1;
  }

  constexpr size_t width = 100;
  constexpr size_t height = 60;

  std::cout << "\x1B[3;" << width << "h";

  const auto carriage_up_and_return = [](size_t n) { fmt::print("\033[{}A", n); };

  std::string buffer;
  buffer.reserve(width * height * 20);

  const double fps = video_capture.get(cv::CAP_PROP_FPS);

  const auto frame_duration = std::chrono::seconds(1) / fps;
  const auto time_start = std::chrono::high_resolution_clock::now();

  // loop through video frames
  cv::Mat frame;
  size_t frame_index = 0;
  while (video_capture.read(frame)) {
    // resize frame
    resize(frame, frame, cv::Size(width, height), cv::INTER_LINEAR);

    buffer.clear();
    auto buffer_inserter = std::back_inserter(buffer);

    // write frame pixels to text buffer
    for (size_t y = 0; y != height; ++y) {
      for (size_t x = 0; x != width; ++x) {
        auto pixel = frame.at<cv::Vec3b>(static_cast<int>(y), static_cast<int>(x));

        auto b = pixel[0];
        auto g = pixel[1];
        auto r = pixel[2];

        const auto foreground = fmt::bg(fmt::rgb(r, g, b));
        fmt::format_to(buffer_inserter, foreground, " ");
      }
      fmt::format_to(buffer_inserter, "\n");
    }

    printf("%s", buffer.c_str());
    carriage_up_and_return(height);

    const auto expected_time = time_start + (frame_index + 1) * frame_duration;
    while (std::chrono::high_resolution_clock::now() < expected_time) {
    }

    ++frame_index;
  }

  return 0;
}