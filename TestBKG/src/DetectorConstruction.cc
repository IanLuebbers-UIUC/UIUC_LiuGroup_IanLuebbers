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
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorConstruction::DetectorConstruction()
 : G4VUserDetectorConstruction()
{
  fWorldSize = 1*m;
  
  //Initialize custom messenger to adjust parameters
  fMessenger = new G4GenericMessenger(this, "/detector/", "Detector Construction");

  fMessenger->DeclareProperty("isShielded", isShielded, "Load in Lead Shielding");
  fMessenger->DeclareProperty("rThickness", rThickness, "If isShielded is true, set Thickness of Lead Shield. Unit = cm.");
  fMessenger->DeclareProperty("dDiameter", dDiameter, "Set Diameter of HPGe. Unit = cm.");
  fMessenger->DeclareProperty("zLength", zLength, "Set Length of HPGe. Unit = cm.");
  fMessenger->DeclareProperty("outerShieldDiameter", outerShieldDiameter, "Set Outer Diameter of the Outer Shield (S3). Unit = in.");

  //Set default values here (these can be changed here for debugging)
  isShielded = true;
  rThickness = 5;
  dDiameter = 8;
  zLength = 3;
  outerShieldDiameter = 8; /*This will be changed*/

  DefineMaterials();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorConstruction::~DetectorConstruction()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::DefineMaterials()
{
  G4NistManager *man = G4NistManager::Instance();

    // Vacuum using NIST Manager //From Jin's Simulation
  Galactic = new G4Material("Galactic", universe_mean_density, 1);
  Galactic->AddElement(man->FindOrBuildElement("H"),1);

    //Background Radiation Source (1st attempt to make geometry-defined background
  /*
  G4Isotope *Co60 = new G4Isotope("Co60", 27, 60);
  G4Element *elCo = new G4Element("rad cobalt", "Co", 1);
  elCo->AddIsotope(Co60, 100*perCent);
  Cobalt = new G4Material("RadSource", 8.90*g/cm3, 1);
  Cobalt->AddElement(elCo, 1);
  */

    //Elemental Materials
  Ge = man->FindOrBuildMaterial("G4_Ge");
  Cu = man->FindOrBuildMaterial("G4_Cu");
  Pb = man->FindOrBuildMaterial("G4_Pb");
}

G4VPhysicalVolume* DetectorConstruction::Construct()
{
    //Define Constants
  
  G4double zDistanceFromOrigin = 0; //In this simulation, the detector is centered at the origin
  G4cout << "zLength = " << zLength << G4endl;

    //World Volume
  solidWorld = new G4Box("World", fWorldSize/2,fWorldSize/2,fWorldSize/2);
  logicWorld = new G4LogicalVolume(solidWorld, Galactic, "World");
  physiWorld = new G4PVPlacement(0, G4ThreeVector(), logicWorld, "World", 0, false, 0);

    //HPGe Detector
  solidDet = new G4Tubs("Det", 0, (dDiameter/2)*cm, (zLength/2)*cm, 0*deg, 360*deg);
  logicDet = new G4LogicalVolume(solidDet, Ge, "Det");
  fScoringVolume = logicDet;
  physDet = new G4PVPlacement(0, G4ThreeVector(0, 0, (zDistanceFromOrigin+(zLength/2))*cm), logicDet, "Det", logicWorld, false, 0, false);

/*    //Background Source
  G4Box *solidSource = new G4Box("Source", fWorldSize/2, 2.5*cm, fWorldSize/2);

  G4LogicalVolume *logicSource = new G4LogicalVolume(solidSource, Cobalt, "Source");

  G4VPhysicalVolume *physiSource = new G4PVPlacement(0, G4ThreeVector(0, -1*fWorldSize/2+2.5*cm, 0), logicSource, "Source", logicWorld, false, 0, false);
*/

    //Shielding
  if(isShielded)
  {
	////////////////////////////////////////////////////////////////
	//////////UNCOMMENT THE LEAD SHIELDING TO INCLUDE THEM//////////
	////////////////////////////////////////////////////////////////
	
/*	//Shield 1 (Lead)
	G4double Di1 = 1; //Inches
	G4double Do1 = 5;
	G4double zL1 = 2;
	G4double DistOrigin1 = zDistanceFromOrigin/2.54 + (zL1/2) + zLength /2.54 + 1; // +1 will be changed
  	solidShield1 = new G4Tubs("solidShield1", Di1*2.54/2*cm, Do1*2.54/2*cm, zL1*2.54/2*cm, 0*deg, 360*deg);
	logicShield1 = new G4LogicalVolume(solidShield1, Pb, "logicShield1");
	physShield1 = new G4PVPlacement(0, G4ThreeVector(0, 0, DistOrigin1*2.54*cm), logicShield1, "physShield1", logicWorld, false, 0, false);
*/      
	//Shield 2 (Copper)
        G4double Di2 = 4; //Inches
        G4double Do2 = 4.125;
        G4double zL2 = zLength/2.54 + 0.5; //This will be changed
        G4double DistOrigin2 = zDistanceFromOrigin/2.54 + (zL2/2);
	solidShield2 = new G4Tubs("solidShield2", Di2*2.54/2*cm, Do2*2.54/2*cm, zL2*2.54/2*cm, 0*deg, 360*deg);
        logicShield2 = new G4LogicalVolume(solidShield2, Cu, "logicShield2");
        physShield2 = new G4PVPlacement(0, G4ThreeVector(0, 0, DistOrigin2*2.54*cm), logicShield2, "physShield2", logicWorld, false, 0, false);

/*	//Shield 3 (Lead)
        G4double Di3 = 5.125; //Inches This will be changed
 	G4double Do3 = outerShieldDiameter;
        G4double zL3 = zL1 + zLength/2.54 + 1; //This will be changed
        G4double DistOrigin3 = zDistanceFromOrigin/2.54+(zL3/2); //This will be changed
        solidShield3 = new G4Tubs("solidShield3", Di3*2.54/2*cm, Do3*2.54/2*cm, zL3*2.54/2*cm, 0*deg, 360*deg);
        logicShield3 = new G4LogicalVolume(solidShield3, Pb, "logicShield3");
        physShield3 = new G4PVPlacement(0, G4ThreeVector(0, 0, DistOrigin3*2.54*cm), logicShield3, "physShield3", logicWorld, false, 0, false);
*/  }


  return physiWorld;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
