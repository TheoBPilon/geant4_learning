void plot_gamma_interactions() {
  TFile file("../build/gammaInteractions.root", "READ");

  if (file.IsZombie()) {
    std::cerr << "Erro ao abrir arquivo\n";
    return;
  }

  TTree *tree = (TTree *)file.Get("interactions");
  if (!tree) {
    std::cerr << "TTree nao encontrado\n";
    return;
  }

  int processID;
  double energy;

  tree->SetBranchAddress("processID", &processID);
  tree->SetBranchAddress("energy", &energy);

  TH1D *hist_phot =
      new TH1D("phot", "Photoelectric;Energy (keV);Counts", 200, 0, 2000);
  TH1D *hist_compt =
      new TH1D("compt", "Compton;Energy (keV);Counts", 200, 0, 2000);
  TH1D *hist_conv =
      new TH1D("conv", "Pair production;Energy (keV);Counts", 200, 0, 2000);

  Long64_t nentries = tree->GetEntries();

  for (Long64_t i = 0; i < nentries; i++) {
    tree->GetEntry(i);

    if (processID == 0)
      hist_phot->Fill(energy);
    else if (processID == 1)
      hist_compt->Fill(energy);
    else if (processID == 2)
      hist_conv->Fill(energy);
  }

  TCanvas *canvas = new TCanvas("canvas", "Gamma processes", 900, 600);

  hist_phot->SetLineColor(kRed);
  hist_compt->SetLineColor(kBlue);
  hist_conv->SetLineColor(kGreen + 2);

  hist_phot->Draw();
  hist_compt->Draw("same");
  hist_conv->Draw("same");

  TLegend *legend = new TLegend(0.7, 0.7, 0.9, 0.9);
  legend->AddEntry(hist_phot, "Photoelectric", "l");
  legend->AddEntry(hist_compt, "Compton", "l");
  legend->AddEntry(hist_conv, "Pair production", "l");
  legend->Draw();

  canvas->SaveAs("../gammaInteractions.png");
}
