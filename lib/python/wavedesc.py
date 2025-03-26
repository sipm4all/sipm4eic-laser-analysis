#! /usr/bin/env python

import wavedesc
import argparse
import numpy as np
import matplotlib.pyplot as plt

parser = argparse.ArgumentParser(description='wavedesc')
parser.add_argument('--input', required=True, help='input file name')
args = parser.parse_args()

wd = wavedesc.wavedesc(args.input)
wd.next()

data = wd.get()
x, y = zip(*data)

plt.plot(x, y)
plt.xlabel('X-axis')
plt.ylabel('Y-axis')
plt.title('Waveform')
plt.show()

