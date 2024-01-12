#include "stepping.hh"
#include "G4RunManager.hh"
#include "G4Gamma.hh"

MySteppingAction::MySteppingAction(EventAction *eventAction)
{
	fEventAction = eventAction;
}

MySteppingAction::~MySteppingAction()
{}

void MySteppingAction::UserSteppingAction(const G4Step *step)
{
	G4LogicalVolume *volume = step->GetPreStepPoint()->GetTouchableHandle()->GetVolume()->GetLogicalVolume();

	G4ParticleDefinition *part = step->GetTrack()->GetDefinition();

	const DetectorConstruction *detectorConstruction = static_cast<const DetectorConstruction*> (G4RunManager::GetRunManager()->GetUserDetectorConstruction());

	G4LogicalVolume *fScoringVolume = detectorConstruction->GetScoringVolume();

	//Add deposited energy at each step within the detector
	//This step must be done to confirm that the deposited energy is added only when it is absorbed into the detector
	if(volume == fScoringVolume)
        {
		G4double edep = step->GetTotalEnergyDeposit();
		fEventAction->AddEdep(edep);
	}

	//Check to see if the particle is a gamma and hit the detector
	if(volume == fScoringVolume and part == G4Gamma::Gamma())
	{	
		fEventAction->IncidentIndicator();
	}
}


