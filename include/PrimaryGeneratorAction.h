#pragma once

#include "G4VUserPrimaryGeneratorAction.hh"

class G4Event;
class G4GeneralParticleSource;

class PrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
    public: 
        PrimaryGeneratorAction();
        virtual ~PrimaryGeneratorAction();
        virtual void GeneratePrimaries(G4Event*);
    private:
        G4GeneralParticleSource* fParticleGun;
};