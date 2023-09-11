#include "TFile.h"
#include "TTree.h"
#include "TChain.h"
#include "TH1F.h"
#include "TF1.h"
#include "TTreeReader.h"
#include "TTreeReaderValue.h"
#include "TTreeReaderArray.h"
#include "TH2D.h"
#include "TLorentzVector.h"
#include "TMath.h"
#include "TCanvas.h"
#include "TChain.h"
#include "TRandom3.h"
#include "TGenPhaseSpace.h"

#include <ctime> // time_t
#include <cstdio>
#include <iostream>
#include <fstream>
using namespace std;

int Generate_Single_Particle()
{
    time_t begin, intermediate, end; // time_t is a datatype to store time values.

    time(&begin); // note time before execution

    int number_to_gen = 1000000;
    int number_per_file = 10000;

    string outFolder = "Gen_folder_posi/";

    TRandom3 *rand_gen = new TRandom3();
    rand_gen->SetSeed(0); // should be the computer clock

    TLorentzVector Lepton, Hadron;

    ofstream Lund_file;
    Lund_file.open(outFolder + "Lund_BG_0.txt");

    int nbEvent = 0;
    int nbLundFile = 0;

    while (nbEvent < number_to_gen) // nentries /*&& nbEvent < 10*/)
    {
        nbEvent++;

        float phi_gen = rand_gen->Uniform(2.0 * TMath::Pi());
        float theta_lepton_gen = rand_gen->Uniform(5.0 * TMath::DegToRad(), 35.0 * TMath::DegToRad());
        float theta_hadron_gen = rand_gen->Uniform(5.0 * TMath::DegToRad(), 35.0 * TMath::DegToRad());
        float p_lepton_gen = rand_gen->Uniform(4.0, 10.6);
        float p_hadron_gen = rand_gen->Uniform(4.0, 10.6);

        Lepton.SetXYZM(p_lepton_gen * sin(theta_lepton_gen) * cos(phi_gen), p_lepton_gen * sin(theta_lepton_gen) * sin(phi_gen), p_lepton_gen * cos(theta_lepton_gen), 0.000511);
        Hadron.SetXYZM(p_hadron_gen * sin(theta_lepton_gen) * cos(phi_gen - TMath::Pi()), p_hadron_gen * sin(theta_lepton_gen) * sin(phi_gen - TMath::Pi()), p_hadron_gen * cos(theta_hadron_gen), 0.139);

        Lund_file << 2 << "   " << 1 << "   " << 1 << "   " << 0 << "    " << 0.85 << "   " << 11 << "     " << 10.6 << "   " << 2212 << "   " << 0 << "   " << 0 << "\n";
        Lund_file << 1 << "   " << 0 << "   " << 1 << "   " << -11 << "   " << 0 << "   " << 0 << "   " << Lepton.Px() << "   " << Lepton.Py() << "   " << Lepton.Pz() << "   " << 0 << "   " << 0 << "   " << 0 << "   " << 0 << "   " << 0 << "\n";
        Lund_file << 2 << "   " << 0 << "   " << 1 << "   " << 211 << "   " << 0 << "   " << 0 << "   " << Hadron.Px() << "   " << Hadron.Py() << "   " << Hadron.Pz() << "   " << 0 << "   " << 0 << "   " << 0 << "   " << 0 << "   " << 0 << "\n";

        if (nbEvent % number_per_file == 0)
        {
            nbLundFile++;
            Lund_file.close();
            // Lund_file.open(Form("%sLund_BG_%i.txt", outFolder, nbLundFile));
            Lund_file.open(outFolder + "Lund_BG_" + std::to_string(nbLundFile) + ".txt");
        }

        if (nbEvent % 10000 == 0)
        {
            time(&intermediate);
            double intermediate_time = difftime(intermediate, begin);

            cout << nbEvent << " events processed in " << intermediate_time << "s"
                 << "\n";
        }
    }

    Lund_file.close();

    cout<<"finished "<<"\n";

    return 0;
}
