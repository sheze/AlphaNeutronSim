#include <iostream>

#include "BOptrMultiParticleChangeCrossSection.h"
#include "BOptrChangeCrossSection.h"

#include "G4BiasingProcessInterface.hh"
#include "G4ParticleDefinition.hh"
#include "G4ParticleTable.hh"
#include "G4SystemOfUnits.hh"

BOptrMultiParticleChangeCrossSection::BOptrMultiParticleChangeCrossSection()
            : G4VBiasingOperator("TestManyExponentialTransform")
{
    std::cout << "INFO : Multi-particle Biasing process is created ! " << std::endl;
}

BOptrMultiParticleChangeCrossSection::~BOptrMultiParticleChangeCrossSection()
{
    std::cout << "INFO : Multi-particle Biasing process is deconstructed ! " << std::endl;
}

void BOptrMultiParticleChangeCrossSection::AddParticle(std::string particleName)
{
    std::cout << "Add particle " << particleName << std::endl;
    const G4ParticleDefinition* particle = 
          G4ParticleTable::GetParticleTable()->FindParticle(particleName);
    
    if(particle == nullptr)
    {
        G4ExceptionDescription ed;
        ed << "Particle `" << particleName << "' not found !" << G4endl;
        G4Exception("BOptrMultiParticleChangeCrossSection::AddParticle(...)",
                    "AlphaSim",
                    JustWarning,
                    ed);
      return;
    }
    BOptrChangeCrossSection* optr = new BOptrChangeCrossSection(particleName);
    fParticlesToBias.push_back(particle);
    fBOptrForParticle[particle] = optr;
}

G4VBiasingOperation* 
BOptrMultiParticleChangeCrossSection::ProposeOccurenceBiasingOperation(const G4Track* aTrack, 
                                                                       const G4BiasingProcessInterface* callingProcess)
{
    // Examples of limitations imposed to apply the biasing:
    // 1. Limit application of biasing to primary particles only : 
    if(aTrack->GetParentID() != 0) 
    {
        // std::cout << "Not parent track and quit !" << std::endl;
        return 0;
    }

    // 2. Limit to alpha inelastic interaction
    if(!callingProcess->GetProcessName().contains("alphaInelastic"))         
    {
        // std::cout << "Not alphaInelastic !" << std::endl;
        return 0;
    }
    // 3. Limit to at most 5 biased interactions
    if(fnInteractions > 1)         
    {
        // std::cout << "5 interactions biased track and quit !" << std::endl;
        return 0;
    }

    // 4. Limit to a weight of at least 0.01
    if(aTrack->GetWeight() < 0.01) 
    {
        // std::cout << "Too small weight !" << std::endl;
        return 0;
    }
    
    if(fCurrentOperator) return fCurrentOperator->GetProposedOccurenceBiasingOperation(aTrack, callingProcess);
    else return 0;
}

void BOptrMultiParticleChangeCrossSection::StartTracking(const G4Track* aTrack)
{
    // Fetch the underneath biasing operator, if any, for the current particle type : 
    const G4ParticleDefinition* particle = aTrack->GetParticleDefinition();
    std::map<const G4ParticleDefinition*, BOptrChangeCrossSection*>::iterator it;
    it = fBOptrForParticle.find(particle);
    fCurrentOperator = nullptr;
    if(it != fBOptrForParticle.end()) fCurrentOperator = (*it).second;

    // Reset count for number of biased interactions : 
    fnInteractions = 0;
}

void BOptrMultiParticleChangeCrossSection::OperationApplied(const G4BiasingProcessInterface* callingProcess,
                                                            G4BiasingAppliedCase biasingCase,
                                                            G4VBiasingOperation* occurenceOperationApplied,
                                                            G4double weightForOccurenceInteraction,
                                                            G4VBiasingOperation* finalStateOperationApplied,
                                                            const G4VParticleChange* particleChangeProduced)
{
    // Count number of biased interactions : 
    fnInteractions++;

    // Inform the underneath biasing operator that a biased interaction occured
    if(fCurrentOperator) fCurrentOperator->ReportOperationApplied(callingProcess,
                                                                  biasingCase,
                                                                  occurenceOperationApplied,
                                                                  weightForOccurenceInteraction,
                                                                  finalStateOperationApplied,
                                                                  particleChangeProduced);
}