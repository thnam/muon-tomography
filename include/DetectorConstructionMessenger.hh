#ifndef DetectorConstructionMessenger_h
#define DetectorConstructionMessenger_h 1

class DetectorConstruction;
class G4UIdirectory;
class G4UIcmdWithADoubleAndUnit;
class G4UIcmdWithAIntAndUnit;
class G4UIcmdWith3VectorAndUnit;
class G4UIcmdWith3Vector;
class G4UIcmdWithABool;
class G4UIcmdWithAString;
class G4UIcmdWithADouble;

#include "G4UImessenger.hh"
#include "globals.hh"

class DetectorConstructionMessenger: public G4UImessenger
{
  public:
    DetectorConstructionMessenger(DetectorConstruction *detCon);
    ~DetectorConstructionMessenger();

    virtual void SetNewValue(G4UIcommand *command, G4String newValues);
    virtual G4String GetCurrentValue(G4UIcommand * command);

  private:
    DetectorConstruction *fDetCon;

    G4UIdirectory *fMuTomoDirectory;

    G4UIcmdWithAString *fTargetMaterialCmd;
    G4UIcmdWith3VectorAndUnit *fTargetSizeCmd;
    G4UIcmdWith3VectorAndUnit *fTargetLocationCmd;
};

#endif
