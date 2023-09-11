

#include <cstdlib>
#include <iostream>
#include <map>
#include <string>

#include "TChain.h"
#include "TFile.h"
#include "TTree.h"
#include "TString.h"
#include "TObjString.h"
#include "TSystem.h"
#include "TROOT.h"

#include "TMVA/MethodCategory.h"
#include "TMVA/Factory.h"
#include "TMVA/DataLoader.h"
#include "TMVA/Tools.h"
#include "TMVA/TMVAGui.h"

int TMVAClassification( TString myMethodList = "" )
{

   gStyle->SetPalette(55);	
   TMVA::Tools::Instance();


   std::cout << std::endl;
   std::cout << "==> Start TMVAClassification" << std::endl;

   
   TFile *input1 = new TFile("../OSG_File/outFile_Lepton_posi_inbending.root");
   TFile *input2 = new TFile("../OSG_File/outFile_Pion_posi_inbending.root");
  
   TTree *signalTree     = (TTree*)input1->Get("tree");
   TTree *background     = (TTree*)input2->Get("tree");

   // Create a ROOT output file where TMVA will store ntuples, histograms, etc.
   TString outfileName( "TMVA.root" );
   TFile* outputFile = TFile::Open( outfileName, "RECREATE" );

   
   TMVA::Factory *factory = new TMVA::Factory( "TMVAClassification", outputFile,
                                               "!V:!Silent:Color:DrawProgressBar:Transformations=I;D;P;G,D:AnalysisType=Classification" );

   TMVA::DataLoader *dataloader=new TMVA::DataLoader("dataset");
   

   dataloader->AddVariable( "P","P","", 'F' );
   dataloader->AddVariable( "Theta","Theta","", 'F' );
   dataloader->AddVariable( "Phi","Phi","", 'F' );
   dataloader->AddVariable( "SFPCAL","PCAL","", 'F' );
   dataloader->AddVariable( "SFECIN","ECIN", "", 'F' );
   dataloader->AddVariable( "SFECOUT","ECOUT", "", 'F' );
   dataloader->AddVariable( "m2PCAL","m2PCAL","", 'F' );
   dataloader->AddVariable( "m2ECIN","m2ECIN", "", 'F' );
   dataloader->AddVariable( "m2ECOUT","m2ECOUT", "", 'F' );
  

   // global event weights per tree (see below for setting event-wise weights)
   Double_t signalWeight     = 1.0;
   Double_t backgroundWeight = 1.0;

   // You can add an arbitrary number of signal or background trees
   dataloader->AddSignalTree    ( signalTree,     signalWeight );
   dataloader->AddBackgroundTree( background, backgroundWeight );

   
   
   dataloader->PrepareTrainingAndTestTree( "", "",
   					"SplitMode=Random:NormMode=NumEvents:!V" );
   


   //"MLP"
   factory->BookMethod( dataloader, TMVA::Types::kMLP, "MLP", "H:!V:NeuronType=tanh:VarTransform=N:NCycles=600:HiddenLayers=N+5:TestRate=5:!UseRegulator" );

   
   //"BDT"  // Adaptive Boost
   factory->BookMethod( dataloader, TMVA::Types::kBDT, "BDT",
                           "!H:!V:NTrees=850:MinNodeSize=2.5%:MaxDepth=3:BoostType=AdaBoost:AdaBoostBeta=0.5:UseBaggedBoost:BaggedSampleFraction=0.5:SeparationType=GiniIndex:nCuts=20" );

  
   factory->TrainAllMethods();
   factory->TestAllMethods();
   factory->EvaluateAllMethods();

   outputFile->Close();

   std::cout << "==> Wrote root file: " << outputFile->GetName() << std::endl;
   std::cout << "==> TMVAClassification is done!" << std::endl;

   delete factory;
   delete dataloader;
   // Launch the GUI for the root macros
   if (!gROOT->IsBatch()) TMVA::TMVAGui( outfileName );

   return 0;
}

int main( int argc, char** argv )
{
   // Select methods (don't look at this code - not of interest)
   TString methodList;
   for (int i=1; i<argc; i++) {
      TString regMethod(argv[i]);
      if(regMethod=="-b" || regMethod=="--batch") continue;
      if (!methodList.IsNull()) methodList += TString(",");
      methodList += regMethod;
   }
   return TMVAClassification(methodList);
}
