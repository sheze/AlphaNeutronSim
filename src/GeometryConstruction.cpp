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
    auto concrete     = aNistManager->FindOrBuildMaterial("G4_CONCRETE");
    auto vacuum       = aNistManager->FindOrBuildMaterial("G4_Galactic");
    // Since the (alpha, n) interaction of Si28 has threshold over 8 MeV
    // auto silicon28    = new G4Isotope("silicon28", z = 14, n = 28, a = 27.98 * g/mole);
    // auto silicon29    = new G4Isotope("silicon29", z = 14, n = 29, a = 28.98 * g/mole);
    // auto silicon30    = new G4Isotope("silicon30", z = 14, n = 30, a = 29.97 * g/mole);
    // auto elSilicon    = new G4Element("elSilicon", "enSi", 3);
    // elSilicon->AddIsotope(silicon28, 99.85 * perCent);
    // elSilicon->AddIsotope(silicon29, 0.11 * perCent);
    // elSilicon->AddIsotope(silicon30, 0.04 * perCent);
    // auto oxygen       = aNistManager->FindOrBuildElement("O");
    // auto enSiO2      = new G4Material("SiO2", density = 2.2 * g/cm3, nComponent = 2);
    // enSiO2->AddElement(elSilicon, nAtom = 1);
    // enSiO2->AddElement(oxygen, nAtom = 2);
    // auto Sand = new G4Material("Sand", density = 1.6 * g/cm3, nComponent = 7);
    // auto CaCO3 = aNistManager->FindOrBuildMaterial("G4_CALCIUM_CARBONATE");
    // auto SiO2  = aNistManager->FindOrBuildMaterial("G4_SILICON_DIOXIDE");
    // auto Na2CO3 = aNistManager->FindOrBuildMaterial("G4_SODIUM_CARBONATE");
    // auto Al2O3 = aNistManager->FindOrBuildMaterial("G4_ALUMINUM_OXIDE");
    // auto elK = aNistManager->FindOrBuildElement("K");
    // auto elC = aNistManager->FindOrBuildElement("C");
    // auto elO = aNistManager->FindOrBuildElement("O");
    // auto K2CO3 = new G4Material("K2CO3", density = 2.43 * g/cm3, nComponent = 3);
    // K2CO3->AddElement(elK, nAtom = 2);
    // K2CO3->AddElement(elC, nAtom = 1);
    // K2CO3->AddElement(elO, nAtom = 3);
    // auto Fe = aNistManager->FindOrBuildMaterial("G4_Fe");
    // auto Mg = aNistManager->FindOrBuildMaterial("G4_Mg");
    // Sand->AddMaterial(SiO2, 80.3 * perCent);
    // Sand->AddMaterial(Na2CO3, 0.7 * perCent);
    // Sand->AddMaterial(CaCO3, 10.3 * perCent);
    // Sand->AddMaterial(K2CO3, 2.1 * perCent);
    // Sand->AddMaterial(Al2O3, 4.9 * perCent);
    // Sand->AddMaterial(Fe, 1.0 * perCent);
    // Sand->AddMaterial(Mg, 0.7 * perCent);

    // auto limestone = new G4Material("Limestone", density = 2.711 * g/cm3, nComponent = 6);
    // limestone->AddMaterial(SiO2, 5.41 * perCent);
    // limestone->AddMaterial(K2CO3, 0.42 * perCent);
    // limestone->AddMaterial(Al2O3, 0.83 * perCent);
    // limestone->AddMaterial(CaCO3, 87.41 * perCent);
    // limestone->AddMaterial(Mg, 5.52 * perCent);
    // limestone->AddMaterial(Fe, 0.41 * perCent);

    // auto Na = aNistManager->FindOrBuildMaterial("G4_Na");
    // auto Al = aNistManager->FindOrBuildMaterial("G4_Al");

    // auto lN2 = new G4Material("liquidN2", density = 0.807 * g/cm3, nComponent = 1);
    // auto elN = new G4Element("elNitrogen", "elN", 2);
    // auto N14 = new G4Isotope("N14", z = 7, n = 14, a = 14. * g/mole);
    // auto N15 = new G4Isotope("N15", z = 7, n = 15, a = 15. * g/mole);
    // elN->AddIsotope(N14, 99.636 * perCent);
    // elN->AddIsotope(N15, 0.364 * perCent);
    // lN2->AddElement(elN, 1.);

    // World definition
    double halfWorldXYZ = 0.2 * m;
    
    auto solidWorld = new G4Box("solidWorld", halfWorldXYZ, halfWorldXYZ, halfWorldXYZ);
    auto logicWorld = new G4LogicalVolume(solidWorld, vacuum, "logicWorld");
    G4VPhysicalVolume* physWorld = new G4PVPlacement(nullptr, G4ThreeVector(), logicWorld, 
                                                     "physWorld", 0, 0, 0, checkOverlaps);
    
    // Volume distributed source geometry
    double concreteRadius = 0.1  * m;
    double srcVolRadius   = 0.09 * m;
    double startAng       = 0.   * deg;
    double endAng         = 360. * deg;
    double startTheta     = 0.   * deg;
    double endTheta       = 360. * deg;

    // The outer component of concrete wall
    auto solidConcrete = new G4Sphere("solidConcrete", 0, concreteRadius, startAng, endAng, startTheta, endTheta);
    auto logicConcrete = new G4LogicalVolume(solidConcrete, concrete, "logicConcrete");
    new G4PVPlacement(nullptr, G4ThreeVector(), logicConcrete, "physConcrete", logicWorld, 0, 0, checkOverlaps);
    
    // The source volume for infinite radius assumption
    auto solidSource = new G4Sphere("solidSource", 0, srcVolRadius, startAng, endAng, startTheta, endTheta);
    auto logicSource = new G4LogicalVolume(solidSource, concrete, "logicSource");
    std::cout << "INFO : The target material simulated is " << logicSource->GetMaterial()->GetName() << std::endl;
    new G4PVPlacement(nullptr, G4ThreeVector(), logicSource, "physSource", logicConcrete, 0, 0, checkOverlaps);

    // alpha incident geometry
    // double halfXYZ = 0.1 * m;
    // auto solidBox = new G4Box("solidBox", halfXYZ, halfXYZ, halfXYZ);
    // auto logicBox = new G4LogicalVolume(solidBox, Al2O3, "logicBox");
    // new G4PVPlacement(nullptr, G4ThreeVector(), logicBox, "physBox", logicWorld, 0, 0, checkOverlaps);

    return physWorld; 
}

void GeometryConstruction::ConstructSDandField()
{
    // Fetch the biasing volume
    // G4LogicalVolume* logicBiasing = G4LogicalVolumeStore::GetInstance()->GetVolume("logicBox");
    
    G4LogicalVolume* logicBiasing1 = G4LogicalVolumeStore::GetInstance()->GetVolume("logicSource");
    G4LogicalVolume* logicBiasing2 = G4LogicalVolumeStore::GetInstance()->GetVolume("logicConcrete");

    // Create operation and attach it to the biasing volume
    BOptrMultiParticleChangeCrossSection* operation = new BOptrMultiParticleChangeCrossSection();
    operation->AddParticle("alpha");
    operation->AttachTo(logicBiasing1);
    operation->AttachTo(logicBiasing2);
    // operation->AttachTo(logicBiasing);
    auto oper = operation->GetOperation().begin()->second;
    std::cout << " Attaching biasing operator " << operation->GetName()
              << " to logical volume " << logicBiasing1->GetName()
              << " and " << logicBiasing2->GetName()
              << " with multiple times " << oper->GetXSMultiple()
              << std::endl;

}