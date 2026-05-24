#pragma once

#include <algorithm>
#include <array>
#include <cstdint>
#include <cstdio>
#include <string>
#include <vector>

namespace hpower {
constexpr size_t kFrameLength = 22;
constexpr size_t kMaxBufferLength = 96;
constexpr uint8_t kPrefix0 = 0x65;
constexpr uint8_t kPrefix1 = 0x6F;
constexpr uint8_t kPrefix2 = 0xDE;

constexpr uint8_t kPumpRunningMask = 0x80;
constexpr uint8_t kPumpModeOnMask = 0x40;
constexpr uint8_t kPumpModeOffMask = 0x20;
constexpr uint8_t kLightOnMask = 0x10;
constexpr uint8_t kLightModeOnMask = 0x08;
constexpr uint8_t kLightModeOffMask = 0x04;

struct Frame {
  std::array<uint8_t, kFrameLength> bytes;
};

inline std::string hex(const uint8_t *data, size_t length) {
  static constexpr char chars[] = "0123456789ABCDEF";
  std::string out;
  out.reserve(length * 2);

  for (size_t i = 0; i < length; i++) {
    out.push_back(chars[(data[i] >> 4) & 0x0F]);
    out.push_back(chars[data[i] & 0x0F]);
  }

  return out;
}

inline void append_bytes(std::vector<uint8_t> &buffer, const std::vector<uint8_t> &incoming) {
  if (incoming.empty()) {
    return;
  }

  if (incoming.size() > kMaxBufferLength || buffer.size() + incoming.size() > kMaxBufferLength) {
    buffer.clear();
  }

  const size_t available = kMaxBufferLength - buffer.size();
  const size_t copy_length = incoming.size() <= available ? incoming.size() : available;
  buffer.insert(buffer.end(), incoming.begin(), incoming.begin() + copy_length);
}

inline int find_prefix(const std::vector<uint8_t> &buffer) {
  if (buffer.size() < 3) {
    return -1;
  }

  for (size_t i = 0; i <= buffer.size() - 3; i++) {
    if (buffer[i] == kPrefix0 && buffer[i + 1] == kPrefix1 && buffer[i + 2] == kPrefix2) {
      return static_cast<int>(i);
    }
  }

  return -1;
}

inline std::vector<Frame> extract_frames(std::vector<uint8_t> &buffer, const std::vector<uint8_t> &incoming) {
  append_bytes(buffer, incoming);

  std::vector<Frame> frames;
  while (buffer.size() >= 3) {
    const int prefix_index = find_prefix(buffer);
    if (prefix_index < 0) {
      if (buffer.size() > 2) {
        buffer.erase(buffer.begin(), buffer.end() - 2);
      }
      return frames;
    }

    if (prefix_index > 0) {
      buffer.erase(buffer.begin(), buffer.begin() + prefix_index);
    }

    if (buffer.size() < kFrameLength) {
      return frames;
    }

    Frame frame{};
    std::copy_n(buffer.begin(), kFrameLength, frame.bytes.begin());
    frames.push_back(frame);
    buffer.erase(buffer.begin(), buffer.begin() + kFrameLength);
  }

  return frames;
}

inline const char *pump_mode(uint8_t tail) {
  if ((tail & kPumpModeOnMask) != 0) {
    return "On";
  }
  if ((tail & kPumpModeOffMask) != 0) {
    return "Off";
  }
  return "Schedule";
}

inline const char *light_mode(uint8_t tail) {
  if ((tail & kLightModeOnMask) != 0) {
    return "On";
  }
  if ((tail & kLightModeOffMask) != 0) {
    return "Off";
  }
  return "Schedule";
}

inline bool pump_running(uint8_t tail) {
  return (tail & kPumpRunningMask) != 0;
}

inline bool light_on(uint8_t tail) {
  return (tail & kLightOnMask) != 0;
}
}  // namespace hpower
