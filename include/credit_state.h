#ifndef CREDIT_STATE_H
#define CREDIT_STATE_H

#include "state.h"

class CreditState : public State {
    public:
      CreditState(Engine* engine, ErrorHandler* errorHandler);

      virtual ~CreditState();

      void update(double seconds);

      void load();
      
      void setup();
};

#endif
