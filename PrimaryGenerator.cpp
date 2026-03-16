#include "include/PrimaryGenerator.hh"
#include "G4Event.hh"
#include "G4IonTable.hh"
#include "G4ParticleTable.hh"
#include <G4ThreeVector.hh>
#include <G4Types.hh>
#include <Randomize.hh>
using namespace CLHEP;
using namespace std;

MyPrimaryGenerator::MyPrimaryGenerator() {
  fParticleGun = new G4ParticleGun(1);
}

MyPrimaryGenerator::~MyPrimaryGenerator() { delete fParticleGun; }

void MyPrimaryGenerator::GeneratePrimaries(G4Event *anEvent) {
  auto na22 = G4IonTable::GetIonTable()->GetIon(11, 22, 0);
  fParticleGun->SetParticleDefinition(na22);
  // associa direção do momento a particula
  fParticleGun->SetParticlePosition(G4ThreeVector(0, 0, 7.1 * cm));
  fParticleGun->SetParticleTime(0 * ns);
  fParticleGun->SetParticleEnergy(0 * MeV);
  // gera o vertex
  fParticleGun->GeneratePrimaryVertex(anEvent);
}
