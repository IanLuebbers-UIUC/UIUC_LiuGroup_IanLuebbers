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

  //Tabulate deposited energy on the detector
  man->CreateNtuple("Edep","Edep");
  man->CreateNtupleDColumn("fEdep_HPGe");
  man->FinishNtuple(0);

  //Tabulate isotope decay count
  man->CreateNtuple("Isotope","Isotope");
  man->CreateNtupleIColumn("I131_Count");
  man->CreateNtupleIColumn("Cr51_Count");
  man->CreateNtupleIColumn("H3_Count");
  man->CreateNtupleIColumn("Co60_Count");
  man->CreateNtupleIColumn("Ru103_Count");
  man->CreateNtupleIColumn("Tc99_Count");
  man->FinishNtuple(1);

  //Tabulate incident gamma count on detector (broken)
  man->CreateNtuple("Incident","Incident");
  man->CreateNtupleIColumn("Incident_Gammas");
  man->FinishNtuple(2);

  //Tabulate statistics of isotope generation
  man->CreateNtuple("StatTracker","StatTracker");
  man->CreateNtupleIColumn("EventMod6");
  man->CreateNtupleDColumn("RandNoProbTracker");
  man->CreateNtupleDColumn("XposTracker");
  man->CreateNtupleDColumn("ZposTracker");
  man->FinishNtuple(3);
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

void RunAction::BeginOfRunAction(const G4Run*)
{ 
  // keep run condition
  if (fPrimary) { 
    G4ParticleDefinition* particle 
      = fPrimary->GetParticleGun()->GetParticleDefinition();
    G4double energy = fPrimary->GetParticleGun()->GetParticleEnergy();
    fRun->SetPrimary(particle, energy);
  }    
     
  auto man = G4AnalysisManager::Instance();

  //Manually enter dimensions here
  G4String DiameterName = "";
  G4String ThicknessName = "";

  man->OpenFile("../rootFiles/TestBKG" + DiameterName + ThicknessName + ".root");
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
