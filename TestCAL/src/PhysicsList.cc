#include "PhysicsList.hh"

PhysicsList::PhysicsList()/* : G4VModularPhysicsList(),
  fEmPhysicsList(0),
  fDecayPhysicsList(0)*/
{
  // set verbosity for zero to avoid double printout
  // on physics verbosity should be restored to 1 when cuts
  // are set
//  G4EmParameters::Instance()->SetVerbose(0);

//  SetDefaultCutValue(1*mm);

/*    RegisterPhysics (new G4EmStandardPhysics());
    RegisterPhysics (new G4DecayPhysics());
    RegisterPhysics (new G4RadioactiveDecayPhysics());

*/
  fDecayPhysicsList = new G4DecayPhysics();
  fRadioactiveDecayPhysicsList = new G4RadioactiveDecayPhysics();
  fEmPhysicsList = new G4EmLivermorePhysics(0);
//  fEmPhysicsList = new G4EmStandardPhysics(0);

//  SetVerboseLevel(1);
}

PhysicsList::~PhysicsList()
{
  delete fDecayPhysicsList;
  delete fRadioactiveDecayPhysicsList;
  delete fEmPhysicsList;
}

void PhysicsList::ConstructParticle()
{
  fDecayPhysicsList->ConstructParticle();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PhysicsList::ConstructProcess()
{
  AddTransportation();
  fEmPhysicsList->ConstructProcess();
  fDecayPhysicsList->ConstructProcess();
  fRadioactiveDecayPhysicsList->ConstructProcess();
//  AddStepMax();  
}
/*
void PhysicsList::AddStepMax()
{
  // Step limitation seen as a process
  fStepMaxProcess = new StepMax();

  auto particleIterator=GetParticleIterator();
  particleIterator->reset();
  while ((*particleIterator)())
  {
    G4ParticleDefinition* particle = particleIterator->value();
    G4ProcessManager* pmanager = particle->GetProcessManager();

    if (fStepMaxProcess->IsApplicable(*particle))
    {
      pmanager->AddDiscreteProcess(fStepMaxProcess);
    }
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PhysicsList::SetCuts()
{
  G4ProductionCutsTable::GetProductionCutsTable()->SetEnergyRange(100.*eV,1e5);
  if ( verboseLevel > 0 ) { DumpCutValuesTable(); }
}*/
