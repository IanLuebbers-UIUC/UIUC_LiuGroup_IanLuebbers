//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
/// \file RunAction.cc
/// \brief Implementation of the RunAction class
//
// 
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "RunAction.hh"
#include "Run.hh"
#include "PrimaryGeneratorAction.hh"
//#include "HistoManager.hh"
#include "G4AnalysisManager.hh"

#include "G4Run.hh"
#include "G4UnitsTable.hh"
#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"
#include <iomanip>

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

RunAction::RunAction(PrimaryGeneratorAction* kin)
:G4UserRunAction(),
 fPrimary(kin), fRun(0)//, fDetConst(0)//, fHistoManager(0)
{
//  fHistoManager = new HistoManager();
  auto man = G4AnalysisManager::Instance();

  //G4cout << "Print length: " << fDetConst->zLength << G4endl;

//  man->SetVerboseLevel(3);
//  man->SetNtupleMerging(true);

  man->CreateNtuple("Edep","Edep");
  man->CreateNtupleDColumn("fEdep_HPGe");
  man->FinishNtuple(0);

  man->CreateNtuple("Decay","Decay");
  man->CreateNtupleDColumn("Decay_GammaEnergies");
  man->FinishNtuple(1);

  man->CreateNtuple("Incident","Incident");
  man->CreateNtupleIColumn("Incident_Gammas");
  man->FinishNtuple(2);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

RunAction::~RunAction()
{ 
//  delete fHistoManager;
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4Run* RunAction::GenerateRun()
{ 
  fRun = new Run();
  return fRun;
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void RunAction::BeginOfRunAction(const G4Run* run/*, DetectorConstruction &DCobj*/) //Commented code: BFS
{ 
  // keep run condition
  if (fPrimary) { 
    G4ParticleDefinition* particle 
      = fPrimary->GetParticleGun()->GetParticleDefinition();
    G4double energy = fPrimary->GetParticleGun()->GetParticleEnergy();
    fRun->SetPrimary(particle, energy);
  }    
     
  auto man = G4AnalysisManager::Instance();

//BFS
//  G4double diam = DCobj.diameter;
//  G4double len = DCobj.zLength;

//  G4String DiameterName = to_string(diam);
//  G4String ThicknessName = to_string(len);

  //Label each run for AllIsotopes.mac with their calibration source
  G4String IsotopeName = "";
  G4int runID = run->GetRunID();
  if(runID==0)
  {
	IsotopeName = "Co60";
  }
  if(runID==1)
  {
        IsotopeName = "Cs137";
  }
  if(runID==2)
  {
        IsotopeName = "Na22";
  }

  man->OpenFile("../rootFiles/TestCAL_" + IsotopeName + ".root");
  //histograms
  //
//  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
//  if ( analysisManager->IsActive() ) {
//    analysisManager->OpenFile();
//  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void RunAction::EndOfRunAction(const G4Run*)
{
 if (isMaster) fRun->EndOfRun();
            
 auto man = G4AnalysisManager::Instance();
 man->Write();
 man->CloseFile();
 //save histograms
 //
// G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
// if ( analysisManager->IsActive() ) {
//  analysisManager->Write();
//  analysisManager->CloseFile();
// } 
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
