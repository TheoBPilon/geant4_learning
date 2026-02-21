#pragma once

// classe base do Geant para definir geometrias
#include "G4VUserDetectorConstruction.hh"
#include "globals.hh"

class MyDetector : public G4VUserDetectorConstruction {
public:
  MyDetector();          // construtor padrão
  virtual ~MyDetector(); // destrutor padrão
  G4VPhysicalVolume *Construct()
      override; //  sobrescreve metodo de constuir geometria padrao do Geant
};
