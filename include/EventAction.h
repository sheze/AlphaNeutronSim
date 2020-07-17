#include "G4UserEventAction.hh"

#include <vector>
#include <fstream>

struct SubEvent
{
    std::string particle;
    double kinetic;
    double vecX;
    double vecY;
    double vecZ;
    int trackID;
    int parentID;
};

class EventAction : public G4UserEventAction
{
    public:
        EventAction();
        ~EventAction();

        virtual void BeginOfEventAction(const G4Event*);
        virtual void EndOfEventAction(const G4Event*);
        void AcceptSubEvent(SubEvent);
        
    private:
        std::vector<SubEvent> fSubEvent;
        std::string fFileName;
        std::ofstream fOutput;
};