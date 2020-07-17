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

    if (partName != "neutron" && partName != "alpha") 
    {
        aTrack->SetTrackStatus(fKillTrackAndSecondaries);
        return;
    }
    
    std::string prePhysVolName, postPhysVolName;
    if(aStep->GetPostStepPoint()->GetPhysicalVolume())
    {
         postPhysVolName = aStep->GetPostStepPoint()->GetPhysicalVolume()->GetName();
    }
    else return;

    if(aStep->GetPreStepPoint()->GetPhysicalVolume())
    {
        prePhysVolName = aStep->GetPreStepPoint()->GetPhysicalVolume()->GetName();
    }
    
    if(prePhysVolName  != "physWorld") return;
    if(postPhysVolName != "physMeasVol") return; 

    fKinetic = aTrack->GetKineticEnergy();
    if(fKinetic < 1*eV) return;

    fVecX = aTrack->GetMomentumDirection().getX();
    fVecY = aTrack->GetMomentumDirection().getY();
    fVecZ = aTrack->GetMomentumDirection().getZ();

    fTrackID     = aTrack->GetTrackID();
    fParentID    = aTrack->GetParentID();

    SubEvent anSubEvent;
    anSubEvent.particle    = partName;
    anSubEvent.kinetic     = fKinetic;
    anSubEvent.vecX        = fVecX;
    anSubEvent.vecY        = fVecY;
    anSubEvent.vecZ        = fVecZ;
    anSubEvent.trackID     = fTrackID;
    anSubEvent.parentID    = fParentID;

    fEventAction->AcceptSubEvent(anSubEvent);

    return;
}