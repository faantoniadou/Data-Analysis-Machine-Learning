#%%
import pandas as pd
import numpy as np
import matplotlib.pyplot as plt

#%%
totalEnergy = pd.read_csv( 'output_h1_LayerTotal.csv', comment='#', \
names=['Sw', 'Sw2', 'Sxw0', 'Sx2w0'] )
electronEnergy = pd.read_csv( 'output_h1_LayerElectron.csv', comment='#', \
names=['Sw', 'Sw2', 'Sxw0', 'Sx2w0'] )


totalEnergy = np.array(totalEnergy)[1:].astype(float)
electronEnergy = np.array(electronEnergy)[1:].astype(float)

print(f'Fraction of energy deposited by electrons = {np.sum(electronEnergy)/np.sum(totalEnergy)}')
