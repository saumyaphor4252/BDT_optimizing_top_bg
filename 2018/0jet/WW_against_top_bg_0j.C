// To compile 
// root -l
// gSystem->Load("libLatinoAnalysisMultiDraw.so")
// .L hww_VBF_MYmvaBDTG.C+ 

#include "TMVA/Reader.h"
#include "TLorentzVector.h"
#include "TTree.h"
#include "TSystem.h"
#include "TROOT.h"
#include "TString.h"
#include "TFile.h"
#include "TChain.h"

using namespace std;

namespace multidraw {
  extern thread_local TTree* currentTree;
}

TMVA::Reader* myreaderBDT= new TMVA::Reader();

bool initialized = false;

TString name_temp = "";


// Variables fed into the BDT
float loc_pt1;
float loc_pt2;
float loc_mll;
float loc_ptll;
float loc_drll;
float loc_dphill;
float loc_dphilmet;
float loc_dphillmet;
float loc_mtw1;
float loc_mtw2;
float loc_pTWW;
float loc_Jet_btagDeepB_CleanJet_jetIdx_0;

int loc_CleanJet_jetIdx_0;

// Variables from the tree
float loc0_pt1;
float loc0_pt2;
float loc0_mll;
float loc0_ptll;
float loc0_drll;
float loc0_dphill;
float loc0_dphilmet;
float loc0_dphillmet;
float loc0_mtw1;
float loc0_mtw2;
float loc0_pTWW;
float loc0_Jet_btagDeepB[100];

int loc0_CleanJet_jetIdx[100];

float bVeto(float* Jet_btagDeepB, int CleanJet_jetIdx)
{
    if (CleanJet_jetIdx >=0)
        return Jet_btagDeepB[CleanJet_jetIdx];
    else
        return -2;
}

void initmyreaderBDT(TTree* tree)
{
    tree->SetBranchAddress("pt1", &loc0_pt1);
    tree->SetBranchAddress("pt2", &loc0_pt2);	
    tree->SetBranchAddress("mll", &loc0_mll);
    tree->SetBranchAddress("ptll", &loc0_ptll);
    tree->SetBranchAddress("drll", &loc0_drll);
    tree->SetBranchAddress("dphill", &loc0_dphill);
    tree->SetBranchAddress("dphilmet", &loc0_dphilmet);
    tree->SetBranchAddress("dphillmet", &loc0_dphillmet);
    tree->SetBranchAddress("mtw1", &loc0_mtw1);
    tree->SetBranchAddress("mtw2", &loc0_mtw2);
    tree->SetBranchAddress("pTWW", &loc0_pTWW);
    tree->SetBranchAddress("Jet_btagDeepB", &loc0_Jet_btagDeepB);
    tree->SetBranchAddress("CleanJet_jetIdx", &loc0_CleanJet_jetIdx);

    // The variables' names and their order have to agree with those defined in TMVAClassification.C file
    myreaderBDT->AddVariable("pt1", &loc_pt1);
    myreaderBDT->AddVariable("pt2", &loc_pt2);
    myreaderBDT->AddVariable("mll", &loc_mll);
    myreaderBDT->AddVariable("ptll", &loc_ptll);
    myreaderBDT->AddVariable("drll", &loc_drll);
    myreaderBDT->AddVariable("dphill", &loc_dphill);
    myreaderBDT->AddVariable("dphilmet", &loc_dphilmet);
    myreaderBDT->AddVariable("dphillmet", &loc_dphillmet);
    myreaderBDT->AddVariable("mtw1", &loc_mtw1);	
    myreaderBDT->AddVariable("mtw2", &loc_mtw2);	
    myreaderBDT->AddVariable("pTWW", &loc_pTWW);
    myreaderBDT->AddVariable("Alt$(Jet_btagDeepB[CleanJet_jetIdx[0]],-2)", &loc_Jet_btagDeepB_CleanJet_jetIdx_0);	   
    //change the path of weight file, xml file is in " BDT_config/weight " folder .
    myreaderBDT->BookMVA("BDT","/afs/cern.ch/user/s/ssaumya/Projects/WW_Analysis_Work/WW_Cuts_Optimization/CMSSW_11_1_3/src/PlotsConfigurations/Configurations/WW/FullRunII/Full2018_v7/BDT_Training_weights_v2/0jet/TMVAClassification_BDTG4D3.weights.xml"); 

}


float WW_against_top_bg_0j(int entry, int nclass)
{
    if(name_temp != multidraw::currentTree->GetCurrentFile()->GetName())
	{
		std::cout << "name_temp = " << name_temp << endl;
		name_temp = multidraw::currentTree->GetCurrentFile()->GetName();
		std::cout << "name_temp = " << name_temp << endl;
		initialized = false;
	}

    if (!initialized)
	{
		//latino = (TTree*)gDirectory->Get("latino");
		delete myreaderBDT;
		myreaderBDT = new TMVA::Reader();
		initmyreaderBDT(multidraw::currentTree);
		cout << "check init" << endl;	
		initialized = true;		
    }

    multidraw::currentTree->GetEntry(entry);

    loc_pt1 = loc0_pt1;
    loc_pt2 = loc0_pt2;
    loc_mll = loc0_mll;
    loc_ptll = loc0_ptll;
    loc_drll = loc0_drll;
    loc_dphill = loc0_dphill;
    loc_dphilmet = loc0_dphilmet;
    loc_dphillmet = loc0_dphillmet;
    loc_mtw1 = loc0_mtw1;
    loc_mtw2 = loc0_mtw2;
    loc_pTWW = loc0_pTWW;
    loc_Jet_btagDeepB_CleanJet_jetIdx_0 = bVeto(loc0_Jet_btagDeepB, loc0_CleanJet_jetIdx[0]);
  
    float classifier = myreaderBDT->EvaluateMVA("BDT");

    return classifier;

}
