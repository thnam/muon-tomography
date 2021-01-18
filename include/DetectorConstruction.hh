
#ifndef DetectorConstruction_h
#define DetectorConstruction_h 1

#include "G4VUserDetectorConstruction.hh"
#include "globals.hh"

class G4VPhysicalVolume;
class G4LogicalVolume;
class DetectorConstructionMessenger;

/// Detector construction class to define materials and geometry.

class DetectorConstruction : public G4VUserDetectorConstruction
{
  public:
    DetectorConstruction();
    virtual ~DetectorConstruction();

    virtual G4VPhysicalVolume* Construct();
    virtual void ConstructSDandField();

    void ComputeGeometry();

    // public fields to avoid all getters and setters
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
    G4double targetLocationX;
    G4double targetLocationY;
    G4double targetLocationZ;
    G4String targetMaterialStr;

    G4double worldSizeX;
    G4double worldSizeY;
    G4double worldSizeZ;

  protected:
    // void DefineMaterials();
    G4bool fCheckOverlaps;
    DetectorConstructionMessenger *fDetConMessenger;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

