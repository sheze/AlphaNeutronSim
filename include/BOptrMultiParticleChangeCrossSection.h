#pragma once

#include "G4VBiasingOperator.hh"
#include <map>

class BOptrChangeCrossSection;
class G4ParticleDefinition;

class BOptrMultiParticleChangeCrossSection : public G4VBiasingOperator
{
    public:
        BOptrMultiParticleChangeCrossSection();
        virtual ~BOptrMultiParticleChangeCrossSection();

        void AddParticle(std::string particleName);

        // It is called at the time a tracking of a particle starts
        void StartTracking(const G4Track* aTrack);
    
    private:
        virtual G4VBiasingOperation* 
                ProposeOccurenceBiasingOperation(const G4Track* aTrack, 
                                                 const G4BiasingProcessInterface* callingProcess);
        // Methods not used but must be implemented
        virtual G4VBiasingOperation*
        ProposeFinalStateBiasingOperation(const G4Track*, const G4BiasingProcessInterface*)
        {
            return 0;
        }
        virtual G4VBiasingOperation*
        ProposeNonPhysicsBiasingOperation(const G4Track*, const G4BiasingProcessInterface*)
        {
            return 0;
        }

        using G4VBiasingOperator::OperationApplied;
        // This method is called to inform the operator that a proposed operation has been applied
        // In the present case, it means that a physical interaction occured
        // (Interaction at PostStepDoIt level) : 
        virtual void OperationApplied(const G4BiasingProcessInterface* callingProcess,
                                      G4BiasingAppliedCase biasingCase,
                                      G4VBiasingOperation* occurenceOperationApplied,
                                      G4double weightForOccurenceInteraction,
                                      G4VBiasingOperation* finalStateOperationApplied,
                                      const G4VParticleChange* particleChangeProduced);
        std::map<const G4ParticleDefinition*, BOptrChangeCrossSection*> fBOptrForParticle;
        std::vector<const G4ParticleDefinition*> fParticlesToBias;
        BOptrChangeCrossSection* fCurrentOperator;

        // Count numer of biased interactions for current track
        int fnInteractions;
};