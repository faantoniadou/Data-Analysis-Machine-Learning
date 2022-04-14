#ifndef EventAction_h
#define EventAction_h 1

#include "G4UserEventAction.hh"
#include "G4Step.hh"

#include <set>

// Something that happens once per event
class EventAction : public G4UserEventAction
{
  public:
    EventAction();
    ~EventAction() override;

    void BeginOfEventAction( const G4Event* ) override;
    void EndOfEventAction( const G4Event* ) override;

    // Our own new method, that lets the stepping action talk to the event action
    void RecordParticleData( const G4Step* );

  private:
    std::set< std::pair< int, int > > m_graphEdges;
    std::set< int > m_bremPhotons;
    std::set< int > m_allParticles;
    std::set< int > m_convPositrons;
    
};

#endif

