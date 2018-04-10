#---------------------------------------#
# python plotter for position/angle
# displacement
# need to have smoothed_data.txt and raw_data.txt
# in same directory
#---------------------------------------#
import numpy as np
import matplotlib.pyplot as plt
smoothed_data = []
raw_data = []
with open("smoothed_data.txt", "r") as f:
    for line in f:
            smoothed_data.append(line.split())

with open("raw_data.txt", "r") as f:
    for line in f:
            raw_data.append(line.split())

plt.figure(1)
plt.subplot(211)
plt.plot(smoothed_data)
plt.subplot(212)
plt.plot(raw_data)
