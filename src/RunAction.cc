
#include "RunAction.hh"

#include "Analysis.hh"
#include "DetectorConstruction.hh"
#include "PrimaryGeneratorAction.hh"
// #include "Run.hh"

#include "G4AccumulableManager.hh"
#include "G4LogicalVolume.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4Run.hh"
#include "G4RunManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4UnitsTable.hh"

RunAction::RunAction() : G4UserRunAction() {
  auto anaMan = G4AnalysisManager::Instance();
  G4cout << "Using " << anaMan->GetType() << G4endl;

  // Create ntuple, first 40 columns are x hits, last 40 columns are y hits
  anaMan->SetVerboseLevel(0);
  anaMan->SetNtupleMerging(true);

  anaMan->CreateNtuple("edep", "Energy deposit [MeV]");
  char columnName[20];
  for (int i = 0; i < 40; i++) {
    snprintf(columnName, 20, "x%02d", i);
    anaMan->CreateNtupleDColumn(G4String(columnName));
  }
  for (int i = 0; i < 40; i++) {
    snprintf(columnName, 20, "y%02d", i);
    anaMan->CreateNtupleDColumn(G4String(columnName));
  }

  anaMan->FinishNtuple();
}

RunAction::~RunAction() { delete G4AnalysisManager::Instance(); }

void RunAction::BeginOfRunAction(const G4Run *) {
  // inform the runManager to save random number seed
  G4RunManager::GetRunManager()->SetRandomNumberStore(false);

  // Get analysis manager
  auto anaMan = G4AnalysisManager::Instance();


  // Open an output file
  //
  G4String fileName = "edep.root";
  anaMan->OpenFile(fileName);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void RunAction::EndOfRunAction(const G4Run *) {
  auto anaMan = G4AnalysisManager::Instance();
  anaMan->Write();
  anaMan->CloseFile();
}
