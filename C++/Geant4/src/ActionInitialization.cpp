#include "ActionInitialization.h"
#include "GeneratorAction.h"

ActionInitialization::ActionInitialization() : G4VUserActionInitialization()
{
}

ActionInitialization::~ActionInitialization()
{
}

// The only thing we actually need this to do - set up the generator
void ActionInitialization::Build() const
{
  this->SetUserAction( new GeneratorAction() );
}
