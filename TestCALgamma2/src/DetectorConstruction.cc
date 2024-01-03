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
/// \file DetectorConstruction.cc
/// \brief Implementation of the DetectorConstruction class
//
// 
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "DetectorConstruction.hh"

#include "G4NistManager.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorConstruction::DetectorConstruction()
 : G4VUserDetectorConstruction()
{
  fWorldSize = 2*m; //Set the world size here
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorConstruction::~DetectorConstruction()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume* DetectorConstruction::Construct()
{
///////////////////////////////////////////////////////////////
//////////////////Material Definition//////////////////////////
///////////////////////////////////////////////////////////////

  G4NistManager *man = G4NistManager::Instance();

    // Vacuum using NIST Manager (From Jin Choi's Simulation)
  G4Material* Galactic = new G4Material("Galactic", universe_mean_density, 1);
  Galactic->AddElement(man->FindOrBuildElement("H"),1);

    //Germanium for HPGe detector
  G4Material* Ge = man->FindOrBuildMaterial("G4_Ge");

///////////////////////////////////////////////////////////////
///////////////////////Volume Definition///////////////////////
///////////////////////////////////////////////////////////////

    //Define Constants
  
  G4double zDistanceFromOrigin = 20*cm; //Adjust detector position here

    //World Volume
  G4Box* solidWorld = new G4Box("World", fWorldSize/2,fWorldSize/2,fWorldSize/2);
  G4LogicalVolume* logicWorld = new G4LogicalVolume(solidWorld, Galactic, "World");
  G4VPhysicalVolume* physiWorld = new G4PVPlacement(0, G4ThreeVector(), logicWorld, "World", 0, false, 0);

    //HPGe Detector
  G4Tubs *solidDet = new G4Tubs("solidDet", 0, diameter/2, zLength/2, 0*deg, 360*deg);
  logicDet = new G4LogicalVolume(solidDet, Ge, "logicalDet");
  fScoringVolume = logicDet;
  fDetVolume = logicDet;
  G4VPhysicalVolume *physDet = new G4PVPlacement(0, G4ThreeVector(0, 0, zDistanceFromOrigin+zLength/2), logicDet, "physDet", logicWorld, false, 0, false);

  return physiWorld;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
