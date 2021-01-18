#include "DetectorConstruction.hh"

#include "DetectorConstructionMessenger.hh"
#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4MultiFunctionalDetector.hh"
#include "G4NistManager.hh"
#include "G4PSDoseDeposit.hh"
#include "G4PSEnergyDeposit.hh"
#include "G4PVPlacement.hh"
#include "G4PhysicalConstants.hh"
#include "G4RotationMatrix.hh"
#include "G4RunManager.hh"
#include "G4SDManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4Transform3D.hh"
#include "G4Tubs.hh"
#include "G4VPrimitiveScorer.hh"
#include "G4VisAttributes.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorConstruction::DetectorConstruction()
    : G4VUserDetectorConstruction(), targetMaterialStr("G4_Pb"),
      fCheckOverlaps(false) {
  fDetConMessenger = new DetectorConstructionMessenger(this);
  // DefineMaterials();
}

DetectorConstruction::~DetectorConstruction() { delete fDetConMessenger; }

G4VPhysicalVolume *DetectorConstruction::Construct() {
  G4NistManager *nist = G4NistManager::Instance();
  G4Material *default_mat = nist->FindOrBuildMaterial("G4_AIR");
  G4Material *targetMaterial = nist->FindOrBuildMaterial(targetMaterialStr);
  G4Material *concreteMaterial = nist->FindOrBuildMaterial("G4_CONCRETE");
  G4Material *scMaterial =
      nist->FindOrBuildMaterial("G4_PLASTIC_SC_VINYLTOLUENE");

  // ComputeGeometry first
  ComputeGeometry();
  // world volume
  G4Box *solidWorld =
      new G4Box("World", 0.5 * worldSizeX, 0.5 * worldSizeY, 0.5 * worldSizeZ);
  G4LogicalVolume *logicWorld =
      new G4LogicalVolume(solidWorld, default_mat, "World");
  G4VPhysicalVolume *physWorld =
      new G4PVPlacement(0,                // no rotation
                        G4ThreeVector(),  // at (0,0,0)
                        logicWorld,       // its logical volume
                        "World",          // its name
                        0,                // its mother  volume
                        false,            // no boolean operation
                        0,                // copy number
                        fCheckOverlaps);  // overlaps checking

  // concrete box and target
  G4Box *solidConcrete =
      new G4Box("concrete", concreteX / 2, concreteY / 2, concreteZ / 2);
  G4LogicalVolume *logConcrete =
      new G4LogicalVolume(solidConcrete, concreteMaterial, "concrete");
  new G4PVPlacement(0, G4ThreeVector(), logConcrete, "concrete", logicWorld,
                    false, 0, fCheckOverlaps);

  G4Box *solidTarget =
      new G4Box("target", targetX / 2, targetY / 2, targetZ / 2);
  G4LogicalVolume *logTarget =
      new G4LogicalVolume(solidTarget, targetMaterial, "target");
  new G4PVPlacement(
      0, G4ThreeVector(targetLocationX, targetLocationY, targetLocationZ),
      logTarget, "target", logConcrete, false, 0, fCheckOverlaps);

  // scintillators
  G4Box *solidScX =
      new G4Box("solidScX", 0.5 * scSizeX, 0.5 * scSizeY, 0.5 * scSizeZ);
  G4LogicalVolume *logicScX =
      new G4LogicalVolume(solidScX, scMaterial, "logicScX");

  G4Box *solidScY =
      new G4Box("solidScY", 0.5 * scSizeY, 0.5 * scSizeX, 0.5 * scSizeZ);
  G4LogicalVolume *logicScY =
      new G4LogicalVolume(solidScY, scMaterial, "logicScY");

  // G4Box *solidLayer =
  // new G4Box("Layer", 0.5 * layerSizeX, 0.5 * layerSizeY, 0.5 * layerSizeZ);
  // G4LogicalVolume *logicLayer =
  // new G4LogicalVolume(solidLayer, default_mat, "logicLayer");

  G4double scOffset = 0. * cm;
  G4double copyNo = 0;

  G4double layerZPosition[4] = {
      -topBotGap / 2 - 2 * layerSizeZ - 2 * layerGapZ, // layer 0
      -topBotGap / 2,                                  // layer 1
      topBotGap / 2,                                   // layer 2
      topBotGap / 2 + 2 * layerSizeZ + 2 * layerGapZ}; // layer 3
  G4RotationMatrix *rotZ90 = new G4RotationMatrix();
  rotZ90->rotateZ(90 * deg);

  // place solid Scintillator into X and Y layers
  G4double Zpos = 0.;
  for (int iLayer = 0; iLayer < nLayerTop + nLayerBottom; iLayer++) {
    for (int iBar = 0; iBar < nScBar; iBar++) {
      scOffset = iBar * (scGapX + scSizeX) - layerSizeX / 2 + scSizeX / 2;
      copyNo = iBar + nScBar * iLayer;
      Zpos = layerZPosition[iLayer];
      new G4PVPlacement(0,                                // rotation
                        G4ThreeVector(scOffset, 0, Zpos), // offset
                        logicScX,                         // logical volume
                        "ScX",                            // name
                        logicWorld,                       // mother  volume
                        false,           // no boolean operation
                        copyNo,          // copy number
                        fCheckOverlaps); // overlaps checking
      Zpos = layerZPosition[iLayer] + layerSizeZ + layerGapZ;
      new G4PVPlacement(0,                                // rotation
                        G4ThreeVector(0, scOffset, Zpos), // offset
                        logicScY,                         // logical volume
                        "ScY",                            // name
                        logicWorld,                       // mother  volume
                        false,           // no boolean operation
                        copyNo,          // copy number
                        fCheckOverlaps); // overlaps checking
    }
  }

  //------------------------------------------------
  // Visualization attributes
  //------------------------------------------------

  logicWorld->SetVisAttributes(G4VisAttributes::Invisible);
  logicScX->SetVisAttributes(new G4VisAttributes(G4Colour(.0, 1.0, 1., 1.0)));
  logicScY->SetVisAttributes(new G4VisAttributes(G4Colour(0.0, 0.0, 1., 0.4)));
  logConcrete->SetVisAttributes(new G4VisAttributes(G4Colour(.8, .3, .3, 0.3)));
  logTarget->SetVisAttributes(new G4VisAttributes(G4Colour(.3, 1., .3, 1.0)));

  // always return the physical World
  //
  return physWorld;
}

void DetectorConstruction::ConstructSDandField() {
  G4SDManager::GetSDMpointer()->SetVerboseLevel(0);

  G4MultiFunctionalDetector *mfDetX = new G4MultiFunctionalDetector("detX");
  G4SDManager::GetSDMpointer()->AddNewDetector(mfDetX);
  G4VPrimitiveScorer *primitiv1 = new G4PSEnergyDeposit("edep");
  mfDetX->RegisterPrimitive(primitiv1);
  SetSensitiveDetector("logicScX", mfDetX);

  G4MultiFunctionalDetector *mfDetY = new G4MultiFunctionalDetector("detY");
  G4SDManager::GetSDMpointer()->AddNewDetector(mfDetY);
  G4VPrimitiveScorer *primitiv2 = new G4PSEnergyDeposit("edep");
  mfDetY->RegisterPrimitive(primitiv2);
  SetSensitiveDetector("logicScY", mfDetY);
}

void DetectorConstruction::ComputeGeometry() {
  scSizeX = 5.0 * cm;
  scSizeY = 50.0 * cm;
  scSizeZ = 1. * cm;
  scGapX = 0.2 * cm;
  scGapZ = 0.2 * cm;
  nScBar = 10;
  nLayerTop = 2;
  nLayerBottom = 2;

  layerGapZ = 1 * cm;
  topBotGap = 50 * cm;

  layerSizeX = nScBar * scSizeX + nScBar * scGapX;
  layerSizeY = nScBar * scSizeX + nScBar * scGapX;
  layerSizeZ = scSizeZ + scGapZ;

  concreteX = 30 * cm;
  concreteY = 30 * cm;
  concreteZ = 30 * cm;
  targetX = 10 * cm;
  targetY = 10 * cm;
  targetZ = 5 * cm;

  worldSizeX = 1.2 * layerSizeX;
  worldSizeY = 1.2 * layerSizeY;
  worldSizeZ =
      1.2 * ((nLayerBottom + nLayerTop) * (layerSizeZ + layerGapZ) + topBotGap);
}
