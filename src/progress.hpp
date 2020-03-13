#pragma once
#include <sys/ioctl.h>
#include <unistd.h>
#include <sstream>
#include <iostream>
#include <chrono>
#include <cstring>

#define CLOCK std::chrono::steady_clock

class ProgressBar {
  private:

    int i, n, pp;
    bool printed, timed;
    double delay;
    std::chrono::time_point<CLOCK> start;
    const char *name;

  public:

    inline ProgressBar(int n) : n(n)
    {
      this->printed = false;
      this->timed = false;
      this->name = NULL;
    }

    inline ProgressBar(int n, const char *name) : ProgressBar(n)
    {
      this->name = name;
    }

    inline ProgressBar(int n, double delay) : delay(delay)
    {
      this-> n = n;
      this->printed = false;
      this->timed = true;
      this->name = NULL;
      this->start = CLOCK::now();
    }

    inline ProgressBar(int n, const char *name, double delay) : ProgressBar(n, delay)
    {
      this->name = name;
    }

    inline ~ProgressBar()
    {
      this->finish();
    }


    inline void update()
    {
      this->update(1);
    }

    inline void update(int di)
    {
      this-> i += di;
      int perc = this->i * 100;
      perc /= this->n;

      struct winsize ws;
      ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws);
      int w = ws.ws_col - 2;


      int p = perc*w;
      p /= 100;

      auto now = CLOCK::now();
      double duration = double( (now - this->start).count() ) * CLOCK::period::num / CLOCK::period::den;

      if ( (p == this->pp) or (this->timed and (duration < this->delay) ) ) return;

      this->pp = p;

      std::stringstream ss;
      ss << "\r\033[K▉";
      int j = 0;

      if (this->name) {
        ss << this->name;
        j = strlen(this->name);
      }

      for (; j < p; j++) {
        ss << "▉";
      }

      for (; j < w; j++) {
        ss << "─";
      }
      ss << "▎";
      std::cerr << ss.str();
      this->printed = true;
    }

    inline void finish() const
    {
      if (this->printed)
        std::cerr << std::endl;
    }
};
