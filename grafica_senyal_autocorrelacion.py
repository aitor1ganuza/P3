import matplotlib
matplotlib.use('TkAgg')
import matplotlib.pyplot as plt
import soundfile as sf
import numpy as np
senyal, fm = sf.read('proba30.wav')
autocorrelation = np.correlate(senyal,senyal,mode='full')
ac = autocorrelation[len(senyal) -1:]
t = np.arange(0, len(senyal)) / fm
tau = np.arange(0, len(ac)) / fm

plt.figure()
plt.subplot(211)
plt.plot(t, senyal)
plt.title("Señal temporal de un segmento de voz")
plt.xlabel("t(s)")
plt.grid()

plt.subplot(212)
plt.plot(tau, ac)
plt.title("Autocorrelación de la señal temporal")
plt.xlabel(r"$\tau(s)$")
plt.grid()
plt.subplots_adjust(hspace=0.5)
plt.show()
