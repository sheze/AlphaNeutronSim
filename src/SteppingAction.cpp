#include "EventAction.h"
#include "SteppingAction.h"

#include "G4UserSteppingAction.hh"
#include "G4Step.hh"
#include "G4Track.hh"
#include "G4VProcess.hh"
#include "G4SystemOfUnits.hh"

#include <iostream>

SteppingAction::SteppingAction(EventAction* anEventAction) : G4UserSteppingAction(),
                fEventAction(anEventAction), fKinetic(0.), fVecX(0.), fVecY(0.), fVecZ(0.)
{
    std::cout << "INFO : Stepping Action is created !" << std::endl;
}

SteppingAction::~SteppingAction()
{
    std::cout << "INFO : Stepping Action is deconstructed !" << std::endl;
}

void SteppingAction::UserSteppingAction(const G4Step* aStep)
{
    G4Track* aTrack = aStep->GetTrack();
    std::string partName = aTrack->GetDynamicParticle()->GetParticleDefinition()->GetParticleName();

    int PDGcode = aTrack->GetDynamicParticle()->GetPDGcode();

    if (partName == "alpha")  return;
    if (PDGcode > 1000020040) return;

    if (partName != "neutron" ) 
    {
        aTrack->SetTrackStatus(fKillTrackAndSecondaries);
        return;
    }
    else
    {
        fKinetic = aStep->GetPreStepPoint()->GetKineticEnergy();
        if(fKinetic < 1*eV) return;

        fVecX = aTrack->GetMomentumDirection().getX();
        fVecY = aTrack->GetMomentumDirection().getY();
        fVecZ = aTrack->GetMomentumDirection().getZ();

        fTrackID  = aTrack->GetTrackID();
        fParentID = aTrack->GetParentID();
        fWeight   = aTrack->GetWeight();

        SubEvent anSubEvent;
        anSubEvent.weight   = fWeight;
        anSubEvent.kinetic  = fKinetic;
        anSubEvent.vecX     = fVecX;
        anSubEvent.vecY     = fVecY;
        anSubEvent.vecZ     = fVecZ;
        anSubEvent.trackID  = fTrackID;
        anSubEvent.parentID = fParentID;

        fEventAction->AcceptSubEvent(anSubEvent);
        aTrack->SetTrackStatus(fKillTrackAndSecondaries);
    }
    
    
    return;
}