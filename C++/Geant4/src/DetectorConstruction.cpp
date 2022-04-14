#include "DetectorConstruction.h"

#include "G4Material.hh"
#include "G4NistManager.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4AutoDelete.hh"
#include "G4GeometryManager.hh"
#include "G4SystemOfUnits.hh"

G4ThreadLocal
G4GlobalMagFieldMessenger* DetectorConstruction::m_magneticFieldMessenger = 0;

DetectorConstruction::DetectorConstruction() : G4VUserDetectorConstruction()
{
}

DetectorConstruction::~DetectorConstruction()
{
}

// Here we define the actual experiment that we want to perform
G4VPhysicalVolume* DetectorConstruction::Construct()
{
  // Materials
  // http://geant4-userdoc.web.cern.ch/geant4-userdoc/UsersGuides/ForApplicationDeveloper/html/Appendix/materialNames.html
  G4NistManager* nistManager = G4NistManager::Instance();
  G4Material* air = nistManager->FindOrBuildMaterial( "G4_AIR" );
  G4Material* aluminium = nistManager->FindOrBuildMaterial( "G4_Al" );

  // Sizes of the principal geometrical components (solids)
  G4double targetThickness = 50.0*cm;
  G4double targetRadius = 50.0*cm;
  G4double worldLength = 250.0*cm;

  // Definitions of Solids, Logical Volumes, Physical Volumes

  // WORLD: Solid (cube)
  G4GeometryManager::GetInstance()->SetWorldMaximumExtent( worldLength );
  G4Box* worldS = new G4Box(
                 "World",         // its name
                 worldLength,
                 worldLength,
                 worldLength );   // its size (in half-lengths)

  // WORLD: Logical volume (how to treat it)
  G4LogicalVolume* worldLV = new G4LogicalVolume(
                 worldS,          // its solid
                 air,             // its material
                 "World" );       // its name

  // WORLD: Physical volume (where is it)
  // Must place the World Physical volume unrotated at (0,0,0).
  G4VPhysicalVolume* worldPV = new G4PVPlacement(
                 0,               // no rotation
                 G4ThreeVector(0.0, 0.0, 0.0), // in the centre
                 worldLV,         // its logical volume
                 "World",         // its name
                 0,               // its mother volume
                 false,           // no boolean operations
                 0,               // copy number
                 true );          // checking overlaps

  // TARGET: Solid (tube)
  G4Tubs* targetS = new G4Tubs(
                 "Target",        // its name
                 0.0,             // inner radius 0, so it's a solid cylinder (not a hollow tube)
                 targetRadius,    // outer radius
                 targetThickness, // how much material in the beam path (half length)
                 0.0*deg,         // starting angle
                 360.0*deg );     // ending angle (i.e. it's a full circle)

  // TARGET: Logical volume (how to treat it)
  G4LogicalVolume* targetLV = new G4LogicalVolume(
                 targetS,         // its solid
                 aluminium,       // its material
                 "Target",        // its name
                 0, 0, 0 );       // Modifiers we don't use

  // TARGET: Physical volume (where is it)
  G4VPhysicalVolume* targetPV = new G4PVPlacement(
                 0,               // no rotation
                 G4ThreeVector(0.0, 0.0, 0.0), // in the centre
                 targetLV,        // its logical volume
                 "Target",        // its name
                 worldLV,         // its mother volume
                 false,           // no boolean operations
                 0,               // copy number
                 true );          // checking overlaps

  // TARGET: Warn if there's an overlap
  if ( targetPV->CheckOverlaps() ) std::cerr << "WARNING: your simulated objects overlap" << std::endl;

  // Always return the physical world
  return worldPV;
}

// Set up the magnetic field
void DetectorConstruction::ConstructSDandField()
{
  // Create global magnetic field messenger.
  // Uniform magnetic field is then created automatically if
  // the field value is not zero.
  G4ThreeVector fieldValue = G4ThreeVector();
  m_magneticFieldMessenger = new G4GlobalMagFieldMessenger( fieldValue );

  // Register the field messenger for deleting
  G4AutoDelete::Register( m_magneticFieldMessenger );
}
