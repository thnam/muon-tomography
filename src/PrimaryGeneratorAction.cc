
#include "PrimaryGeneratorAction.hh"

#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4ParticleDefinition.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4RunManager.hh"
#include "G4SystemOfUnits.hh"
#include "Randomize.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PrimaryGeneratorAction::PrimaryGeneratorAction()
    : G4VUserPrimaryGeneratorAction(), fParticleGun(0), fEnvelopeBox(0) {
  G4int n_particle = 1;
  fParticleGun = new G4ParticleGun(n_particle);

  // default particle kinematic
  G4ParticleTable *particleTable = G4ParticleTable::GetParticleTable();
  G4String particleName;
  G4ParticleDefinition *particle =
      particleTable->FindParticle(particleName = "mu+");
  fParticleGun->SetParticleDefinition(particle);
  fParticleGun->SetParticleMomentumDirection(G4ThreeVector(0., 0., -1.));
  fParticleGun->SetParticleEnergy(4. * GeV);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PrimaryGeneratorAction::~PrimaryGeneratorAction() { delete fParticleGun; }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PrimaryGeneratorAction::GeneratePrimaries(G4Event *anEvent) {
  // this function is called at the begining of ecah event
  //

  // In order to avoid dependence of PrimaryGeneratorAction
  // on DetectorConstruction class we get Envelope volume
  // from G4LogicalVolumeStore.

  G4double worldSizeX = 0;
  G4double worldSizeY = 0;
  G4double worldSizeZ = 0;

  if (!fEnvelopeBox) {
    G4LogicalVolume *envLV =
        G4LogicalVolumeStore::GetInstance()->GetVolume("World");
    if (envLV) fEnvelopeBox = dynamic_cast<G4Box *>(envLV->GetSolid());
  }

  if (fEnvelopeBox) {
    worldSizeX = fEnvelopeBox->GetXHalfLength() * 2.;
    worldSizeY = fEnvelopeBox->GetYHalfLength() * 2.;
    worldSizeZ = fEnvelopeBox->GetZHalfLength() * 2.;
  } else {
    G4ExceptionDescription msg;
    msg << "Envelope volume of box shape not found.\n";
    msg << "Perhaps you have changed geometry.\n";
    msg << "The gun will be place at the center.";
    G4Exception("PrimaryGeneratorAction::GeneratePrimaries()", "MyCode0002",
                JustWarning, msg);
  }

  G4double x0 = worldSizeX * (G4UniformRand() - 0.5);
  G4double y0 = worldSizeY * (G4UniformRand() - 0.5);
  G4double z0 = 0.5 * worldSizeZ;

  fParticleGun->SetParticlePosition(G4ThreeVector(x0, y0, z0));

  fParticleGun->GeneratePrimaryVertex(anEvent);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
