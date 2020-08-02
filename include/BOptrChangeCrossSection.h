#pragma once

#include "G4VBiasingOperator.hh"
#include <map>

class G4Track;
class G4BiasingProcessInterface;
class G4BiaisngAppliedCase;
class G4VBiasingOperation;
class G4VOarticleChange;
class G4BOptnChangeCrossSection;
class G4ParticleDefinition;

class BOptrChangeCrossSection : public G4VBiasingOperator
{
    public:
        // Takes the name of particle type to bias
        BOptrChangeCrossSection(std::string particleToBias, std::string name = "ChangeXS");
        virtual ~BOptrChangeCrossSection();
        
        // Method called at begining of run
        virtual void StartRun();

        // Print the multiple times
        double GetXSMultiple() {return fXSMultiple;}

    private:
        // This method returns the biasing operation that will bias the physics process occurence
        virtual G4VBiasingOperation* 
        ProposeOccurenceBiasingOperation(const G4Track*                   track, 
                                         const G4BiasingProcessInterface* callingProcess);
        // Method not used, but need to be  override, otherwise it is an abstract class
        virtual G4VBiasingOperation* ProposeFinalStateBiasingOperation(const G4Track*, 
                                                                       const G4BiasingProcessInterface*)
        {
            return 0; 
        }
        virtual G4VBiasingOperation* ProposeNonPhysicsBiasingOperation(const G4Track*,
                                                                       const G4BiasingProcessInterface*)
        {
            return 0;
        }
        
        using G4VBiasingOperator::OperationApplied;
        // This method is called to inform the operator that a proposed operation has been applied
        // In the present case, it means that a physical interaction occured
        // interaction at PostStepDoIt level):
        virtual void OperationApplied(const G4BiasingProcessInterface* callingProcess,
                                      G4BiasingAppliedCase biasingCase,
                                      G4VBiasingOperation* occurenceOperationApplied,
                                      G4double weightForOccurenceInteraction,
                                      G4VBiasingOperation* finalStateOperationApplied,                                 
                                      const G4VParticleChange* particleChangeProduced
                                     );
        std::map<const G4BiasingProcessInterface*, G4BOptnChangeCrossSection*> fChangeCrossSectionOperations;
        double fXSMultiple;
        bool fSetup;
        const G4ParticleDefinition* fParticleToBias; 
};