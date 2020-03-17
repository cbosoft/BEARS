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

    int n, i, previous_progress;
    bool printed, timed;
    double delay;
    std::chrono::time_point<CLOCK> start;
    const char *name;

  public:

    ProgressBar(int n)
    {
      this->n = n;
      this->i = 0;
      this->previous_progress = 0;
      this->printed = false;
      this->timed = false;
      this->name = NULL;
    }

    ProgressBar(int n, const char *name)
    {
      this->name = name;
      this->n = n;
      this->i = 0;
      this->previous_progress = 0;
      this->printed = false;
      this->timed = false;
      this->name = NULL;
    }

    ProgressBar(int n, double delay) : delay(delay)
    {
      this->n = n;
      this->printed = false;
      this->timed = true;
      this->name = NULL;
      this->start = CLOCK::now();
      this->i = 0;
      this->previous_progress = 0;
    }

    ProgressBar(int n, const char *name, double delay) : ProgressBar(n, delay)
    {
      this->name = name;
    }

    ~ProgressBar()
    {
      this->finish();
    }


    void update()
    {
      this->update(1);
    }

    void update(int di)
    {
      this-> i += di;
      double fraction = double(this->i) / double(this->n);

      struct winsize ws;
      ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws);
      double w = double(ws.ws_col - 2);


      int progress = int(fraction * w);

      auto now = CLOCK::now();
      double duration = double( (now - this->start).count() ) * CLOCK::period::num / CLOCK::period::den;

      if ( (progress == this->previous_progress) or (this->timed and (duration < this->delay) ) ) return;

      this->previous_progress = progress;

      std::stringstream ss;
      ss << "\r\033[K▉";
      int j = 0;

      if (this->name) {
        ss << this->name;
        j = strlen(this->name);
      }

      for (; j < progress; j++) {
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
