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

// \file muonSDSimDetectorConstruction.cc
// \brief Implementation of the muonSDSimDetectorConstruction class

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "muonSDSimDetectorConstruction.hh"

#include "G4RunManager.hh"
#include "G4NistManager.hh"
#include "G4PVPlacement.hh"
#include "G4LogicalVolume.hh"
#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"
#include "G4VSensitiveDetector.hh"
#include "G4MultiFunctionalDetector.hh"
#include "G4VPrimitiveScorer.hh"
#include "G4PSEnergyDeposit.hh"
#include "G4RotationMatrix.hh"
#include "G4FieldManager.hh"
#include "G4UserLimits.hh"
#include "G4SDManager.hh"
#include "G4Cons.hh"
#include "G4Tubs.hh"
#include "G4Box.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

muonSDSimDetectorConstruction::muonSDSimDetectorConstruction()
: G4VUserDetectorConstruction(),
    fScoringVolume(0)
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

muonSDSimDetectorConstruction::~muonSDSimDetectorConstruction()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume* muonSDSimDetectorConstruction::Construct()
{  
    // Get nist material manager
    G4NistManager* nist = G4NistManager::Instance();

    // Option to switch on/off checking of volumes overlaps
    G4bool checkOverlaps = true;

    // Bool value (dunno what it means)
    G4bool boOperation = false;


// World
    G4double world_sizeX = 100*cm;
    G4double world_sizeY = 100*cm;
    G4double world_sizeZ  = 150*cm;
    G4Material* world_mat = nist->FindOrBuildMaterial("G4_AIR");

    G4Box* solidWorld = 
        new G4Box("World",                               //its name
            0.5*world_sizeX,                            //its size x
            0.5*world_sizeY,                            //its size y
            0.5*world_sizeZ);                            //its size z
        
    G4LogicalVolume* logicWorld =                         
        new G4LogicalVolume(solidWorld,                  //its solid
            world_mat,                                   //its material
            "World");                                    //its name
                                    
    G4VPhysicalVolume* physWorld =
        new G4PVPlacement(0,                             //no rotation
            G4ThreeVector(),                             //at (0,0,0)
            logicWorld,                                  //its logical volume
            "World",                                     //its name
            0,                                           //its mother  volume
            boOperation,                                 //no boolean operation
            0,                                           //copy number
            checkOverlaps);                              //overlaps checking


// Scintillator Block
    G4double scint_sizeXY = 10*cm;
    G4double scint_sizeZ = 100*cm;
    G4Material* scint_mat = nist->FindOrBuildMaterial("G4_PLASTIC_SC_VINYLTOLUENE");

    G4Box* solidScint = 
        new G4Box("Solid_Scintillator",                  //its name
            0.5*scint_sizeXY,                            //its size x
            0.5*scint_sizeXY,                            //its size y
            0.5*scint_sizeZ);                            //its size z

    G4LogicalVolume* logicScint =                         
        new G4LogicalVolume(solidScint,                  //its solid
            scint_mat,                                   //its material
            "Logic_Scintillator",                        //its name
            0,                                           //G4FieldManager
            0,                                           //G4VSensitiveDetector
            0,                                           //G4UserLimits
            true);                                       //Optimise

    G4VPhysicalVolume* physScint =
        new G4PVPlacement(0,                             //no rotation
            G4ThreeVector(0.*cm,0.*cm,0.*cm),            //at (0,0,0)
            logicScint,                                  //its logical volume
            "Phys_Scintillator",                         //its name
            logicWorld,                                  //its mother  volume
            boOperation,                                 //no boolean operation
            0,                                           //copy number
            checkOverlaps);                              //overlaps checking



// Light Guide Dimensions
    G4double LGRmin1 = 0.*cm;
    G4double LGRmax1 = 3.*cm;
    G4double LGRmin2 = 0.*cm;
    G4double LGRmax2 = 10.*cm;
    G4double LGDz = 10.*cm;
    G4double LGSPhi = 0.*cm;
    G4double LGDPhi = 360.*degree;


// Light Guide Rotation
    G4RotationMatrix* xRot = new G4RotationMatrix; 
    xRot->rotateX(pi *rad);


// Light Guide Solid and Logic Volumes
    G4Cons* LGSolid = 
        new G4Cons("Light_Guide_Solid",                //its name
            0.5*LGRmin1,     0.5*LGRmax1,                //bottom inner/outer radius     
            0.5*LGRmin2,     0.5*LGRmax2,                //top inner/outer r
            0.5*LGDz,        LGSPhi,                     //Change in height & start angle
            LGDPhi);                                     //Change in angle

    G4LogicalVolume* LGlogic =                         
        new G4LogicalVolume(LGSolid,                    //its solid
            scint_mat,                                   //its material
            "Light_Guide_Logic",                       //its name
            0,                                           //G4FieldManager
            0,                                           //G4VSensitiveDetector
            0,                                           //G4UserLimits
            true);                                       //Optimise


// Scintillator Light Guides Physical Volumes
    G4VPhysicalVolume* LG1Phys =
        new G4PVPlacement(0,                             //no rotation
            G4ThreeVector(0.*cm,0.*cm,-55.*cm),          //at (0,0,0)
            LGlogic,                                    //its logical volume
            "Light_Guide_Phys_1",                        //its name
            logicWorld,                                  //its mother  volume
            boOperation,                                 //no boolean operation
            0,                                           //copy number
            checkOverlaps);                              //overlaps checking

    G4VPhysicalVolume* LG2Phys =
        new G4PVPlacement(xRot,                          //no rotation
            G4ThreeVector(0.*cm,0.*cm,55.*cm),           //at (0,0,0)
            LGlogic,                                    //its logical volume
            "Light_Guide_Phys_2",                        //its name
            logicWorld,                                  //its mother  volume
            boOperation,                                 //no boolean operation
            0,                                           //copy number
            checkOverlaps);                              //overlaps checking



// PMT Face Dimensions, Solid and Logical Volumes
    G4double PMTRmin = 0.*cm;
    G4double PMTRmax = 3.*cm;
    G4double PMTDz = 1.*cm;
    G4double PMTSPhi = 0.*cm;
    G4double PMTDPhi = 360.*degree;

    G4Tubs* PMTSolid = 
        new G4Tubs("PMT_Solid",                        //its name
            0.5*PMTRmin,     0.5*PMTRmax,                //bottom inner/outer radius     
            0.5*PMTDz,        PMTSPhi,                   //Change in height & start angle
            PMTDPhi);                                    //Change in angle

    G4LogicalVolume* PMTlogic =                         
        new G4LogicalVolume(PMTSolid,                   //its solid
            scint_mat,                                   //its material
            "PMT_Logic",                               //its name
            0,                                           //G4FieldManager
            0,                                           //G4VSensitiveDetector
            0,                                           //G4UserLimits
            true);                                       //Optimise

// Scintillator PMT Physical Volumes
    G4VPhysicalVolume* PMT1Phys =
        new G4PVPlacement(0,                             //no rotation
            G4ThreeVector(0.*cm,0.*cm,-60.5*cm),         //at (0,0,0)
            PMTlogic,                                   //its logical volume
            "PMT_Phys_1",                                //its name
            logicWorld,                                  //its mother  volume
            boOperation,                                 //no boolean operation
            0,                                           //copy number
            checkOverlaps);                              //overlaps checking

    G4VPhysicalVolume* PMT2Phys =
        new G4PVPlacement(xRot,                          //no rotation
            G4ThreeVector(0.*cm,0.*cm,60.5*cm),          //at (0,0,0)
            PMTlogic,                                   //its logical volume
            "PMT_Phys_2",                                //its name
            logicWorld,                                  //its mother  volume
            boOperation,                                 //no boolean operation
            0,                                           //copy number
            checkOverlaps);                              //overlaps checking



// Set Scoring Volumes
    fScoringVolume = logicScint;

// Always return the physical World
    return physWorld;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
