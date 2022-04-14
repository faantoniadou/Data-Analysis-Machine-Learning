#include "GeneratorAction.h"

#include "G4ParticleTable.hh"
#include "G4SystemOfUnits.hh"


GeneratorAction::GeneratorAction() : G4VUserPrimaryGeneratorAction()
{
  G4int nofParticles = 1;
  m_particleGun = new G4ParticleGun( nofParticles );

  // Default particle
  G4ParticleDefinition* particleDefinition = G4ParticleTable::GetParticleTable()->FindParticle( "proton" );
  m_particleGun->SetParticleDefinition( particleDefinition );
  m_particleGun->SetParticlePosition( G4ThreeVector( 0.0, 0.0, -250.0*cm ) ); // right in the middle
  m_particleGun->SetParticleMomentumDirection( G4ThreeVector( 0.0, 0.0, 1.0 ) ); // along z axis
  m_particleGun->SetParticleEnergy( 3.0*GeV );
}

GeneratorAction::~GeneratorAction()
{
  delete m_particleGun;
}

// This function is called at the begining of event
void GeneratorAction::GeneratePrimaries( G4Event* anEvent )
{
  // Fire a particle
  m_particleGun->GeneratePrimaryVertex( anEvent );
}
