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

/// \file muonSDSimSteppingAction.cc
/// \brief Implementation of the muonSDSimSteppingAction class

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "muonSDSimSteppingAction.hh"
#include "muonSDSimEventAction.hh"
#include "muonSDSimDetectorConstruction.hh"

#include "G4Step.hh"
#include "G4Event.hh"
#include "G4RunManager.hh"
#include "G4LogicalVolume.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4SystemOfUnits.hh"
#include "muonSDSimAnalysis.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

muonSDSimSteppingAction::muonSDSimSteppingAction(muonSDSimEventAction* eventAction)
: G4UserSteppingAction(),
    fEventAction(eventAction),
    fScoringVolume(0)
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

muonSDSimSteppingAction::~muonSDSimSteppingAction()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void muonSDSimSteppingAction::UserSteppingAction(const G4Step* step)
{
    if (!fScoringVolume)
    { 
        const muonSDSimDetectorConstruction* detectorConstruction
            = static_cast<const muonSDSimDetectorConstruction*>
                (G4RunManager::GetRunManager()->GetUserDetectorConstruction());
        fScoringVolume = detectorConstruction->GetScoringVolume();
    }
    
    // get volume of the current step
    G4LogicalVolume* logvolume 
        = step->GetPreStepPoint()->GetTouchableHandle()
            ->GetVolume()->GetLogicalVolume();

    const G4Event* evt = G4RunManager::GetRunManager()->GetCurrentEvent();
    G4double eID = evt->GetEventID();
    
    G4String particleName;
    G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
    G4ParticleDefinition* particlemu = particleTable->FindParticle(particleName="mu-");
    G4ParticleDefinition* particlee = particleTable->FindParticle(particleName="e-");
    G4ParticleDefinition* particlenu = particleTable->FindParticle(particleName="nu_mu");
  
    // check if we are in scoring volume
    if (logvolume == fScoringVolume)
    {
        const G4Track* track = step->GetTrack();

        if (track->GetParticleDefinition() == particlee){
            G4double timestepe = track->GetGlobalTime();
            G4double NeutrinoTime = fEventAction->GetNeutrinoStepTime();

            if (timestepe > NeutrinoTime){
                G4int ParentIdnum = track->GetParentID();
                if (ParentIdnum == 1){
                    if ((timestepe - NeutrinoTime) > 0.001){
                    G4double Listthing[] = {0.,0.,timestepe /ns,eID};
                    fEventAction->AddEdep(Listthing);
                    }
                }
            }
        }

        if (track->GetParticleDefinition() == particlemu){

            G4double timestepmu = track->GetGlobalTime();
            G4double numberE = 0.;

            const std::vector<const G4Track*>* secondary =
                step->GetSecondaryInCurrentStep(); 

            for (size_t lp=0; lp<(*secondary).size(); lp++) {

                G4ParticleDefinition* particle = (*secondary)[lp]->GetDefinition();

                if (particle == particlee) {numberE ++;}

                if (particle == particlenu){
                    G4double Listthing[] = {numberE,timestepmu /ns,0.,eID};
                    fEventAction->AddEdep(Listthing);
                    fEventAction->SetNeutrinoStepTime(timestepmu);
                }
            }
        }
    }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
