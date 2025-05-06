'''                                                     Trend Following Strategies                                                  '''
import numpy as np
import pandas as pd 
#from sysquant.estimators.vol import robust_vol_calc

def robust_vol_calc(returns: pd.Series, window: int = 20) -> pd.Series:
    """
    Estimate robust rolling volatility using Median Absolute Deviation (MAD) scaled to approximate standard deviation.
    
    Parameters:
    - returns: pd.Series of returns
    - window: rolling window size
    
    Returns:
    - pd.Series of robust volatility estimates
    """
    def mad(x):
        return np.median(np.abs(x - np.median(x)))

    # Rolling MAD, scaled to match standard deviation units
    mad_vol = returns.rolling(window).apply(lambda x: mad(x) * 1.4826, raw=True)
    return mad_vol

# Moving Average Crossover
def ewmac_strategy(data, fast_span=16, slow_span=64):
    data = data.copy()

    # EWMA instead of SMA
    data['ewma_fast'] = data['close'].ewm(span=fast_span, adjust=False).mean()
    data['ewma_slow'] = data['close'].ewm(span=slow_span, adjust=False).mean()

    # EWMAC signal: fast - slow
    data['ewmac'] = data['ewma_fast'] - data['ewma_slow']

    # Signal: positive => long, negative => short
    data['signal'] = np.where(data['ewmac'] > 0, 1, -1)

    # Returns
    data['returns'] = data['close'].pct_change()
    data['strategy_returns'] = data['signal'].shift(1) * data['returns']

    return data

def ewmac_(price : pd.DataFrame, Lfast, Lslow = None) -> pd.DataFrame:
    price['time'] = pd.to_datetime(price['time']) 
    price.set_index('time', inplace=True)         
    price = price.resample("1B").last()

    if Lslow is None:
        Lslow = 4 * Lfast

    ## We don't need to calculate the decay parameter, just use the span directly
    fast_ewma = price.ewm(span=Lfast).mean()
    slow_ewma = price.ewm(span=Lslow).mean()
    raw_ewmac = fast_ewma - slow_ewma

    vol = robust_vol_calc(price.diff())

    return raw_ewmac / vol
























# def long_only_ema_strategy(data, short_window=16, long_window=64, vol_window=10, vol_threshold=0.02, trailing_stop_pct=0.03):
#     data = data.copy()

#     # Reset the index to ensure proper sequential indices
#     data.reset_index(drop=True, inplace=True)

#     # Indicators
#     data['ema_short'] = data['close'].ewm(span=short_window, adjust=False).mean()
#     data['ema_long'] = data['close'].ewm(span=long_window, adjust=False).mean()
#     data['volatility'] = data['close'].pct_change().rolling(window=vol_window).std()

#     # Initial signal: EMA crossover and volatility filter
#     data['signal'] = 0
#     buy_condition = (data['ema_short'] > data['ema_long']) & (data['volatility'] < vol_threshold)
#     data.loc[buy_condition, 'signal'] = 1

#     # Trailing stop logic
#     data['position'] = 0
#     in_position = False
#     entry_price = 0
#     peak_price = 0

#     for i in range(1, len(data)):
#         # Ensure that we're working with valid data (i.e., no NaNs)
#         if pd.notna(data.loc[i, 'signal']):  # Access using .loc[i, 'signal'] after resetting index
#             if not in_position:
#                 if data.loc[i, 'signal'] == 1:
#                     in_position = True
#                     data.loc[i, 'position'] = 1
#                     entry_price = data.loc[i, 'close']
#                     peak_price = entry_price
#             else:
#                 peak_price = max(peak_price, data.loc[i, 'close'])
#                 if data.loc[i, 'close'] < peak_price * (1 - trailing_stop_pct):
#                     in_position = False  # exit position if trailing stop is hit
#                 else:
#                     data.loc[i, 'position'] = 1

#     # Calculate returns
#     data['returns'] = data['close'].pct_change()
#     data['strategy_returns'] = data['position'].shift(1) * data['returns']
#     return data


