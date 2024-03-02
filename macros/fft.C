#include <TGraph.h>
#include <TCanvas.h>
#include <TMath.h>
#include <TVirtualFFT.h>

void FFT(TGraph *graph) {
    int nPoints = graph->GetN();

    double *xValues = graph->GetX();
    double *yValues = graph->GetY();

    // Create an array to hold the complex data for the FFT
    double *in = new double[nPoints];

    for (int i = 0; i < nPoints; ++i) {
      in[i] = yValues[i];
    }

    // Perform the FFT
    TVirtualFFT *fft = TVirtualFFT::FFT(1, &nPoints, "R2C");
    fft->SetPoints(in);
    fft->Transform();

    // Get the transformed data
    fft->GetPoints(in);

    // Create a new TGraph to store the magnitude of the FFT
    TGraph *fftMagnitude = new TGraph(nPoints);

    // Fill the TGraph with the magnitude of the FFT
    for (int i = 0; i < nPoints; ++i) {
        double freq = i / (xValues[nPoints - 1] - xValues[0]);
        double mag = TMath::Sqrt(in[2 * i] * in[2 * i] + in[2 * i + 1] * in[2 * i + 1]);
        fftMagnitude->SetPoint(i, freq, mag);
    }

    // Draw the original graph
    TCanvas *c1 = new TCanvas("c1", "Original Graph", 800, 600);
    graph->Draw();

    // Draw the FFT magnitude graph
    TCanvas *c2 = new TCanvas("c2", "FFT Magnitude", 800, 600);
    fftMagnitude->SetTitle("FFT Magnitude");
    fftMagnitude->Draw("AL");

    // Clean up
    delete fft;
    delete[] in;
}
