'''                                                     Performance Metrics                                     '''

import numpy as np

## Risk-Adjusted Performance Metrics:
# sharpe ratio 
def sharpe_ratio(returns, risk_free_rate=0.0):
    excess_returns = returns - risk_free_rate
    return np.sqrt(252) * excess_returns.mean() / excess_returns.std()


