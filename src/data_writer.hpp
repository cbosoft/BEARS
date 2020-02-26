#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <iomanip>
#include <list>
#include <utility>

#include "exception.hpp"
#include "formatter.hpp"

class DataWriter {

  private:
    
    std::ofstream of;
    std::string separator;
    bool needs_sep;

  public:

    DataWriter(std::string path, std::string separator)
    {
      this->of.open(path, std::ios::trunc);

      if (!this->of.is_open()) {
        throw IOError(Formatter() << "Error opening file ('" << path << "')", true);
      }

      this->needs_sep = false;
      this->separator = separator;
    }

    ~DataWriter();

    virtual std::string format_field(std::string) const =0;

    template<typename T>
    DataWriter& operator<<(T obj)
    {
      this->of << obj;

      if (this->needs_sep) {
        this->of << this->separator;
      }
      else {
        this->needs_sep = true;
      }

      return *this;
    }

    void endl()
    {
      this->of<< std::endl;
      this->needs_sep = false;
    }

};

class TSVWriter : public DataWriter {

  public:

    TSVWriter(std::string path) : DataWriter(path, "\t") {}

    std::string format_field(std::string s) const override { return s; }

};
