#include "GeometryConstruction.h"
#include "BOptrMultiParticleChangeCrossSection.h"
#include "BOptrChangeCrossSection.h"

#include "G4VUserDetectorConstruction.hh"
#include "G4VPhysicalVolume.hh"
#include "G4NistManager.hh"
#include "G4Isotope.hh"
#include "G4Element.hh"
#include "G4Material.hh"
#include "G4SystemOfUnits.hh"
#include "G4Box.hh"
#include "G4Sphere.hh"
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
    
    int z, n, nComponent, nAtom;
    double a, density;
    auto aNistManager = G4NistManager::Instance();
    // auto concrete     = aNistManager->FindOrBuildMaterial("G4_CONCRETE");
    auto vacuum       = aNistManager->FindOrBuildMaterial("G4_Galactic");
    // auto beryllium    = aNistManager->FindOrBuildMaterial("G4_Be");
    // Since the (alpha, n) interaction of Si28 has threshold over 8 MeV
    auto silicon28    = new G4Isotope("silicon28", z = 14, n = 28, a = 27.98 * g/mole);
    auto silicon29    = new G4Isotope("silicon29", z = 14, n = 29, a = 28.98 * g/mole);
    auto silicon30    = new G4Isotope("silicon30", z = 14, n = 30, a = 29.97 * g/mole);
    auto elSilicon    = new G4Element("elSilicon", "enSi", 3);
    elSilicon->AddIsotope(silicon28, 99.85 * perCent);
    elSilicon->AddIsotope(silicon29, 0.11 * perCent);
    elSilicon->AddIsotope(silicon30, 0.04 * perCent);
    auto oxygen       = aNistManager->FindOrBuildElement("O");
    auto SiO2      = new G4Material("SiO2", density = 2.2 * g/cm3, nComponent = 2);
    SiO2->AddElement(elSilicon, nAtom = 1);
    SiO2->AddElement(oxygen, nAtom = 2);

    // auto boron        = aNistManager->FindOrBuildMaterial("G4_B");
    // World definition
    double halfWorldXYZ = 0.2 * m;
    
    auto solidWorld = new G4Box("solidWorld", halfWorldXYZ, halfWorldXYZ, halfWorldXYZ);
    auto logicWorld = new G4LogicalVolume(solidWorld, vacuum, "logicWorld");
    G4VPhysicalVolume* physWorld = new G4PVPlacement(nullptr, G4ThreeVector(), logicWorld, 
                                                     "physWorld", 0, 0, 0, checkOverlaps);
    
    // Volume distributed source geometry
    // double concreteRadius = 0.1  * m;
    // double srcVolRadius   = 0.09 * m;
    // double startAng       = 0.   * deg;
    // double endAng         = 360. * deg;
    // double startTheta     = 0.   * deg;
    // double endTheta       = 360. * deg;

    // // The outer component of concrete wall
    // auto solidConcrete = new G4Sphere("solidConcrete", 0, concreteRadius, startAng, endAng, startTheta, endTheta);
    // auto logicConcrete = new G4LogicalVolume(solidConcrete, concrete, "logicConcrete");
    // new G4PVPlacement(nullptr, G4ThreeVector(), logicConcrete, "physConcrete", logicWorld, 0, 0, checkOverlaps);
    
    // // The source volume for infinite radius assumption
    // auto solidSource = new G4Sphere("solidSource", 0, srcVolRadius, startAng, endAng, startTheta, endTheta);
    // auto logicSource = new G4LogicalVolume(solidSource, concrete, "logicSource");
    // new G4PVPlacement(nullptr, G4ThreeVector(), logicSource, "physSource", logicConcrete, 0, 0, checkOverlaps);

    // alpha incident geometry
    double halfXYZ = 0.1 * m;
    auto solidBox = new G4Box("solidBox", halfXYZ, halfXYZ, halfXYZ);
    auto logicBox = new G4LogicalVolume(solidBox, SiO2, "logicBox");
    new G4PVPlacement(nullptr, G4ThreeVector(), logicBox, "physBox", logicWorld, 0, 0, checkOverlaps);

    return physWorld; 
}

void GeometryConstruction::ConstructSDandField()
{
    // Fetch the biasing volume
    G4LogicalVolume* logicBiasing = G4LogicalVolumeStore::GetInstance()->GetVolume("logicBox");
    
    // Create operation and attach it to the biasing volume
    BOptrMultiParticleChangeCrossSection* operation = new BOptrMultiParticleChangeCrossSection();
    operation->AddParticle("alpha");
    // operation->AddParticle("gamma");
    operation->AttachTo(logicBiasing);
    auto oper = operation->GetOperation().begin()->second;
    std::cout << " Attaching biasing operator " << operation->GetName()
              << " to logical volume " << logicBiasing->GetName()
              << " with multiple times " << oper->GetXSMultiple()
              << std::endl;

}