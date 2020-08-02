#include "GeometryConstruction.h"
#include "ActionInitialization.h"

#include "G4RunManager.hh"
#include "G4GenericBiasingPhysics.hh"
#include "Shielding.hh"
#include "QGSP_BERT_HP.hh"
#include "QBBC.hh"
#include "G4RadioactiveDecayPhysics.hh"
#include "G4VisManager.hh"
#include "G4VisExecutive.hh"
#include "G4UImanager.hh"
#include "G4UIExecutive.hh"
#include "G4SystemOfUnits.hh"

#include <iostream>
#include <stdlib.h>

void inputErrorMessage()
{
    std::cout << "ERROR : The input message should follow this convention : \n" << std::endl;
    std::cout << "./program macro or " << "./program -m macro -b biasingFlag" << std::endl;
    abort();
}

int main(int argc, char** argv)
{ 
    std::string macro;
    G4String biasingFlag = "";

    // Parser the input arguments
    // Instantiate G4UIExecutive if interactive mode
    G4UIExecutive* ui = nullptr;
    if (argc == 1) 
    {
        ui = new G4UIExecutive(argc, argv);
    }
    else if (argc == 2) 
    {
        std::cout << "INFO : Do not provide biasing flag, the default choice is not biasing" 
                  << std::endl;
        macro = argv[1];
    }
    else if (argc == 5)
    {
        for (int i = 1; i < argc; i += 2)
        {
            if      (std::string(argv[i]) == "-m") macro = argv[i+1];
            else if (std::string(argv[i]) == "-b") biasingFlag = argv[i+1];
            else
            {
                inputErrorMessage();
            }
        }
    }
    else
    {
        inputErrorMessage();
    }

    std::cout << biasingFlag << std::endl;
 
    G4RunManager* runManager = new G4RunManager;

    // Geometry construction and registration
    GeometryConstruction* geometry = new GeometryConstruction();
    runManager->SetUserInitialization(geometry);

    // Physical list construction and registration, togther with biasing physics
    // Shielding* physicsList = new Shielding;
    // QGSP_BERT_HP* physicsList = new QGSP_BERT_HP();
    G4VModularPhysicsList* physicsList = new QBBC();
    physicsList->RegisterPhysics(new G4RadioactiveDecayPhysics);
    G4GenericBiasingPhysics* biasingPhysics = new G4GenericBiasingPhysics();
    biasingFlag.toUpper();
    if(biasingFlag == "ON")
    {
        biasingPhysics->Bias("alpha");
        // biasingPhysics->Bias("gamma");
        physicsList->RegisterPhysics(biasingPhysics);
        std::cout <<  "INFO :processes are wrapped for biasing " << std::endl;
    }
    else
    {
      std::cout <<  "INFO :processes are not wrapped " << std::endl;
    }
    physicsList->SetDefaultCutValue(0.001 * mm);
    physicsList->SetVerboseLevel(1);
    runManager->SetUserInitialization(physicsList);

    // Action Initialization
    runManager->SetUserInitialization(new ActionInitialization);
    
    // Initialize visualization
    G4VisManager* visManager = new G4VisExecutive;
    // G4VisExecutive can take a verbosity argument - see /vis/verbose guidance.
    visManager->Initialize();

    // Get the pointer to the User Interface manager
    G4UImanager* UImanager = G4UImanager::GetUIpointer();

    if ( !ui )   // batch mode
    {
      std::string command = "/control/execute ";
      UImanager->ApplyCommand(command+macro);
    }
    else
    {  // interactive mode : define UI session
        UImanager->ApplyCommand("/control/execute vis.mac");
        ui->SessionStart();
        delete ui;
    }

    delete visManager;
    delete runManager;

    return 0;
}
