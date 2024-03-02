const double sqrt2 = 1.4142136;

double
emg(double *_x, double *par)
{
  double x = _x[0];
  double norm = par[0];
  double mean = par[1];
  double sigma = par[2];
  double lambda = par[3];
  double sigma2 = sigma * sigma;
  double lambda2 = 0.5 * lambda;
  double erfc = TMath::Erfc( (mean + lambda * sigma2 - x) / (sqrt2 * sigma) );
  return norm * lambda2 * TMath::Exp( lambda2 * (2. * mean + lambda * sigma2 - 2 * x) ) * erfc;
}

TF1 *
EMG(double norm, double mean, double sigma, double lambda, double min, double max)
{
  auto f = new TF1("fEMG", emg, min, max, 4);
  f->SetParameter(0, norm);
  f->SetParameter(1, mean);
  f->SetParameter(2, sigma);
  f->SetParameter(3, lambda);
  return f;
}
