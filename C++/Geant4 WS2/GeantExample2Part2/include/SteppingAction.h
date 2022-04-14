#ifndef SteppingAction_h
#define SteppingAction_h 1

#include "G4UserSteppingAction.hh"
#include "EventAction.h"

// Collect information each step, send it to the event action
class SteppingAction : public G4UserSteppingAction
{
  public:
    SteppingAction( EventAction* eventAction );
    ~SteppingAction() override;

    void UserSteppingAction( const G4Step* step ) override;

  private:
    EventAction* m_eventAction;
};

#endif
