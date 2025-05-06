import numpy as np
import pandas as pd
from metrics.performance import sharpe_ratio
from strategies.trend_following import *

def bootstrap_sharpe(data, n_bootstraps=1000, block_size=50):
    sr_values = []
    n = len(data)

    for _ in range(n_bootstraps):
        indices = np.random.randint(0, n - block_size, size=n // block_size)
        sample = pd.concat([data.iloc[i:i+block_size] for i in indices])
        strat = ewmac_strategy(sample, 32, 128)
        sr = sharpe_ratio(strat['strategy_returns'].dropna())
        sr_values.append(sr)

    return np.array(sr_values)
