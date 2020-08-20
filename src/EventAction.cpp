#include "EventAction.h"

#include "G4UserEventAction.hh"
#include "G4Event.hh"

#include <iostream>
#include <fstream>

EventAction::EventAction() : G4UserEventAction(), fSubEvent({}), fFileName("")
{
    fFileName = "AlphaEnhanced.csv";
    fOutput.open(fFileName);
    fOutput << "EventID,trackID,parentID,kineticMeV,"
            << "VecX,VecY,VecZ,"
            << "Weight\n";
    std::cout << "INFO : Event Action is created !" << std::endl;
    std::cout << "Open file with file-name : " << std::endl;
    std::cout << fFileName << std::endl;
}

EventAction::~EventAction()
{
    fOutput.close();
    std::cout << "INFO : Event Action is deconstructed !" << std::endl;
}

void EventAction::BeginOfEventAction(const G4Event*)
{}

void EventAction::EndOfEventAction(const G4Event* anEvent)
{
    int eventID = anEvent->GetEventID();
    for(unsigned int i = 0; i < fSubEvent.size(); i++)
    {
        fOutput << eventID << "," 
                << fSubEvent[i].trackID << "," 
                << fSubEvent[i].parentID << ","
                << fSubEvent[i].kinetic << ","
                << fSubEvent[i].vecX << "," 
                << fSubEvent[i].vecY << ","
                << fSubEvent[i].vecZ << "," 
                << fSubEvent[i].weight
                << std::endl;
    }
    // Re-initialization at end of each event
    fSubEvent = {};  
}

void EventAction::AcceptSubEvent(SubEvent aSubEvent)
{
    fSubEvent.push_back(aSubEvent);
}