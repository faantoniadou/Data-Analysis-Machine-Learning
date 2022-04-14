#ifndef EnergyCounter_h
#define EnergyCounter_h 1

#include "G4VSensitiveDetector.hh"

class EnergyCounter : public G4VSensitiveDetector
{
  public:
    EnergyCounter( const G4String& name, const G4int id );
    ~EnergyCounter() override;

    void Initialize( G4HCofThisEvent* ) override;
    G4bool ProcessHits( G4Step* step, G4TouchableHistory* ) override;
    void EndOfEvent( G4HCofThisEvent* ) override;
    
    void RecordParticleData( const G4Step* );


  private:
    G4double m_totalEnergy;
    G4int m_ID;
    G4double m_electronEnergy;
};

#endif
