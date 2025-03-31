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
parser.add_argument('--thrmin', type=float, required=True, help='Threshold scan minimum')
parser.add_argument('--thrmax', type=float, required=True, help='Threshold scan maximum')
parser.add_argument('--thrstep', type=float, default=0.1, help='Threshold scan step')
parser.add_argument('--nwaveforms', type=int, default=1000, help='Maximum number of waveforms to process')
args = parser.parse_args()

wd = wavedesc.wavedesc(args.input)

nwaveforms = 0
nnpeaks = {thr: 0. for thr in np.arange(args.thrmin, args.thrmax, args.thrstep)}

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

    for thr in np.arange(args.thrmin, args.thrmax, args.thrstep):

        data = y
        threshold = thr
        if thr < 0:
            data = -data
            threshold = -threshold
        
        peaks, properties = find_peaks(data, height=threshold, prominence=threshold)
        peaks = np.asarray(peaks)
        nnpeaks[thr] += len(peaks)

        continue
        
        ppeaks, properties = find_peaks(y, height=thr, prominence=thr)
        ppeaks = np.asarray(ppeaks)
        
        npeaks, properties = find_peaks(-y, height=thr, prominence=thr)
        npeaks = np.asarray(npeaks)

        speaks = []
        for ppeak in ppeaks:
            is_ringing = False
            for npeak in npeaks:
                deltat = abs(x[ppeak] - x[npeak])
                if deltat < 200:
                    is_ringing = True
                    break
            if not is_ringing:
                speaks.append(ppeak)

        nnpeaks[thr] += len(speaks)

        
        
seconds = nwaveforms * 100e-6
thresholds = sorted(nnpeaks.keys())
rate = [nnpeaks[thr] / seconds for thr in thresholds]
ratee = [np.sqrt(nnpeaks[thr]) / seconds for thr in thresholds]

with open(args.output, 'w') as file:
    for thr, r, re in zip(thresholds, rate, ratee):
        file.write(f'{thr} {r} 0. {re}\n')

exit(0)
        
plt.figure(figsize=(8, 5))
plt.errorbar(thresholds, rate, yerr=ratee, fmt='o-', capsize=4)
plt.xlabel("threshold (mV)")
plt.ylabel("rate (Hz)")
plt.title("Rate vs Threshold")
plt.yscale("log")
plt.grid(True)
plt.tight_layout()
plt.show()
