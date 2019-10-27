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

/// \file muonSDSimEventAction.cc
/// \brief Implementation of the muonSDSimEventAction class

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "muonSDSimEventAction.hh"
#include "muonSDSimRunAction.hh"
#include "muonSDSimAnalysis.hh"

#include "G4Event.hh"
#include "G4THitsMap.hh"
#include "G4SDManager.hh"
#include "G4RunManager.hh"
#include "G4HCofThisEvent.hh"
#include "G4SystemOfUnits.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

muonSDSimEventAction::muonSDSimEventAction()
: G4UserEventAction()
{} 

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

muonSDSimEventAction::~muonSDSimEventAction()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void muonSDSimEventAction::BeginOfEventAction(const G4Event*)
{    
    fNumE.clear();
    fStepCount = 0;
    fEvtIDCount = 0.;
    fDelTmu.clear();
    fDelTe.clear();
    NeutrinoStepTime = 0;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void muonSDSimEventAction::EndOfEventAction(const G4Event*)
{   
    auto analysisManager = G4AnalysisManager::Instance();

    if (fDelTmu.size() > 0){
        
        int printverbose = 0;
        if (printverbose != 0){
            G4cout << " " << G4endl;
            G4cout << "|--------------------------------------------------------------------------------------|" << G4endl;
            G4cout << "|-------------------------------- Event (" << fEvtIDCount <<  ") Statistics --------------------------------|" << G4endl;
            G4cout << "|----------------- Steps Containing Secondary Muon Neutrino Production ----------------|" << G4endl;
            for (double n : fDelTmu){G4cout << "Time mu: " << n /microsecond << G4endl;}
            for (double n : fDelTe){G4cout << "Time e: " << n /microsecond<< G4endl;}
            for (double n : fNumE){G4cout << "Number of e-: " << n << G4endl;}
            G4cout << "# Steps of secondary muon neutrino or #electrons with parent ID = 1: " << fStepCount << G4endl;
        }

        G4double timeMU = 0.;
        for (double n : fDelTmu){ timeMU = n; }

        G4double minVal = fDelTe[0];
        int vectorSize = fDelTe.size();

        for (int i =0; i < vectorSize; i++){
            if (fDelTe[i] < minVal){minVal = fDelTe[i];}
        }

        G4double deltaT = minVal - timeMU;
        if (deltaT > 0.1){analysisManager->FillH1(0,deltaT);}
    }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
