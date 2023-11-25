#pragma once
#include "waveutils.h"

struct __attribute((packed)) WAVEDESC_t {
  char INITIAL_CRAP[21];
  char DESCRIPTOR_NAME[16];
  char TEMPLATE_NAME[16];
  short COMM_TYPE;
  short COMM_ORDER;
  int WAVE_DESCRIPTOR;
  int USER_TEXT;
  int RES_DESC1;
  int TRIGTIME_ARRAY;
  int RIS_TIME_ARRAY;
  int RES_ARRAY1;
  int WAVE_ARRAY_1;
  int WAVE_ARRAY_2;
  int RES_ARRAY2;
  int RES_ARRAY3;
  char INSTRUMENT_NAME[16];
  int INSTRUMENT_NUMBER;
  char TRACE_LABEL[16];
  short RESERVED1;
  short RESERVED2;
  int WAVE_ARRAY_COUNT;
  int PNTS_PER_SCREEN;
  int FIRST_VALID_PNT;
  int LAST_VALID_PNT;
  int FIRST_POINT;
  int SPARSING_FACTOR;
  int SEGMENT_INDEX;
  int SUBARRAY_COUNT;
  int SWEEPS_PER_ACQ;
  short POINTS_PER_PAIR;
  short PAIR_OFFSET;
  float VERTICAL_GAIN;
  float VERTICAL_OFFSET;
  float MAX_VALUE;
  float MIN_VALUE;
  short NOMINAL_BITS;
  short NOM_SUBARRAY_COUNT;
  float HORIZ_INTERVAL;
  double HORIZ_OFFSET;
  double PIXEL_OFFSET;
  char VERTUNIT[48];
  char HORIZUNIT[48];
  float HORIZ_UNCERTAINTY;
  double TRIGGER_TIME_seconds;
  char TRIGGER_TIME_minutes;
  char TRIGGER_TIME_hours;
  char TRIGGER_TIME_days;
  char TRIGGER_TIME_months;
  short TRIGGER_TIME_year;
  short TRIGGER_TIME_unused;
  float ACQ_DURATION;
  short RECORD_TYPE;
  short PROCESSING_DONE;
  short RESERVED5;
  short RIS_SWEEPS;
  short TIMEBASE;
  short VERT_COUPLING;
  float PROBE_ATT;
  short FIXED_VERT_GAIN;
  short BANDWIDTH_LIMIT;
  float VERTICAL_VERNIER;
  float ACQ_VERT_OFFSET;
  short WAVE_SOURCE;
  char TERMINATOR;
};

void
WAVEDESC_dump(WAVEDESC_t WAVEDESC)
{
  std::cout << "DECRIPTOR_NAME: " << WAVEDESC.DESCRIPTOR_NAME << std::endl;
  std::cout << "TEMPLATE_NAME: " << WAVEDESC.TEMPLATE_NAME << std::endl;
  std::cout << "COMM_TYPE: " << WAVEDESC.COMM_TYPE << std::endl;
  std::cout << "COMM_ORDER: " << WAVEDESC.COMM_ORDER << std::endl;
  std::cout << "WAVE_DESCRIPTOR: " << WAVEDESC.WAVE_DESCRIPTOR << std::endl;
  std::cout << "USER_TEXT: " << WAVEDESC.USER_TEXT << std::endl;
  std::cout << "RES_DESC1: " << WAVEDESC.RES_DESC1 << std::endl;
  std::cout << "TRIGTIME_ARRAY: " << WAVEDESC.TRIGTIME_ARRAY << std::endl;
  std::cout << "RIS_TIME_ARRAY: " << WAVEDESC.RIS_TIME_ARRAY << std::endl;
  std::cout << "RES_ARRAY1: " << WAVEDESC.RES_ARRAY1 << std::endl;
  std::cout << "WAVE_ARRAY_1: " << WAVEDESC.WAVE_ARRAY_1 << std::endl;
  std::cout << "WAVE_ARRAY_2: " << WAVEDESC.WAVE_ARRAY_2 << std::endl;
  std::cout << "RES_ARRAY2: " << WAVEDESC.RES_ARRAY2 << std::endl;
  std::cout << "RES_ARRAY3: " << WAVEDESC.RES_ARRAY3 << std::endl;
  std::cout << "INSTRUMENT_NAME: " << WAVEDESC.INSTRUMENT_NAME << std::endl;
  std::cout << "INSTRUMENT_NUMBER: " << WAVEDESC.INSTRUMENT_NUMBER << std::endl;
  std::cout << "TRACE_LABEL: " << WAVEDESC.TRACE_LABEL << std::endl;
  std::cout << "RESERVED1: " << WAVEDESC.RESERVED1 << std::endl;
  std::cout << "RESERVED2: " << WAVEDESC.RESERVED2 << std::endl;
  std::cout << "WAVE_ARRAY_COUNT: " << WAVEDESC.WAVE_ARRAY_COUNT << std::endl;
  std::cout << "PNTS_PER_SCREEN: " << WAVEDESC.PNTS_PER_SCREEN << std::endl;
  std::cout << "FIRST_VALID_PNT: " << WAVEDESC.FIRST_VALID_PNT << std::endl;
  std::cout << "LAST_VALID_PNT: " << WAVEDESC.LAST_VALID_PNT << std::endl;
  std::cout << "FIRST_POINT: " << WAVEDESC.FIRST_POINT << std::endl;
  std::cout << "SPARSING_FACTOR: " << WAVEDESC.SPARSING_FACTOR << std::endl;
  std::cout << "SEGMENT_INDEX: " << WAVEDESC.SEGMENT_INDEX << std::endl;
  std::cout << "SUBARRAY_COUNT: " << WAVEDESC.SUBARRAY_COUNT << std::endl;
  std::cout << "SWEEPS_PER_ACQ: " << WAVEDESC.SWEEPS_PER_ACQ << std::endl;
  std::cout << "POINTS_PER_PAIR: " << WAVEDESC.POINTS_PER_PAIR << std::endl;
  std::cout << "PAIR_OFFSET: " << WAVEDESC.PAIR_OFFSET << std::endl;
  std::cout << "VERTICAL_GAIN: " << WAVEDESC.VERTICAL_GAIN << std::endl;
  std::cout << "VERTICAL_OFFSET: " << WAVEDESC.VERTICAL_OFFSET << std::endl;
  std::cout << "MAX_VALUE: " << WAVEDESC.MAX_VALUE << std::endl;
  std::cout << "MIN_VALUE: " << WAVEDESC.MIN_VALUE << std::endl;
  std::cout << "NOMINAL_BITS: " << WAVEDESC.NOMINAL_BITS << std::endl;
  std::cout << "NOM_SUBARRAY_COUNT: " << WAVEDESC.NOM_SUBARRAY_COUNT << std::endl;
  std::cout << "HORIZ_INTERVAL: " << WAVEDESC.HORIZ_INTERVAL << std::endl;
  std::cout << "HORIZ_OFFSET: " << WAVEDESC.HORIZ_OFFSET << std::endl;
  std::cout << "PIXEL_OFFSET: " << WAVEDESC.PIXEL_OFFSET << std::endl;
  std::cout << "VERTUNIT: " << WAVEDESC.VERTUNIT << std::endl;
  std::cout << "HORIZUNIT: " << WAVEDESC.HORIZUNIT << std::endl;
  std::cout << "HORIZ_UNCERTAINTY: " << WAVEDESC.HORIZ_UNCERTAINTY << std::endl;
  std::cout << "TRIGGER_TIME_seconds: " << WAVEDESC.TRIGGER_TIME_seconds << std::endl;
  std::cout << "TRIGGER_TIME_minutes: " << WAVEDESC.TRIGGER_TIME_minutes << std::endl;
  std::cout << "TRIGGER_TIME_hours: " << WAVEDESC.TRIGGER_TIME_hours << std::endl;
  std::cout << "TRIGGER_TIME_days: " << WAVEDESC.TRIGGER_TIME_days << std::endl;
  std::cout << "TRIGGER_TIME_months: " << WAVEDESC.TRIGGER_TIME_months << std::endl;
  std::cout << "TRIGGER_TIME_year: " << WAVEDESC.TRIGGER_TIME_year << std::endl;
  std::cout << "TRIGGER_TIME_unused: " << WAVEDESC.TRIGGER_TIME_unused << std::endl;
  std::cout << "ACQ_DURATION: " << WAVEDESC.ACQ_DURATION << std::endl;
  std::cout << "RECORD_TYPE: " << WAVEDESC.RECORD_TYPE << std::endl;
  std::cout << "PROCESSING_DONE: " << WAVEDESC.PROCESSING_DONE << std::endl;
  std::cout << "RESERVED5: " << WAVEDESC.RESERVED5 << std::endl;
  std::cout << "RIS_SWEEPS: " << WAVEDESC.RIS_SWEEPS << std::endl;
  std::cout << "TIMEBASE: " << WAVEDESC.TIMEBASE << std::endl;
  std::cout << "VERT_COUPLING: " << WAVEDESC.VERT_COUPLING << std::endl;
  std::cout << "PROBE_ATT: " << WAVEDESC.PROBE_ATT << std::endl;
  std::cout << "FIXED_VERT_GAIN: " << WAVEDESC.FIXED_VERT_GAIN << std::endl;
  std::cout << "BANDWIDTH_LIMIT: " << WAVEDESC.BANDWIDTH_LIMIT << std::endl;
  std::cout << "VERTICAL_VERNIER: " << WAVEDESC.VERTICAL_VERNIER << std::endl;
  std::cout << "ACQ_VERT_OFFSET: " << WAVEDESC.ACQ_VERT_OFFSET << std::endl;
  std::cout << "WAVE_SOURCE: " << WAVEDESC.WAVE_SOURCE << std::endl;
}

class waveform
{

 public:
  waveform(std::string filename);
  void rewind();
  bool next();
  TGraph get_graph() { return graph; };
  std::vector<float> get_transitions(float threshold, float sign);
  float get_amplitude(float tmin, float tmax);

 private:
  std::ifstream file;
  WAVEDESC_t WAVEDESC;
  char DATA[1048576];
  TGraph graph;
  
};

waveform::waveform(std::string filename)
{
  file.open(filename, std::ios::binary);
}

void
waveform::rewind()
{
  file.clear();
  file.seekg(0);
}

bool
waveform::next()
{
  graph.Set(0);
  file.read((char *)&WAVEDESC, sizeof(WAVEDESC));
  if (file.eof()) return false;
  file.read((char *)&DATA, WAVEDESC.WAVE_ARRAY_1);
  for (int i = WAVEDESC.FIRST_VALID_PNT; i < WAVEDESC.LAST_VALID_PNT; ++i) {
    double x = (WAVEDESC.HORIZ_INTERVAL * i + WAVEDESC.HORIZ_OFFSET) * 1.e9;
    double y = (WAVEDESC.VERTICAL_GAIN * DATA[i] - WAVEDESC.VERTICAL_OFFSET) * 1.e3;
    graph.SetPoint(i, x, y);
  }
  return true;
}

float
waveform::get_amplitude(float tmin, float tmax)
{
  float amp = 0.;
  for (int i = 0; i < graph.GetN(); ++i) {
    if (graph.GetX()[i] > tmin && graph.GetX()[i] < tmax && graph.GetY()[i] > amp)
      amp = graph.GetY()[i];
  }
  return amp;
}

std::vector<float>
waveform::get_transitions(float threshold, float sign)
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
