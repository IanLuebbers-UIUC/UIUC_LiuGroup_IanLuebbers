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
/// \file PrimaryGeneratorAction.cc
/// \brief Implementation of the PrimaryGeneratorAction class
//
// 
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
#include "PrimaryGeneratorAction.hh"

#include "G4Event.hh"
#include "G4ParticleTable.hh"
#include "G4IonTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4Geantino.hh"
#include "G4SystemOfUnits.hh"
#include "Randomize.hh"
#include "G4AnalysisManager.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PrimaryGeneratorAction::PrimaryGeneratorAction()
 : G4VUserPrimaryGeneratorAction(),
   fParticleGun(0)
{
  G4int n_particle = 1;
  fParticleGun  = new G4ParticleGun(n_particle);

  fParticleGun->SetParticleEnergy(0*eV);
  fParticleGun->SetParticlePosition(G4ThreeVector(0.,0.,0.));
  fParticleGun->SetParticleMomentumDirection(G4ThreeVector(1.,0.,0.));          
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PrimaryGeneratorAction::~PrimaryGeneratorAction()
{
  delete fParticleGun;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{ 
    auto man = G4AnalysisManager::Instance();
    G4double ionCharge   = 0.*eplus;
    G4double excitEnergy = 0.*keV;
    
    G4int ColumnNo;
    G4double IsotopeProb;

    //Cycle through the six isotopes
    G4int EvtNo = anEvent->GetEventID();
//    G4cout << "Event Number: " << EvtNo << G4endl;
//    G4cout << "Event Number mod 6: " << EvtNo%6 << G4endl;
    /*man->FillNtupleIColumn(3, 0, EvtNo);
    man->AddNtupleRow(3);*/

    //Generate Random Numbers & Track them
    RandNo = G4UniformRand();
    /*man->FillNtupleDColumn(3, 1, RandNo);
    man->AddNtupleRow(3);*/

    G4double randNx = G4UniformRand() - 0.5;
    G4double randNz = G4UniformRand() - 0.5;
    
    /*man->FillNtupleDColumn(3, 2, randNx);
    man->AddNtupleRow(3);
    man->FillNtupleDColumn(3, 3, randNz);
    man->AddNtupleRow(3);*/

    //Cycle through each isotope based on event number    
    if (EvtNo%6 == 0) //Iodine-131
    {
	    Z = 53;
	    A = 131;
	    ColumnNo = 0;
	    IsotopeProb = 0.75;

	    if (RandNo <= IsotopeProb)
	    {
		    G4ParticleDefinition* ion = G4IonTable::GetIonTable()->GetIon(Z,A,excitEnergy);
		    fParticleGun->SetParticleDefinition(ion);
		    fParticleGun->SetParticleCharge(ionCharge); 
		    fParticleGun->SetParticlePosition(G4ThreeVector(randNx*m, -0.5*m, randNz*m));
		    man->FillNtupleIColumn(1, ColumnNo, 1);
		    man->AddNtupleRow(1);
	    }
	    else if (RandNo > IsotopeProb)
	    {
		    man->FillNtupleIColumn(1, ColumnNo, 0);
		    man->AddNtupleRow(1);
	    }
    }

    else if (EvtNo%6 == 1) //Chromium-51
    {
            Z = 24;
            A = 51;
            ColumnNo = 1;
            IsotopeProb = 0.40;

	    if (RandNo <= IsotopeProb)
            {
                    G4ParticleDefinition* ion = G4IonTable::GetIonTable()->GetIon(Z,A,excitEnergy);
                    fParticleGun->SetParticleDefinition(ion);
                    fParticleGun->SetParticleCharge(ionCharge);
                    fParticleGun->SetParticlePosition(G4ThreeVector(randNx*m, -0.5*m, randNz*m));
                    man->FillNtupleIColumn(1, ColumnNo, 1);
                    man->AddNtupleRow(1);
            }
            else if (RandNo > IsotopeProb)
            {
                    man->FillNtupleIColumn(1, ColumnNo, 0);
                    man->AddNtupleRow(1);
            }
    }

    else if (EvtNo%6 == 2) //Hydrogen-3
    {
            Z = 1;
            A = 3;
            ColumnNo = 2;
            IsotopeProb = 0.50;
    
            if (RandNo <= IsotopeProb)
            {
                    G4ParticleDefinition* ion = G4IonTable::GetIonTable()->GetIon(Z,A,excitEnergy);
                    fParticleGun->SetParticleDefinition(ion);
                    fParticleGun->SetParticleCharge(ionCharge);
		    fParticleGun->SetParticlePosition(G4ThreeVector(randNx*m, -0.5*m, randNz*m));
                    man->FillNtupleIColumn(1, ColumnNo, 1);
                    man->AddNtupleRow(1);
            }
            else if (RandNo > IsotopeProb)
            {
                    man->FillNtupleIColumn(1, ColumnNo, 0);
                    man->AddNtupleRow(1);
            }    
    }

    else if (EvtNo%6 == 3) //Cobalt-60
    {
            Z = 27;
            A = 60;
            ColumnNo = 3;
            IsotopeProb = 0.65;
    
            if (RandNo <= IsotopeProb)
            {
                    G4ParticleDefinition* ion = G4IonTable::GetIonTable()->GetIon(Z,A,excitEnergy);
                    fParticleGun->SetParticleDefinition(ion);
                    fParticleGun->SetParticleCharge(ionCharge);
		    fParticleGun->SetParticlePosition(G4ThreeVector(randNx*m, -0.5*m, randNz*m));
                    man->FillNtupleIColumn(1, ColumnNo, 1);
                    man->AddNtupleRow(1);
            }
            else if (RandNo > IsotopeProb)
            {
                    man->FillNtupleIColumn(1, ColumnNo, 0);
                    man->AddNtupleRow(1);
            }    
    }

    else if (EvtNo%6 == 4) //Ruthenium-103
    {
            Z = 44;
            A = 103;
            ColumnNo = 4;
            IsotopeProb = 0.42;
    
            if (RandNo <= IsotopeProb)
            {
                    G4ParticleDefinition* ion = G4IonTable::GetIonTable()->GetIon(Z,A,excitEnergy);
                    fParticleGun->SetParticleDefinition(ion);
                    fParticleGun->SetParticleCharge(ionCharge);
		    fParticleGun->SetParticlePosition(G4ThreeVector(randNx*m, -0.5*m, randNz*m));
                    man->FillNtupleIColumn(1, ColumnNo, 1);
                    man->AddNtupleRow(1);
            }
            else if (RandNo > IsotopeProb)
            {
                    man->FillNtupleIColumn(1, ColumnNo, 0);
                    man->AddNtupleRow(1);
            }    
    }

    else if (EvtNo%6 == 5) //Technetium-99
    {
            Z = 43;
            A = 99;
            ColumnNo = 5;
            IsotopeProb = 0.39;
    
            if (RandNo <= IsotopeProb)
            {
                    G4ParticleDefinition* ion = G4IonTable::GetIonTable()->GetIon(Z,A,excitEnergy);
                    fParticleGun->SetParticleDefinition(ion);
                    fParticleGun->SetParticleCharge(ionCharge);
		    fParticleGun->SetParticlePosition(G4ThreeVector(randNx*m, -0.5*m, randNz*m));
                    man->FillNtupleIColumn(1, ColumnNo, 1);
                    man->AddNtupleRow(1);
            }
            else if (RandNo > IsotopeProb)
            {
                    man->FillNtupleIColumn(1, ColumnNo, 0);
                    man->AddNtupleRow(1);
            }    
    }

    //Determine if Isotope will decay
    
    /*auto man = G4AnalysisManager::Instance();
    if (RandNo <= IsotopeProb)
    {
	    G4ParticleDefinition* ion = G4IonTable::GetIonTable()->GetIon(Z,A,excitEnergy);
	    fParticleGun->SetParticleDefinition(ion);
	    fParticleGun->SetParticleCharge(ionCharge);
	    fParticleGun->SetParticlePosition(G4ThreeVector(randNx*m, -1*m, randNz*m));
	    man->FillNtupleIColumn(1, ColumnNo, 1);
	    man->AddNtupleRow(1); 
    }
    else if (RandNo > IsotopeProb)
    {
	    man->FillNtupleIColumn(1, ColumnNo, 0);
	    man->AddNtupleRow(1);
    }*/   

  //create vertex 
  fParticleGun->GeneratePrimaryVertex(anEvent);
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
