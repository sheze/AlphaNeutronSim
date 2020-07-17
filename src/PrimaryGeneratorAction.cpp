#include "PrimaryGeneratorAction.h"

#include "G4GeneralParticleSource.hh"

PrimaryGeneratorAction::PrimaryGeneratorAction()
{
    fParticleGun = new G4GeneralParticleSource();
    std::cout << "INFO : Primary generator action is created ! " << std::endl;
}

PrimaryGeneratorAction::~PrimaryGeneratorAction()
{
    std::cout << "INFO : Primary generator action is deconstructed ! " << std::endl;
    delete fParticleGun;
}

void PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
    fParticleGun->GeneratePrimaryVertex(anEvent);
}