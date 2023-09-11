#include "TFile.h"
#include "TTree.h"
#include "TChain.h"
#include "TH1F.h"
#include "TF1.h"
#include "TTreeReader.h"
#include "TTreeReaderValue.h"
#include "TTreeReaderArray.h"
#include "TH2F.h"
#include "TLorentzVector.h"
#include "TMath.h"
#include "TCanvas.h"
#include "TH3F.h"
#include "THStack.h"
#include <iostream>
#include <fstream>
using namespace std;

int Variable_Plots()
{
	gROOT->SetBatch(kTRUE);

	gStyle->SetPaintTextFormat("4.1f");
	gStyle->SetPalette(kLightTemperature);
	gStyle->SetOptStat(0);
	gStyle->SetLabelSize(.04, "xyz");
	gStyle->SetTitleSize(.04, "xyz");
	gStyle->SetTitleSize(.07, "t");
	gStyle->SetFrameLineWidth(1);
	gStyle->SetLineWidth(1);
	gStyle->SetHistLineWidth(1);
	gStyle->SetMarkerStyle(13);
	gStyle->SetTitleW(0.8); // per cent of the pad width
	gStyle->SetTitleH(0.1); // per cent of the pad height



	

	TFile *Data_file = new TFile("../OSG_File/outFile_Pion_neg_inbending.root");
	TTree *Data_tree = (TTree *)Data_file->Get("tree");
	int entries_tree = Data_tree->GetEntries();
	TString label = "#pi^{+}";
	TCut cut_0 = "";


	TFile *Data_file_1 = new TFile("../OSG_File/outFile_Lepton_neg_inbending.root");
	TTree *Data_tree_1 = (TTree *)Data_file_1->Get("tree");
	int entries_tree_1 = Data_tree_1->GetEntries();
	TString label_1 = "e^{+}";
	TCut cut_1 = "";
	


	std::vector<vector<TString>> labels1D{

		
		{"P", "P", "custom_range", "4.", "12.",
		 "custom_binning", "30", "", "legend", "P"},

		{"Theta*180./3.141592", "#Theta", "custom_range", "0.", "40",
		 "custom_binning", "30", "", "legend", "electron_SF"},

		{"SFPCAL", "SF PCAL", "custom_range", "0.", "0.3",
		 "custom_binning", "30", "", "legend", "positron_SF_4"},

		{"SFECIN", "SF ECIN", "custom_range", "0.", "0.25",
		 "custom_binning", "30", "", "legend", "positron_SF_4"},

		 {"SFECOUT", "SF ECOUT", "custom_range", "0.", "0.25",
		 "custom_binning", "30", "", "legend", "positron_SF_4"},

		 {"m2PCAL", "m2 PCAL", "custom_range", "0.", "200",
		 "custom_binning", "30", "", "legend", "positron_SF_4"},

		{"m2ECIN", "m2 ECIN", "custom_range", "0.", "500",
		 "custom_binning", "30", "", "legend", "positron_SF_4"},

		 {"m2ECOUT", "m2 ECOUT", "custom_range", "0.", "500",
		 "custom_binning", "30", "", "legend", "positron_SF_4"}
	};

	for (int i = 0; i < labels1D.size(); i++)
	{

		//////////////////////////////////////////////////
		// Set options for each label
		TString label1 = labels1D[i][0];
		TString xAxis_label = labels1D[i][1];
		TString range_x_option = labels1D[i][2];
		TString min_x_option = labels1D[i][3];
		TString max_x_option = labels1D[i][4];
		TString binning_x_option = labels1D[i][5];
		TString nb_x_bins = labels1D[i][6];
		TString string_cut = labels1D[i][7];
		TString legend_option = labels1D[i][8];
		TString output_string = labels1D[i][9];
		//////////////////////////////////////////////////

		cout << endl;
		cout << "//////////////////////////////////////////////////" << endl;
		cout << "Doing 1D " << label1 << " plot" << endl;
		cout << "//////////////////////////////////////////////////" << endl;
		cout << endl;

		TCut cut = string_cut.Data(); 

		TString cut_string = cut.GetTitle();

		float min_X_histo_ini;
		float max_X_histo_ini;

		int nBins_X = 20;

		if (range_x_option == "custom_range")
		{
			cout << "here " << endl;
			min_X_histo_ini = stof((string)min_x_option.Data());
			max_X_histo_ini = stof((string)max_x_option.Data());
		}

		if (binning_x_option == "custom_binning")
		{
			cout << "here " << nb_x_bins.Data() << endl;
			nBins_X = stoi((string)nb_x_bins.Data());
		}

		TH1F *Data_hist = new TH1F("Data_hist", "Data_hist", nBins_X, min_X_histo_ini, max_X_histo_ini);
		Data_tree->Draw(label1 + ">>Data_hist", cut_0 * cut);
		// Data_tree->Draw("Q2>>Data_hist");

		Data_hist->SetLineWidth(2);
		Data_hist->SetLineColor(kBlack);
		Data_hist->SetMarkerColor(kBlack);
		Data_hist->SetMarkerSize(2);
		Data_hist->SetMarkerStyle(20);
		Data_hist->SetTitle(";" + xAxis_label+";counts");

		TH1F *Data_hist_1 = new TH1F("Data_hist_1", "Data_hist_1", nBins_X, min_X_histo_ini, max_X_histo_ini);
		TCut weight = Form("%i/%i", entries_tree, entries_tree_1);
		Data_tree_1->Draw(label1 + ">>Data_hist_1", weight * cut_1 * cut);

		Data_hist_1->SetLineWidth(2);
		Data_hist_1->SetLineColor(kRed);
		Data_hist_1->SetMarkerColor(kRed);
		Data_hist_1->SetMarkerSize(2);
		Data_hist_1->SetMarkerStyle(20);
		Data_hist_1->SetTitle(";" + xAxis_label+";counts");

		Data_hist->SetMaximum(std::max(Data_hist->GetMaximum(),Data_hist_1->GetMaximum()) * 1.2);

		auto legend = new TLegend(0.65, 0.87, 0.87, 0.67);
		legend->AddEntry(Data_hist, Form("%s ", label.Data()), "l");
		legend->AddEntry(Data_hist_1, Form("%s", label_1.Data()), "l");

		
		TCanvas *cancG0 = new TCanvas("", "can0", 1500, 1000);
		cancG0->cd();


		Data_hist->Draw("pe ");
		Data_hist_1->Draw("pe same");

		legend->SetFillStyle(0);
		legend->SetLineWidth(0);
		legend->Draw("same ");

		// Labels
		double x_top_label = 0.90;

		TPaveText *CLAS12_Internal = new TPaveText(0.2, x_top_label, 0.288191, x_top_label + 0.1, "NDC");
		CLAS12_Internal->SetFillStyle(4050);
		CLAS12_Internal->SetLineColor(0);
		CLAS12_Internal->SetTextFont(42);
		CLAS12_Internal->SetTextSize(0.0599401);
		CLAS12_Internal->SetBorderSize(0);
		CLAS12_Internal->AddText(Form("CLAS12 Lepton ID"));
		CLAS12_Internal->Draw();


		TString name_pdf = "LeptonID";
		if (i == 0)
			cancG0->SaveAs(name_pdf + ".pdf(");
		else if (i == (labels1D.size() - 1))
			cancG0->SaveAs(name_pdf + ".pdf)");
		else
			cancG0->SaveAs(name_pdf + ".pdf");

	}

	gApplication->Terminate();

	return 0;
}
