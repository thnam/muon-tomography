
#ifndef DetectorConstruction_h
#define DetectorConstruction_h 1

#include "G4VUserDetectorConstruction.hh"
#include "globals.hh"

class G4VPhysicalVolume;
class G4LogicalVolume;

/// Detector construction class to define materials and geometry.

class DetectorConstruction : public G4VUserDetectorConstruction
{
  public:
    DetectorConstruction();
    virtual ~DetectorConstruction();

    virtual G4VPhysicalVolume* Construct();
    virtual void ConstructSDandField();

    const G4String GetTargetMaterial() { return fTargetMaterial; }
    void SetTargetMaterial(const G4String mat) { fTargetMaterial = mat; }

  protected:
    // void DefineMaterials();
    G4bool  fCheckOverlaps;
    G4double scSizeX;
    G4double scSizeY;
    G4double scSizeZ;
    G4double scGapX;
    G4double scGapZ;
    G4int nScBar;
    G4int nLayerTop;
    G4int nLayerBottom;

    G4double layerGapZ;
    G4double topBotGap;

    G4double layerSizeX;
    G4double layerSizeY;
    G4double layerSizeZ;

    G4double concreteX;
    G4double concreteY;
    G4double concreteZ;
    G4double targetX;
    G4double targetY;
    G4double targetZ;
    G4String fTargetMaterial;

    G4double worldSizeX;
    G4double worldSizeY;
    G4double worldSizeZ;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

