'''                                                     Trend Following Strategies                                                  '''
import numpy as np

# Moving Average Crossover
def moving_average_strategy(data, short_window=10, long_window=50):
    data = data.copy()
    data['ma_short'] = data['close'].rolling(window=short_window).mean()
    data['ma_long'] = data['close'].rolling(window=long_window).mean()
    data['signal'] = 0
    data['signal'][long_window:] = np.where(data['ma_short'][long_window:] > data['ma_long'][long_window:], 1, -1)
    data['returns'] = data['close'].pct_change()
    data['strategy_returns'] = data['signal'].shift(1) * data['returns']
    return data
