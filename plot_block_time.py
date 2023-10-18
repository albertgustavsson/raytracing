import sys
import pandas as pd
import matplotlib.pyplot as plt
from matplotlib import colors
import numpy as np

if __name__ == '__main__':
    if len(sys.argv) < 2:
        print(f"Expected 1 argument. Usage:\n    python {sys.argv[0]} <path-to-stats.csv>")
        sys.exit(-1)
    filename=sys.argv[1]
    print(filename)
    data = pd.read_csv('raytracing/stats.csv')
    min_x = data['block_x'].min()
    max_x = data['block_x'].max()
    min_y = data['block_y'].min()
    max_y = data['block_y'].max()
    data_grid=np.zeros(shape=(max_y+1, max_x+1))
    for i in data.index:
        x=data['block_x'][i]
        y=data['block_y'][i]
        data_grid[y][x] = data['render_time'][i]
    data_grid=np.divide(data_grid, 1000)

    cmap = colors.ListedColormap(['red', 'blue'])
    bounds = [0,10,20]
    norm = colors.BoundaryNorm(bounds, cmap.N)
    fig, ax = plt.subplots(figsize=(7,4))
    c = ax.imshow(data_grid, origin='lower', aspect='equal')
    ax.set_xlim(min_x-0.5, max_x+0.5)
    cbar = fig.colorbar(c, ax=ax)
    cbar.ax.set_ylabel('Block render time [ms]')
    plt.tight_layout()
    
    fig.savefig('images/block-render-times.png')
    plt.show()