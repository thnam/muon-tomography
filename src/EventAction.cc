
#include "EventAction.hh"

#include "Analysis.hh"
#include "G4Event.hh"
#include "G4HCofThisEvent.hh"
#include "G4RunManager.hh"
#include "G4SDManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4THitsMap.hh"
#include "G4UnitsTable.hh"
#include "RunAction.hh"

EventAction::EventAction(RunAction *)
    : G4UserEventAction(), fCollID_detX(-1), fCollID_detY(-1) {}

EventAction::~EventAction() {}

void EventAction::BeginOfEventAction(const G4Event *) {}

void EventAction::EndOfEventAction(const G4Event *evt) {
  G4HCofThisEvent *HCE = evt->GetHCofThisEvent();
  if (!HCE) return;

  if (fCollID_detX < 0) {
    G4SDManager *SDMan = G4SDManager::GetSDMpointer();
    fCollID_detX = SDMan->GetCollectionID("detX/edep");
  }

  if (fCollID_detY < 0) {
    G4SDManager *SDMan = G4SDManager::GetSDMpointer();
    fCollID_detY = SDMan->GetCollectionID("detY/edep");
  }

  auto anaMan = G4AnalysisManager::Instance();

  G4THitsMap<G4double> *evtMap =
      (G4THitsMap<G4double> *)(HCE->GetHC(fCollID_detX));
  std::map<G4int, G4double *>::iterator itr;
  for (itr = evtMap->GetMap()->begin(); itr != evtMap->GetMap()->end(); itr++) {
    G4int copyNb = (itr->first);
    G4double edep = *(itr->second);
    // G4cout << "\n  detX" << copyNb << ": " << edep / keV << " keV ";
    anaMan->FillNtupleDColumn(copyNb, edep / MeV);
  }

  evtMap = (G4THitsMap<G4double> *)(HCE->GetHC(fCollID_detY));
  for (itr = evtMap->GetMap()->begin(); itr != evtMap->GetMap()->end(); itr++) {
    G4int copyNb = (itr->first);
    G4double edep = *(itr->second);
    // G4cout << "\n  detX" << copyNb << ": " << edep / keV << " keV ";
    anaMan->FillNtupleDColumn(copyNb + 40, edep / MeV);
  }

  anaMan->AddNtupleRow();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
