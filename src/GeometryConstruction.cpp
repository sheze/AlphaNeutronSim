#include "GeometryConstruction.h"
#include "BOptrMultiParticleChangeCrossSection.h"
#include "BOptrMultiParticleChangeCrossSection.h"

#include "G4VUserDetectorConstruction.hh"
#include "G4VPhysicalVolume.hh"
#include "G4NistManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4LogicalVolumeStore.hh"


GeometryConstruction::GeometryConstruction() : G4VUserDetectorConstruction()
{
    std::cout << "INFO : Detector construction class is created !" 
              << std::endl;
}

GeometryConstruction::~GeometryConstruction()
{
    std::cout << "INFO : Detector construction class is deconstructed !" 
              << std::endl;
}

G4VPhysicalVolume* GeometryConstruction::Construct()
{
    // Geometry overlap check option
    bool checkOverlaps = true;
    
    auto aNistManager = G4NistManager::Instance();
    auto concrete     = aNistManager->FindOrBuildMaterial("G4_CONCRETE");
    auto vacuum       = aNistManager->FindOrBuildMaterial("G4_Galactic");
    
    // World definition
    double halfWorldXYZ = 0.5 * m;
    
    auto solidWorld = new G4Box("solidWorld", halfWorldXYZ, halfWorldXYZ, halfWorldXYZ);
    auto logicWorld = new G4LogicalVolume(solidWorld, vacuum, "logicWorld");
    G4VPhysicalVolume* physWorld = new G4PVPlacement(nullptr, G4ThreeVector(), logicWorld, 
                                                     "physWorld", 0, 0, 0, checkOverlaps);
    
    // Concrete definition
    double halfConcreteXY = 0.5  * m;
    double halfMeasXY     = 0.5  * m; 
    double halfConcreteZ  = 0.05 * m;
    double halfMeasZ      = 0.05 * m;  
    
    // The outer component of concrete wall
    auto solidConcrete = new G4Box("solidConcrete", halfConcreteXY, halfConcreteXY, halfConcreteZ);
    auto logicConcrete = new G4LogicalVolume(solidConcrete, concrete, "logicConcrete");
    new G4PVPlacement(nullptr, G4ThreeVector(), logicConcrete, "physConcrete", logicWorld, 0, 0, checkOverlaps);
    
    // The measurement volume
    auto solidMeasVol = new G4Box("solidMeasVol", halfMeasXY, halfMeasXY, halfMeasZ);
    auto logicMeasVol = new G4LogicalVolume(solidMeasVol, vacuum, "logicMeasVol");
    new G4PVPlacement(nullptr, G4ThreeVector(0, 0, 0.12*m), logicMeasVol, "physMeasVol", logicWorld, 0, 0, checkOverlaps);

    return physWorld; 
}

void GeometryConstruction::ConstructSDandField()
{
    // Fetch the biasing volume
    G4LogicalVolume* logicBiasing = G4LogicalVolumeStore::GetInstance()->GetVolume("logicConcrete");

    // Create operation and attach it to the biasing volume
    BOptrMultiParticleChangeCrossSection* operation = new BOptrMultiParticleChangeCrossSection();
    operation->AddParticle("alpha");
    // operation->AddParticle("gamma");
    operation->AttachTo(logicBiasing);
    std::cout << " Attaching biasing operator " << operation->GetName()
              << " to logical volume " << logicBiasing->GetName()
              << std::endl;

}