#include "include/PrimaryGenerator.hh"
#include "G4Event.hh"
#include "G4ParticleTable.hh"
#include <G4ThreeVector.hh>
#include <G4Types.hh>
#include <Randomize.hh>
using namespace CLHEP;
using namespace std;

MyPrimaryGenerator::MyPrimaryGenerator() {
  fParticleGun = new G4ParticleGun(1);
  fE0 = 1 * keV;
  fEmax = 100 * MeV;
  fnPoints = 1000;
  fNumberOfEventsPerEnergy = 100;
}

MyPrimaryGenerator::~MyPrimaryGenerator() { delete fParticleGun; }

void MyPrimaryGenerator::GeneratePrimaries(G4Event *anEvent) {
  auto particle = G4ParticleTable::GetParticleTable()->FindParticle("gamma");
  fParticleGun->SetParticleDefinition(particle);
  // associa direção do momento a particula
  fParticleGun->SetParticleMomentumDirection(G4ThreeVector(0, 0, -1));
  fParticleGun->SetParticlePosition(G4ThreeVector(0, 0, 8 * cm));

  // define energia da particula
  // passos log
  G4int eventID = anEvent->GetEventID();
  auto event_index = eventID / fNumberOfEventsPerEnergy;
  G4double logE0 = log(fE0);
  G4double logEmax = log(fEmax);
  G4double deltaLogE = (logEmax - logE0) / fnPoints;
  G4double energy = exp(logE0 + event_index * deltaLogE);

  fParticleGun->SetParticleEnergy(energy);
  // gera o vertex
  fParticleGun->GeneratePrimaryVertex(anEvent);
}
