#include "include/MyDetector.hh"

// incluindo diferentes formatos de geometria
#include "G4Box.hh"
#include "G4Colour.hh"
#include "G4LogicalBorderSurface.hh"
#include "G4LogicalVolume.hh"
#include "G4Material.hh"
#include "G4MaterialPropertiesTable.hh"
#include "G4NistManager.hh"
#include "G4OpticalSurface.hh"
#include "G4PVPlacement.hh"
#include "G4VisAttributes.hh"
#include <G4SurfaceProperty.hh>
#include <G4Types.hh>
#include <G4VPhysicalVolume.hh>
using namespace CLHEP;
// Definindo o construtor e o destrutor
MyDetector::MyDetector() {}
MyDetector::~MyDetector() {}

G4VPhysicalVolume *MyDetector::Construct() {
  // definindo vetores de energia e de rindex
  G4double photon_energies[2] = {1 * eV, 10 * eV};
  G4double air_rindex[2] = {1.0, 1.0};
  G4double water_rindex[2] = {1.33, 1.33};

  // Gerenciador de materiais do Geant
  G4NistManager *nist = G4NistManager::Instance();

  // ------------------------ar ----------------------------
  G4Material *air = nist->FindOrBuildMaterial("G4_AIR");
  auto airMPT = new G4MaterialPropertiesTable();
  // propriedades do ar
  airMPT->AddProperty("RINDEX", photon_energies, air_rindex, 2);
  air->SetMaterialPropertiesTable(airMPT);

  // ------------------------agua ----------------------------
  G4Material *water = nist->FindOrBuildMaterial("G4_WATER");
  auto waterMPT = new G4MaterialPropertiesTable();
  // propriedades do agua
  waterMPT->AddProperty("RINDEX", photon_energies, water_rindex, 2);
  water->SetMaterialPropertiesTable(waterMPT);

  // ----------------------- SSTEEL --------------------------
  G4Material *ssteel = nist->FindOrBuildMaterial("G4_STAINLESS-STEEL");
  auto ssteelMPT = new G4MaterialPropertiesTable();
  ssteelMPT->AddProperty("RINDEX", photon_energies, water_rindex, 2);
  ssteel->SetMaterialPropertiesTable(ssteelMPT);

  //------- criando superficie optica entre ar e agua ---------
  auto air_water_surface = new G4OpticalSurface("air_water_surface");
  air_water_surface->SetModel(unified);
  air_water_surface->SetType(dielectric_dielectric);
  air_water_surface->SetFinish(polished);

  // ------- cruando superficie optica entre ar e ssteel ---------
  auto air_ssteel_surface = new G4OpticalSurface("air_ssteel_surface");
  air_ssteel_surface->SetModel(unified);
  air_ssteel_surface->SetType(dielectric_metal);
  air_ssteel_surface->SetFinish(polished);

  // definindo o solido do mundo
  G4Box *solidWorld = new G4Box("World", 10 * cm, 10 * cm, 10 * cm);
  // definindo a lógica do mundo
  G4LogicalVolume *logicWorld = new G4LogicalVolume(solidWorld, air, "World");
  // definindo o volume do mundo
  G4VPhysicalVolume *physWorld =
      new G4PVPlacement(0, G4ThreeVector(), logicWorld, "World", 0, false, 0);
  auto defaultVis = new G4VisAttributes(G4Colour(0, 0, 0));

  // criando o solido da caixa
  G4Box *solidBox = new G4Box("Box", 10 * cm, 10 * cm, 8 * cm);
  // criando a lógica da caixa
  G4LogicalVolume *logicBox = new G4LogicalVolume(solidBox, ssteel, "Box");
  // criando o volume da caixa
  auto physbox = new G4PVPlacement(0, G4ThreeVector(0, 0, 0 * m), logicBox,
                                   "Box", logicWorld, false, 0, defaultVis);

  // --------------logbordersurface air-ssteel -----------------------
  auto ssteel_air_logical_border_surface = new G4LogicalBorderSurface(
      "air_ssteel_surface", physWorld, physbox, air_ssteel_surface);
  auto air_ssteel_logical_border_surface = new G4LogicalBorderSurface(
      "ssteel_air_surface", physbox, physWorld, air_ssteel_surface);

  auto ssteel_vis = new G4VisAttributes(G4Colour(0, 0, 0, 0.2));
  ssteel_vis->SetForceSolid(true);
  logicBox->SetVisAttributes(ssteel_vis);
  logicWorld->SetVisAttributes(defaultVis);
  return physWorld;
}
