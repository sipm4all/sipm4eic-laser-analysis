#pragma once

namespace waveutils {

float get_amplitude(TGraph &graph, float tmin, float tmax)
{
  float amp = 0.;
  for (int i = 0; i < graph.GetN(); ++i) {
    if (graph.GetX()[i] > tmin && graph.GetX()[i] < tmax && graph.GetY()[i] > amp)
      amp = graph.GetY()[i];
  }
  return amp;
}
  
std::vector<float> get_transitions(TGraph &graph, float threshold, float sign)
{
  std::vector<float> values;
  bool armed = false;
  for (int i = 0; i < graph.GetN(); ++i) {
    if (!armed && sign * graph.GetY()[i] > sign * threshold * 0.5)
      continue;
    armed = true;
    if (sign * graph.GetY()[i] < sign * threshold)
      continue;
    values.push_back(graph.GetX()[i]);
    armed = false;
  }
  return values;
 }
 
} /** namespace waveutils **/
