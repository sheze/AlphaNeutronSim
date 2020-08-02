#include "BOptrChangeCrossSection.h"

#include "G4BiasingProcessInterface.hh"
#include "G4BOptnChangeCrossSection.hh"
#include "G4ParticleDefinition.hh"
#include "G4ParticleTable.hh"
#include "G4VProcess.hh"
#include "G4InteractionLawPhysical.hh"

#include "Randomize.hh"

BOptrChangeCrossSection::BOptrChangeCrossSection(std::string particleName, 
                                                 std::string name)
                        : G4VBiasingOperator(name), fXSMultiple(1.0), fSetup(true)
{
    fParticleToBias = G4ParticleTable::GetParticleTable()->FindParticle(particleName);
    if(fParticleToBias == nullptr)
    {
        G4ExceptionDescription ed;
        ed << "Particle '" << particleName << "' not found !" << std::endl;
        G4Exception("BOptrChangeCrossSection(...)",
                    "AlphaN simulation",
                    JustWarning,
                    ed); 
    }
    fXSMultiple = 1000.0;
    
    std::cout << "Construct operator change cross section" << std::endl;
}

BOptrChangeCrossSection::~BOptrChangeCrossSection()
{
    std::map<const G4BiasingProcessInterface*, G4BOptnChangeCrossSection* >::iterator it;
    for(it = fChangeCrossSectionOperations.begin(); it != fChangeCrossSectionOperations.end(); it++)
    {
        delete (*it).second;
    }
    std::cout << "Deconstruct operator change cross section" << std::endl;

}

void BOptrChangeCrossSection::StartRun()
{
    // Setup stage
    // Start by collecting processes under biasing, create needed biasing
    // operations and associate these operations to the processes
    if(fSetup)
    {
        const G4ProcessManager* processManager = fParticleToBias->GetProcessManager();
        const G4BiasingProcessSharedData* sharedData = G4BiasingProcessInterface::GetSharedData(processManager);
        // Shared data tested,  as it can happen a user attaches an operator to a volume
        // without defined BiasingProcessInterface processes
        if(sharedData) 
        {
            for(unsigned int i = 0; i < (sharedData->GetPhysicsBiasingProcessInterfaces().size()); i++)
            {
                const G4BiasingProcessInterface* wrapperProcess = (sharedData->GetPhysicsBiasingProcessInterfaces())[i];
                std::string operationName = "XSchange-" + wrapperProcess->GetWrappedProcess()->GetProcessName();
                fChangeCrossSectionOperations[wrapperProcess] = new G4BOptnChangeCrossSection(operationName);
            }
        }
        fSetup = false;
    }
}

G4VBiasingOperation* BOptrChangeCrossSection::ProposeOccurenceBiasingOperation(const G4Track* aTrack,
                                                                               const G4BiasingProcessInterface* callingProcess)
{
    // Check if current particle type is the one to bias
    // Check if current interaction type is the one to bias
    if(aTrack->GetDefinition() != fParticleToBias) return 0;
    // if (!callingProcess->GetProcessName().contains("alphaInelastic")) return 0;
    // if (!callingProcess->GetProcessName().contains("phot")) return 0;

    // Select and setup the biasing operation for current callingProcess
    // Check if the analog cross-section well defined :
    // e.g. the conversion process for a gamma below e+e- creation threshold has an DBL_MAX
    // interaction length. Nothing is done in this case (i.e., let analog process to deal with the case)
    double analogInteractionLength = callingProcess->GetWrappedProcess()->GetCurrentInteractionLength();
    if(analogInteractionLength > DBL_MAX/10.) return 0;

    // Analog corss-section is well-defined :
    double analogXS = 1./analogInteractionLength;

    // Choose a constant cross-section bias
    // But at this level, this factor can be made direction dependent, 
    // like in the exponential transform MCNP case, 
    // or it can be chosen differently, depending on the process, etc 
    double XSTransformation = fXSMultiple;

    // Fetch the operation associated to this callingProcess :
    G4BOptnChangeCrossSection* operation = fChangeCrossSectionOperations[callingProcess];
    // Get the operation that was proposed to the process in the previous step : 
    G4VBiasingOperation* previousOperation = callingProcess->GetPreviousOccurenceBiasingOperation();

    // Now, setup the operation to be returned to the process :
    // this consists in setting the biased cross-section, and in asking the operation to sample its exponential interaction law
    // To do this, to first order, the two lines : 
    //         1. operation->SetBiasedCrossSection(XSTransformation* analogXS);
    //         2. operation->Sample();
    // are correct and sufficient
    // But, to avoid having to shoot a random number each time, we sample only the first time the operation is proposed,
    // or if the interaction occured.
    // If the interaction did not occur for the process in the previous,
    // we update the number of interaction length instead of resampling
    if(previousOperation == nullptr)
    {
        operation->SetBiasedCrossSection(XSTransformation * analogXS);
        operation->Sample();
    }
    else
    {
        if(previousOperation != operation)
        {
            // Should not happen!
            G4ExceptionDescription ed;
            ed << " Logic problem in operation handling !" << G4endl;
            G4Exception("GB01BOptrChangeCrossSection::ProposeOccurenceBiasingOperation(...)",
                        "exGB01.02",
                        JustWarning,
                        ed);
          return 0;
        }
        if(operation->GetInteractionOccured())
        {
            operation->SetBiasedCrossSection( XSTransformation * analogXS);
            operation->Sample();
        }
        else
        {
            // Update the 'interaction length' and underneath 'number of interaction lengths'
            // for past step (this take into account the previous step cross-section value)
            operation->UpdateForStep(callingProcess->GetPreviousStepSize());
            // Update the cross-section value : 
            operation->SetBiasedCrossSection(XSTransformation * analogXS);
            // Forces recomputation of the 'interaction length' taking into account above
            // new cross-section value 
            operation->UpdateForStep(0.0);
        }          
    }
    return operation;
}

void BOptrChangeCrossSection::OperationApplied(const G4BiasingProcessInterface* callingProcess,
                                                 G4BiasingAppliedCase,
                                                 G4VBiasingOperation* occurenceOperationApplied,
                                                 G4double,
                                                 G4VBiasingOperation*,
                                                 const G4VParticleChange*)
{
    G4BOptnChangeCrossSection* operation = fChangeCrossSectionOperations[callingProcess];
    if(operation == occurenceOperationApplied) operation->SetInteractionOccured();
}