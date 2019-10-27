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

/// \file muonSDSimEventAction.hh
/// \brief Definition of the muonSDSimEventAction class

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#ifndef muonSDSimEventAction_h
#define muonSDSimEventAction_h 1

#include <iostream>
#include <vector>
#include "G4UserEventAction.hh"
#include "globals.hh"

class muonSDSimRunAction;
class G4Run;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

/// Event action class

class muonSDSimEventAction : public G4UserEventAction
{
  	public:
		muonSDSimEventAction();
		virtual ~muonSDSimEventAction();

		virtual void BeginOfEventAction(const G4Event* event);
		virtual void EndOfEventAction(const G4Event* event);

		G4double GetNeutrinoStepTime(){return NeutrinoStepTime;}
		void SetNeutrinoStepTime(G4double input){NeutrinoStepTime = input;}

		void AddEdep(G4double attribute[]) {
			if (attribute[0] != 0){
				fNumE.push_back(attribute[0]);}
			if (attribute[1] != 0){
				fDelTmu.push_back(attribute[1]);}
			if (attribute[2] != 0){
				fDelTe.push_back(attribute[2]);}
			fEvtIDCount = attribute[3];
			fStepCount ++;}
		
		const G4Run* run;

  	private:
		G4double 	 fEvtIDCount;
		G4double 	 NeutrinoStepTime;
		G4int 		 fStepCount;
		std::vector<double>  fDelTmu;
		std::vector<double>  fDelTe;
		std::vector<double> fNumE;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
