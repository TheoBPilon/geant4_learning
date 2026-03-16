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
#include <G4ThreeVector.hh>
#include <G4Tubs.hh>
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
  G4double water_rindex[2] = {1.23, 1.23};
  G4double ssteel_reflectivity[2] = {0.6, 0.6};

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
  // adicionar reflectance
  ssteelMPT->AddProperty("REFLECTIVITY", photon_energies, ssteel_reflectivity,
                         2);
  ssteel->SetMaterialPropertiesTable(ssteelMPT);

  // ------------------------ Argonio líquido ------------------------
  G4Material *lAr = G4NistManager::Instance()->FindOrBuildMaterial("G4_lAr");
  auto lArMPT = new G4MaterialPropertiesTable();
  lArMPT->AddProperty("RINDEX", photon_energies, water_rindex, 2);
  // define lAr Scintillation spectra gaussiana centrada em
  G4int num_entries = 1000;
  G4double meanE = 9.68 * eV;
  G4double sigmaE = 0.5 * eV;

  G4double *energies = new G4double[num_entries];
  G4double *spectra = new G4double[num_entries];

  G4double E_min = meanE - 4. * sigmaE;
  G4double E_max = meanE + 4. * sigmaE;

  for (int i = 0; i < num_entries; i++) {
    energies[i] = E_min + i * (E_max - E_min) / (num_entries - 1);
    G4double x = energies[i];
    // gaussiana centrada em 9.68 eV
    spectra[i] = pow(sigmaE * pow(2 * CLHEP::pi, 0.5), -1) *
                 exp(-0.5 * pow((x - meanE) / sigmaE, 2));
  }

  lArMPT->AddProperty("SCINTILLATIONCOMPONENT1", energies, spectra, num_entries,
                      true);
  lArMPT->AddProperty("SCINTILLATIONCOMPONENT2", energies, spectra, num_entries,
                      true);
  lArMPT->AddConstProperty("RESOLUTIONSCALE", 1.0, true);
  lArMPT->AddConstProperty("SCINTILLATIONTIMECONSTANT1", 6. * ns, true);
  lArMPT->AddConstProperty("SCINTILLATIONTIMECONSTANT2", 1.3 * us, true);
  lArMPT->AddConstProperty("SCINTILLATIONYIELD1", 0.75);
  lArMPT->AddConstProperty("SCINTILLATIONYIELD2", 0.25);
  lArMPT->AddConstProperty("SCINTILLATIONYIELD", 4 / MeV, true);
  lAr->SetMaterialPropertiesTable(lArMPT);

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

  // ------- cruando superficie optica entre lAr e ssteel ---------
  auto lAr_ssteel_surface = new G4OpticalSurface("water_ssteel_surface");
  lAr_ssteel_surface->SetModel(unified);
  lAr_ssteel_surface->SetType(dielectric_metal);
  lAr_ssteel_surface->SetFinish(polished);

  // definindo o solido do mundo
  G4Box *solidWorld = new G4Box("World", 35 * cm, 35 * cm, 34 * cm);
  // definindo a lógica do mundo
  G4LogicalVolume *logicWorld = new G4LogicalVolume(solidWorld, air, "World");
  // definindo o volume do mundo
  G4VPhysicalVolume *physWorld =
      new G4PVPlacement(0, G4ThreeVector(), logicWorld, "World", 0, false, 0);
  auto defaultVis = new G4VisAttributes(G4Colour(1, 1, 1));

  // Modelando um volume para trigger coincidental
  G4Tubs *solid_trigger_volume =
      new G4Tubs("trigger_volume", 0, 1 * cm, 1 * cm, 0, 360 * deg);
  G4LogicalVolume *logic_trigger =
      new G4LogicalVolume(solid_trigger_volume, ssteel, "logic_trigger");
  // SEMPRE ADICIONAR 1,1CM EM RELACAO AO NA22
  auto phys_trigger_volume =
      new G4PVPlacement(0, G4ThreeVector(0, 0, 8.2 * cm), logic_trigger,
                        "trigger_volume", logicWorld, false, 0, defaultVis);

  // criando o solido da caixa
  G4Box *solidBox = new G4Box("Box", 7 * cm, 7 * cm, 7 * cm);
  // criando a lógica da caixa
  G4LogicalVolume *logicBox = new G4LogicalVolume(solidBox, ssteel, "Box");
  // criando o volume da caixa
  auto physbox = new G4PVPlacement(0, G4ThreeVector(0, 0, 0 * m), logicBox,
                                   "Box", logicWorld, false, 0, defaultVis);

  // criando caixa de argonio dentro
  G4Box *solidBox2 = new G4Box("Box2", 5 * cm, 5 * cm, 5 * cm);
  // criando a lógica da caixa
  G4LogicalVolume *logicBox2 = new G4LogicalVolume(solidBox2, lAr, "Box2");
  // criando o volume da caixa
  auto physbox2 = new G4PVPlacement(0, G4ThreeVector(0, 0, 0 * m), logicBox2,
                                    "Box2", logicBox, false, 0, defaultVis);

  // --------------logbordersurface air-ssteel -----------------------
  auto ssteel_air_logical_border_surface = new G4LogicalBorderSurface(
      "air_ssteel_surface", physbox, physbox, air_ssteel_surface);
  auto air_ssteel_logical_border_surface = new G4LogicalBorderSurface(
      "ssteel_air_surface", physbox, physWorld, air_ssteel_surface);

  auto lAr_ssteel_logical_border_surface = new G4LogicalBorderSurface(
      "lAr_ssteel_surface", physbox2, physbox, lAr_ssteel_surface);
  auto ssteel_lAr_logical_border_surface = new G4LogicalBorderSurface(
      "ssteel_lAr_surface", physWorld, physbox2, lAr_ssteel_surface);

  auto ssteel_vis = new G4VisAttributes(G4Colour(0, 0, 0, 0.2));
  ssteel_vis->SetForceSolid(true);
  logicBox->SetVisAttributes(ssteel_vis);
  logicWorld->SetVisAttributes(defaultVis);

  auto lAr_vis = new G4VisAttributes(G4Colour(0, 0, 1, 0.2));
  logicBox2->SetVisAttributes(lAr_vis);
  return physWorld;
}
