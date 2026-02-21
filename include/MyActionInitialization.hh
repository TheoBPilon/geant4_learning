#pragma once
#include "G4VUserActionInitialization.hh"

class MyActionInitialization : public G4VUserActionInitialization {
public:
  MyActionInitialization();
  virtual ~MyActionInitialization();
  virtual void Build() const override;
  virtual void BuildForMaster() const override;
};
