#pragma once
#include "G4OpticalPhoton.hh"
#include "G4ParticleGun.hh"
#include "G4VUserPrimaryGeneratorAction.hh"
#include <G4Types.hh>

class MyPrimaryGenerator : public G4VUserPrimaryGeneratorAction {
public:
  MyPrimaryGenerator();
  virtual ~MyPrimaryGenerator();
  virtual void GeneratePrimaries(G4Event *anEvent);

private:
  G4ParticleGun *fParticleGun;
  G4double fE0;
  G4double fEmax;
  G4int fnPoints;
  G4int fNumberOfEventsPerEnergy;
};
