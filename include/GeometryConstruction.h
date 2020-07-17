#pragma once

#include "G4VUserDetectorConstruction.hh"

class G4VPhysicalVolume;

class GeometryConstruction : public G4VUserDetectorConstruction
{
    public:
        GeometryConstruction();
        ~GeometryConstruction();
        virtual G4VPhysicalVolume* Construct();

        void ConstructSDandField();
};