import matplotlib
matplotlib.use('TKAgg')
import math
import matplotlib.pyplot as plt
import matplotlib.animation as animation

import numpy as np
from numpy.random import rand
import random
from numpy import savetxt

import scipy
from numpy import array
from matplotlib import colors


from tqdm import tqdm



# setting up the values for the grid
ON = 1
OFF = 0
vals = [ON, OFF]

N = 50 #int(input('Array size: '))



def random_state():
    '''
    returns a grid of NxN random values
    '''
    return np.random.choice(vals, size=(N,N))



def selection(grid, p):
    '''
    ON  --> OFF with probability 1 - p
    ON --> infects one of its four nearest neighbours with probability p
    
    the neighbour to be infected is chosen randomly
    if the chosen neighbour is already infected nothing happens
    
    '''
    
    r = rand()
    i, j  = random.choices(np.arange(N), k=2)          # choose a random cell
    
    # # this ensures the randomly chosen neighbour is not the same as the first cell (unnecessary)
    # while True:
    #     k, l  = random.choices(np.arange(N), k=2)
    #     if (k,l) != (i,j):
    #         k, l  = k, l
    #         break
    
    neighbours = [[(i-1)%N, j], [(i+1)%N, j], [i, (j+1)%N], [i, (j-1)%N]]
    # choose random neighbour
    k, l = neighbours[random.choice(np.arange(len(neighbours)))]
    # print(np.array((i,j))- np.array((k,l)))
    
    if grid[i ,j] == ON:
        if (1 - p) > r:
            grid[i, j] = OFF
        
        else:    
            # with probability p it infects one of its four nearest neighbours
            # print("Neighbour infected!")
            grid[k, l] = ON
                
            # if the chosen neighbour is already infected nothing happens
            # else:
            #     pass
        
    return grid
    

def simulation():

    '''
    Animated simulation of the SIRS model
        
    Returns
    -------
    animated simulation
        
    '''
    
    nstep = 250
    p = float(input("p: "))

    #initialise spins randomly
    state = random_state()
    # make a color map of fixed colors
    # cmap = colors.ListedColormap(['white', 'red'])
    # bounds=[0,0.5,1]
    # norm = colors.BoundaryNorm(bounds, cmap.N)

    # plt.figure()
    # plt.imshow(state, animated=True, interpolation='nearest', origin='lower',
    #                 cmap=cmap, norm=norm) 
    # plt.colorbar()
    
    no_infected = [] 
    times = []
    for n in tqdm(range(nstep)):    
        for i in range(N**2):
            state = selection(state, p)
            
        # occasionally plot or update measurements, eg every 10 sweeps
        # if(n%1==0):
        #     plt.cla()
        #     plt.title(n)
        #     im = plt.imshow(state, animated=True, interpolation='nearest', origin='lower',
        #             cmap=cmap, norm=norm)
        #     plt.draw()
        #     plt.pause(0.0001)
        
        if n%1==0:
            times.append(n)
            no_infected.append(np.count_nonzero(state==ON))
    
    data = no_infected, times
    np.savetxt(f"no_infected_VS_time_p={p}.csv", data)
    
    plt.clf()
    plt.plot(times, no_infected)
    plt.savefig(f"p={p}.png")
    plt.show()


def many_probs():
    
    nstep = 400
    res = 0.01
    prob_array = np.arange(0.55,0.70 + res, res)
    A_array = np.empty(len(prob_array))
    f_array = np.empty(len(prob_array))
    var_array = np.empty(len(prob_array))
    inf_errs = np.empty(len(prob_array))
    
    for p, prob in tqdm(enumerate(prob_array)):
        state = random_state()
        no_infected = [] 
        for n in tqdm(range(nstep)):    
            for i in range(N**2):
                state = selection(state, prob)
            
            if n > 200:
                if np.count_nonzero(state==ON) == 0.:
                    no_infected.append(0)
                    break
                
                else:
                    no_infected.append(np.count_nonzero(state==ON))
                    
        # estimate bootstrap errors
        resampled_calc = []
        for i in range(200):
            resampled_I = np.random.choice(no_infected, 800)
            new_calc_I = np.var(resampled_I)/N**2
            resampled_calc.append(new_calc_I)
        
        inf_errs[p] = np.var(resampled_calc)**(1/2)
        A_array[p] = np.nanmean(no_infected)/N
        var_array[p] = np.nanvar(no_infected)/N
        
        
    f_array = A_array/N**2
                
    data = prob_array, A_array, f_array, var_array, inf_errs
    np.savetxt("data.csv", np.c_[data])
        
    
    plt.clf()
    plt.plot(prob_array, f_array)
    plt.errorbar(prob_array, f_array, yerr=inf_errs, fmt='x', ecolor = 'lightblue',color='m')
    plt.savefig(f"f_graph.png")
    plt.show()
    

# call main
if __name__ == '__main__':
    # move(random_state(50), 50)
    # simulation()
    many_probs()
    
    
    