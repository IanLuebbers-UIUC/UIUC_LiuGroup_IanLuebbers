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

  
  //~~Materials for testing the e+e- annihilation in Na22 Decay~~//
    //Cu Film Material
  G4Material* Cu = man->FindOrBuildMaterial("G4_Cu");
    //Na22 Source Material
  G4Isotope *Na22 = new G4Isotope("Na22", 11, 22);
  G4Element *elNa = new G4Element("rad sodium", "Na", 1);
  elNa->AddIsotope(Na22, 100*perCent);
  G4Material *Sodium = new G4Material("RadSource", 0.968*g/cm3, 1);
  Sodium->AddElement(elNa, 1);
    //Sodium Iodide to check Electron-Positron Annihilation
  G4Material* NaI = new G4Material("NaI", 3.67*g/cm3, 2);
  NaI->AddElement(man->FindOrBuildElement("Na"), 1);
  NaI->AddElement(man->FindOrBuildElement("I"), 1);

///////////////////////////////////////////////////////////////
///////////////////////Volume Definition///////////////////////
///////////////////////////////////////////////////////////////

    //Define Constants
  G4double zDistanceFromOrigin = 20*cm;
  G4bool PhysRadSource = false; //e+e- debug
  G4bool PhysCuFilm = false;    //e+e- debug

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

  //~~Geometries for testing the e+e- annihilation in Na22 Decay~~//
    //Copper Film
  if(PhysCuFilm==true)
  	{
		G4Tubs *solidCuFilm = new G4Tubs("solidCuFilm", 0, 1.5005*cm, 0.2505*cm, 0*deg, 360*deg);

		G4LogicalVolume *logicCuFilm = new G4LogicalVolume(solidCuFilm, Cu, "logicalCuFilm");

        	G4VPhysicalVolume *physCuFilm = new G4PVPlacement(0, G4ThreeVector(0,0,0), logicCuFilm, "physCuFilm", logicWorld, false, 0, false);

		G4Tubs *solidAirGap = new G4Tubs("solidAirGap", 0, 1.5*cm, 0.25*cm, 0*deg, 360*deg);

        	G4LogicalVolume *logicAirGap = new G4LogicalVolume(solidAirGap, Galactic, "logicalAirGap");

        	G4VPhysicalVolume *physAirGap = new G4PVPlacement(0, G4ThreeVector(0,0,0), logicAirGap, "physAirGap", logicCuFilm, false, 0, false);
  	}

    //Radiation Source
  if(PhysRadSource==true)
  	{
  		G4Tubs *solidRadSource = new G4Tubs("solidRadSource", 0, 1.5*cm, 0.25*cm, 0*deg, 360*deg);

  		G4LogicalVolume *logicRadSource = new G4LogicalVolume(solidRadSource, Sodium, "logicalRadSource");

  		G4VPhysicalVolume *physRadSource = new G4PVPlacement(0, G4ThreeVector(0,0,0), logicRadSource, "physRadSource", logicWorld, false, 0, false);
	}
	return physiWorld;
}


/*
//This is to test to see if there is any positron annihilation peak

    //World Volume
  G4Box* solidWorld = new G4Box("World", fWorldSize/2,fWorldSize/2,fWorldSize/2);

  G4LogicalVolume* logicWorld = new G4LogicalVolume(solidWorld, Galactic, "World");

  G4VPhysicalVolume* physiWorld = new G4PVPlacement(0, G4ThreeVector(), logicWorld, "World", 0, false, 0);

    //HPGe Detector  
  G4Tubs *solidDet = new G4Tubs("solidDet", 0, fWorldSize/2, fWorldSize/2, 0*deg, 360*deg);

  logicDet = new G4LogicalVolume(solidDet, Ge, "logicalDet");

  fScoringVolume = logicDet;
  fDetVolume = logicDet;

  G4VPhysicalVolume *physDet = new G4PVPlacement(0, G4ThreeVector(0, 0, 0), logicDet, "physDet", logicWorld, false, 0, false);

    //Air Gap
  G4Tubs *solidAG = new G4Tubs("solidAG", 0, 1*cm, 1*cm, 0*deg, 360*deg);

  G4LogicalVolume* logicAG = new G4LogicalVolume(solidAG , Galactic, "logicalAG");

  G4VPhysicalVolume *physAG = new G4PVPlacement(0, G4ThreeVector(0, 0, 0), logicAG, "physAG", logicWorld, false, 0, false);

  return physiWorld;
}
*/

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
