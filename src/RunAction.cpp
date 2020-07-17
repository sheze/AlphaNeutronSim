#include "RunAction.h"

#include <iostream>

RunAction::RunAction() : G4UserRunAction()
{
    std::cout << "INFO : Run Action is created ! " << std::endl;
}

RunAction::~RunAction()
{
    std::cout << "INFO : Run Action is deconstructed ! " << std::endl;
}

void RunAction::BeginOfRunAction(const G4Run* )
{
    std::cout << "A run starts !" << std::endl;
}

void RunAction::EndOfRunAction(const G4Run*)
{
    std::cout << "A run is completed !" << std::endl;
}