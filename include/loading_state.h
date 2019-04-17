#ifndef LOAD_STATE_H
#define LOAD_STATE_H

#include "state.h"
#include "pallete.h"

class LoadingState : public State {
    public:
      LoadingState(Engine* engine, ErrorHandler* errorHandler);

      virtual ~LoadingState();
      virtual void load();
      void advance(); 
      void start();
      void end();

    private:
      virtual void setup();
      void update(double);

      int num_states;

      std::vector<double> ratios;

      bool is_saved = false;

      std::ofstream of;

      int totalTime = 0;

      void saveRatios();

      unsigned int segmentTime;

      double width_ratio = 0;
};

#endif
