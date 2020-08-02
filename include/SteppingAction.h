#include "G4UserSteppingAction.hh"

class EventAction;

class SteppingAction : public G4UserSteppingAction
{
    public:
        SteppingAction(EventAction* anEventAction);
        ~SteppingAction();

        virtual void UserSteppingAction(const G4Step*);

    private:
        EventAction* fEventAction;
        double fWeight;
        double fKinetic;
        double fVecX;
        double fVecY;
        double fVecZ;
        int fTrackID;
        int fParentID;
};