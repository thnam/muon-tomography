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

  fTargetMaterialCmd =
      new G4UIcmdWithAString("/muTomo/setTargetMaterial", this);
  fTargetMaterialCmd->SetGuidance("Set target material.");
  fTargetMaterialCmd->SetParameterName("targetMaterial", true);
  fTargetMaterialCmd->SetDefaultValue("G4_Pb");

  fTargetSizeCmd = new G4UIcmdWith3VectorAndUnit("/muTomo/setTargetSize", this);
  fTargetSizeCmd->SetGuidance("Set target size.");
  fTargetSizeCmd->SetParameterName("muTomoSizeX", "muTomoSizeY", "muTomoSizeZ",
                                   true);
  fTargetSizeCmd->SetDefaultValue(G4ThreeVector(6., 2., 6.));
  fTargetSizeCmd->SetDefaultUnit("mm");
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

DetectorConstructionMessenger::~DetectorConstructionMessenger() {
  delete fTargetMaterialCmd;
  delete fTargetSizeCmd;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void DetectorConstructionMessenger::SetNewValue(G4UIcommand *command,
                                                G4String newValue) {
  // if (command == fTargetMaterialCmd) {
  // fDetCon->SetMaterial(newValue);
  // }
  // if (command == fTargetSizeCmd) {
  // fDetCon->SetSizes(fTargetSizeCmd->GetNew3VectorValue(newValue));
  // }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

G4String DetectorConstructionMessenger::GetCurrentValue(G4UIcommand *command) {
  G4String cv;

  // if (command == fTargetMaterialCmd) {
  // cv = fDetCon->GetMaterial();
  // }
  // if (command == fTargetSizeCmd) {
  // cv = fTargetSizeCmd->ConvertToString(fDetCon->GetSizes(), "mm");
  // }
  return cv;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....
