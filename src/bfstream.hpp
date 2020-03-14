#pragma once
#include <fstream>
#include <string>

class OutputByteStreamer : public std::ofstream {
  public:

    OutputByteStreamer(const char *filename, std::ios_base::openmode mode = ios_base::out) : std::ofstream(filename, mode) {}
    OutputByteStreamer(const std::string &filename, std::ios_base::openmode mode = ios_base::out) : std::ofstream(filename.c_str(), mode) {}

    template<typename T>
    OutputByteStreamer& operator<<(const T& value)
    {

      T value_copy = value;

      if (this->is_open()) {
        this->write(reinterpret_cast<char*>(&value_copy), sizeof(value));
        this->flush();
      }

      return *this;
    }
};


class InputByteStreamer : public std::ifstream {
  public:
    InputByteStreamer(const char *filename, std::ios_base::openmode mode = ios_base::in) : std::ifstream(filename, mode) {}
    InputByteStreamer(const std::string &filename, std::ios_base::openmode mode = ios_base::in) : std::ifstream(filename.c_str(), mode) {}

    template<typename T>
    InputByteStreamer& operator>>(const T& value)
    {
      T value_copy = value;

      if (this->is_open()) {
        this->read(reinterpret_cast<char*>(&value_copy), sizeof(value));
      }

      return *this;
    }
};
