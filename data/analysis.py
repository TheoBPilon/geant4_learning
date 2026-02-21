import ROOT
import numpy as np
from array import array


file = ROOT.TFile("build/gammaInteractions.root")
tree = file.Get("interactions")
print("Numero de Entradas", tree.GetEntries())
#importar macro de estilo
ROOT.gROOT.Macro("/home/theobp/tutorial_laura/Style_bold_italic.C")
# bins log
nbins = 100
emin = 1
emax = 100000

bins = np.logspace(np.log10(emin), np.log10(emax), nbins+1)
bins_array = array('d', bins) # agora cria histogramas
h_phot  = ROOT.TH1D("h_phot",  "Photoelectric", nbins, bins_array)
h_compt = ROOT.TH1D("h_compt", "Compton",       nbins, bins_array)
h_pair  = ROOT.TH1D("h_pair",  "Pair production", nbins, bins_array)
h_total = ROOT.TH1D("h_total", "Total",         nbins, bins_array)
# preencher o histograma correto dependendo do ID da interação
for entry in tree:
    h_total.Fill(entry.energy)
    if entry.processID == 0:
        h_phot.Fill(entry.energy)
    elif entry.processID == 1:
        h_compt.Fill(entry.energy)
    elif entry.processID == 2:
        h_pair.Fill(entry.energy)

# Normaliza pela soma de todas as interacoes
h_phot.Divide(h_phot, h_total, 1.0, 1.0, "B")
h_compt.Divide(h_compt, h_total, 1.0, 1.0, "B")
h_pair.Divide(h_pair, h_total, 1.0, 1.0, "B")

# Canvas
canvas = ROOT.TCanvas("canvas", "Gamma interactions", 900, 600)
h_phot.SetLineColor(ROOT.kRed)
h_compt.SetLineColor(ROOT.kBlue)
h_pair.SetLineColor(ROOT.kGreen+2)
h_phot.Draw("HIST")
h_compt.Draw("HIST SAME")
h_pair.Draw("HIST SAME")

legend = ROOT.TLegend(0.7,0.7,0.9,0.9)
legend.AddEntry(h_phot,  "Photoelectric", "l")
legend.AddEntry(h_compt, "Compton", "l")
legend.AddEntry(h_pair,  "Pair production", "l")
# legend.SetTextFont(33);
legend.Draw()

h_phot.SetTitle("Gamma interaction probabilities;Energy (keV);Normalized Counts")
# h_phot.GetXaxis().SetMoreLogLabels()
# h_phot.GetXaxis().SetNoExponent() 
# canvas.SetLogy()
canvas.SetLogx()
# canvas.SaveAs("gammaInteractions.png")
canvas.Draw()
input("Press Enter to continue...")
