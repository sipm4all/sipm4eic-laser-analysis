#pragma once

Double_t
qExp(Double_t x, Double_t q)
{
  if (q == 1.)
    return TMath::Exp(x);
  if ((1. + (1. - q) * x) > 0.)
    return TMath::Power(1. + (1. - q) * x, 1. / (1. - q));
  return TMath::Power(0., 1. / (1. - q));
}

Double_t
qGaus(Double_t *_x, Double_t *_par)
{
  Double_t norm = _par[0];
  Double_t mean = _par[1];
  Double_t sigma = _par[2];
  //
  Double_t x = _x[0] - mean;
  Double_t beta = 1. / (2. * sigma * sigma);
  Double_t q = _par[3];
  //
  return _par[4] + norm * qExp(-beta * x * x, q);
}

Double_t
qTOF(Double_t *_x, Double_t *_par)
{
  Double_t norm = _par[0];
  Double_t mean = _par[1];
  Double_t sigma = _par[2];
  //
  Double_t x = _x[0] - mean;
  Double_t beta = 1. / (2. * sigma * sigma);
  Double_t qR = _par[3];
  Double_t qL = _par[4];
  //
  if (x <= 0.) return norm * qExp(-beta * x * x, qL);
  return norm * qExp(-beta * x * x, qR);
}

static TF1 *fqIntegrand = NULL;

Double_t
qIntegrand(Double_t *_x, Double_t *_par)
{
  Double_t x = _x[0];
  Double_t x1 = x - _par[4];
  Double_t par[4] = {_par[0], _par[1], _par[2], _par[3]};
  return qTOF(&x, par) * qTOF(&x1, par);
}

Double_t
qConv(Double_t *_x, Double_t *_par)
{
  if (!fqIntegrand)
    fqIntegrand = new TF1("fqIntegrand", qIntegrand, -24400., 24400., 5);
  fqIntegrand->SetParameters(_par[0], _par[1], _par[2], _par[3], _x[0]);
  return _par[4] + fqIntegrand->Integral(-24400., 24400.);
}


TF1 *
qGaus(Float_t norm, Float_t mean, Float_t sigma, Float_t q)
{
  
  TF1 *f = new TF1("qGaus", qGaus, -1000., 1000., 5);
  f->SetParameter(0, norm);
  f->SetParameter(1, mean);
  f->SetParameter(2, sigma);
  f->SetParameter(3, q);
  return f;
}

TF1 *
qTOF(Float_t norm, Float_t mean, Float_t sigma, Float_t qR, Float_t qL = 1.)
{
  
  TF1 *f = new TF1("qTOF", qTOF, -1000., 1000., 5);
  f->SetParameter(0, norm);
  f->SetParameter(1, mean);
  f->SetParameter(2, sigma);
  f->SetParameter(3, qR);
  f->SetParameter(4, qL);
  
  f->SetParLimits(3, 0., 100.);
  f->SetParLimits(4, 0., 100.);
  return f;
}

