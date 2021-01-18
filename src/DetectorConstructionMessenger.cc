#include "DetectorConstructionMessenger.hh"
#include "DetectorConstruction.hh"
#include "G4UIdirectory.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWithABool.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWith3VectorAndUnit.hh"
#include "G4UIcmdWith3Vector.hh"
#include "G4UIcmdWithADouble.hh"
#include "G4RunManager.hh"

#include "G4ios.hh"

DetectorConstructionMessenger::DetectorConstructionMessenger(
    DetectorConstruction *detCon)
    : fDetCon(detCon) {
  fMuTomoDirectory = new G4UIdirectory("/muTomo/");
  fMuTomoDirectory->SetGuidance("Customize muon tomography geometry commands.");

  fTargetMaterialCmd = new G4UIcmdWithAString("/muTomo/targetMaterial", this);
  fTargetMaterialCmd->SetGuidance("Set target material.");
  fTargetMaterialCmd->SetParameterName("targetMaterial", true);
  fTargetMaterialCmd->SetDefaultValue("G4_Pb");

  fTargetSizeCmd = new G4UIcmdWith3VectorAndUnit("/muTomo/targetSize", this);
  fTargetSizeCmd->SetGuidance("Set target size.");
  fTargetSizeCmd->SetParameterName("targetSizeX", "targetSizeY", "targetSizeZ",
                                   true);
  fTargetSizeCmd->SetDefaultValue(G4ThreeVector(6., 2., 6.));
  fTargetSizeCmd->SetDefaultUnit("mm");

  fTargetLocationCmd =
      new G4UIcmdWith3VectorAndUnit("/muTomo/targetLocation", this);
  fTargetLocationCmd->SetGuidance("Set target location.");
  fTargetLocationCmd->SetParameterName("targetLocationX", "targetLocationY",
                                       "targetLocationZ", true);
  fTargetLocationCmd->SetDefaultValue(G4ThreeVector(0., 0., 0.));
  fTargetLocationCmd->SetDefaultUnit("mm");
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

DetectorConstructionMessenger::~DetectorConstructionMessenger() {
  delete fTargetMaterialCmd;
  delete fTargetSizeCmd;
  delete fTargetLocationCmd;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void DetectorConstructionMessenger::SetNewValue(G4UIcommand *command,
                                                G4String newValue) {
  if (command == fTargetMaterialCmd) {
    fDetCon->targetMaterialStr = newValue;
  }
  if (command == fTargetSizeCmd) {
    G4ThreeVector newPos = fTargetSizeCmd->GetNew3VectorValue(newValue);
    fDetCon->targetX = newPos.x();
    fDetCon->targetY = newPos.y();
    fDetCon->targetZ = newPos.z();
  }
  if (command == fTargetLocationCmd) {
    G4ThreeVector newLoc = fTargetLocationCmd->GetNew3VectorValue(newValue);
    fDetCon->targetLocationX = newLoc.x();
    fDetCon->targetLocationY = newLoc.y();
    fDetCon->targetLocationZ = newLoc.z();
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

G4String DetectorConstructionMessenger::GetCurrentValue(G4UIcommand *command) {
  G4String cv;

  if (command == fTargetMaterialCmd) {
    cv = fDetCon->targetMaterialStr;
  }
  if (command == fTargetSizeCmd) {
    cv = fTargetSizeCmd->ConvertToString(
        G4ThreeVector(fDetCon->targetX, fDetCon->targetY, fDetCon->targetZ));
  }
  if (command == fTargetLocationCmd) {
    cv = fTargetLocationCmd->ConvertToString(
        G4ThreeVector(fDetCon->targetLocationX, fDetCon->targetLocationY,
                      fDetCon->targetLocationZ));
  }

  return cv;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....
