#include "ActionInitialization.h"
#include "PrimaryGeneratorAction.h"
#include "RunAction.h"
#include "EventAction.h"
#include "SteppingAction.h"

ActionInitialization::ActionInitialization() : G4VUserActionInitialization()
{}

ActionInitialization::~ActionInitialization()
{}

void ActionInitialization::Build() const
{
    PrimaryGeneratorAction* generator = new PrimaryGeneratorAction();
    
    RunAction*      aRunAction      = new RunAction();
    EventAction*    anEventAction   = new EventAction();
    SteppingAction* aSteppingAction = new SteppingAction(anEventAction);
    
    SetUserAction(generator);
    SetUserAction(aRunAction);
    SetUserAction(anEventAction);
    SetUserAction(aSteppingAction);
}

void ActionInitialization::BuildForMaster() const
{
   RunAction* aRunAction = new RunAction();
   SetUserAction(aRunAction);
}
