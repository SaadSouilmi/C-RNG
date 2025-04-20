from argparse import ArgumentParser

import numpy as np 
import matplotlib.pyplot as plt 
from scipy.stats import norm
import scienceplots

plt.style.use(["grid", "science"])

if __name__ == "__main__":
        
    parser = ArgumentParser()
    parser.add_argument("--display", action="store_true")
    parser.add_argument("--save", action="store_true")
    args = parser.parse_args()
    
    # Frontier U_2 = \frac{a}{\sqrt{a^2 - 2\log(U_1)}}
    a = np.arange(1, 6).reshape(-1, 1)
    u = np.linspace(0, 1)
    log_u = np.log(u)
    ys = a / np.sqrt(a**2  - 2*log_u)
    
    fig = plt.figure(figsize=(7, 5))
    
    for a_, y in zip(a, ys):
        plt.plot(u, y, label=rf"$a = {a_.item()}$", lw=2, alpha=0.7)
    
    plt.xlabel("$U_1$")
    plt.ylabel("$U_2$")
    plt.title(r"$U_2 = \frac{a}{\sqrt{a^2 - 2\log(U_1)}}$")
    plt.xlim(0, 1)
    plt.ylim(0, 1)
    legend = plt.legend(fancybox=True)
    legend.get_frame().set_edgecolor('black')
    legend.get_frame().set_linewidth(1.5)
    if args.display:
        plt.show()  
        plt.close()
    if args.save:
        fig.savefig("/Users/saad/Code/dev_c/RNG/figures/frontier_u_2.pdf")
    
    # Efficiency w.r.t a 
    
    a = np.linspace(1, 10)
    efficiency = a * np.exp(a**2 / 2) * norm.sf(a) * np.sqrt(2*np.pi) 
    
    fig = plt.figure(figsize=(7, 5))
    
    plt.plot(a, efficiency, marker="o", ms=3, mec="k", lw=2, alpha=.7)
    
    plt.xlabel("$a$")
    plt.title(r"Efficiency i.e: $Prb\left(U_2 \leqslant \frac{a}{\sqrt{a^2 - 2\log(U_1)}} \right)$")
    plt.xlim(1, 10)
    plt.ylim(efficiency[0], 1)
    if args.display:
        plt.show()
        plt.close()
    if args.save:
        fig.savefig("/Users/saad/Code/dev_c/RNG/figures/efficiency_u_2.pdf")
