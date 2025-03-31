#! /usr/bin/env python

import wavedesc
import argparse
import numpy as np
from scipy.signal import butter, iirnotch, filtfilt
from scipy.signal import find_peaks
import matplotlib.pyplot as plt

def butter_lowpass_filter(data, cutoff, fs, order=4):
    nyquist = 0.5 * fs
    normal_cutoff = cutoff / nyquist
    b, a = butter(order, normal_cutoff, btype='low', analog=False)
    return filtfilt(b, a, data)

parser = argparse.ArgumentParser(description='wavedesc')
parser.add_argument('--input', type=str, required=True, help='Input file name')
parser.add_argument('--output', type=str, required=True, help='Output file name')
parser.add_argument('--baseline', type=str, default='none', choices=['none', 'median'], help='Baseline subtraction method')
parser.add_argument('--lowpass', type=float, help='Low-pass filter')
parser.add_argument('--notch', type=float, help='Notch filter')
parser.add_argument('--thr', type=float, required=True, help='Threshold')
parser.add_argument('--nwaveforms', type=int, default=1000, help='Maximum number of waveforms to process')
args = parser.parse_args()

wd = wavedesc.wavedesc(args.input)

time_differences = []
nwaveforms = 0

file = open(args.output, 'w')

while wd.next() and nwaveforms < args.nwaveforms:
    print(f' --- doing waveform {nwaveforms}')
    nwaveforms += 1
    
    data = wd.get()
    x, y = zip(*data)
    x = np.asarray(x)
    y = np.asarray(y)
    dt = (x[1] - x[0]) * 1.e-9
    fs = 1 / dt

    if args.lowpass:
        y = butter_lowpass_filter(y, cutoff=args.lowpass, fs=fs)

    if args.notch:
        b, a = iirnotch(w0=args.notch, Q=30, fs=fs)
        y = filtfilt(b, a, y)

    baseline = 0.
    if args.baseline == 'median':
        baseline = np.median(y)
    y = y - baseline

    peaks, properties = find_peaks(y, height=args.thr, prominence=args.thr)
    peaks = np.asarray(peaks)
    if len(peaks > 1):
        peak_times = x[peaks]
        diffs = np.diff(peak_times)
        time_differences.extend(diffs)
        for d in diffs:
            file.write(f'{d}\n')

file.close()

exit(0)

n_bins = 100
log_bins = np.logspace(np.log10(10.), np.log10(1.e5), n_bins)

counts, bin_edges = np.histogram(time_differences, bins=log_bins)
bin_widths = np.diff(bin_edges)
counts_per_unit = counts / bin_widths  # normalise by bin width

# Plot using left edges and widths
plt.bar(bin_edges[:-1], counts_per_unit, width=bin_widths, align='edge', edgecolor='black')
plt.xscale('log')
plt.xlabel("Time between peaks")
plt.ylabel("Counts per unit time")
plt.title("Log-binned Histogram of Time Differences (Normalised by Bin Width)")
plt.grid(True, which='both', linestyle='--', linewidth=0.5)
plt.tight_layout()
plt.show()

exit(0)

# Plot histogram with log-spaced bins
plt.hist(time_differences, bins=log_bins, edgecolor='black')
plt.xscale('log')
plt.xlabel("Time between peaks")
plt.ylabel("Counts")
plt.title("Histogram of Time Differences with Logarithmic Binning")
plt.grid(True, which='both', linestyle='--', linewidth=0.5)
plt.tight_layout()
plt.show()
