from .data_loader import *
from .bootstrapper import *
from metrics.performance import *
from strategies.trend_following import *
from matplotlib.pyplot import show
from .ohlcv_visualizer import *

import numpy as np


def compute_basic_stats(df):
    print("\n===== Basic Metrics =====")
    numeric_cols = ['open', 'high', 'low', 'close', 'volume']
    stats = df[numeric_cols].agg(['mean', 'median', 'min', 'max', 'std'])
    print(stats)
    print("\n")

def compute_return_stats(df):
    print("\n===== Return metrics =====")

    df['returns'] = df['close'].pct_change()
    df['log_returns'] = np.log(df['close'] / df['close'].shift(1))
    print("Mean Return:", df['returns'].mean())
    print("Standard Deviation of Return:", df['returns'].std())
    print("Mean Log Return:", df['log_returns'].mean())
    print("Standard Deviation of Log Return:", df['log_returns'].std())
    print("Skewness:", df['returns'].skew())
    print("Kurtosis:", df['returns'].kurt())
    print("\n")

def compute_volatility(df, window=14):
    print(f"\n===== {window}-Period Rolling Volatility =====")
    df['rolling_volatility'] = df['returns'].rolling(window).std()
    print(df['rolling_volatility'].describe())
    print("\n")

def compute_drawdowns(df):
    print("\n===== Drawdown Statistics =====")
    df['cum_returns'] = (1 + df['returns']).cumprod()
    df['rolling_max'] = df['cum_returns'].cummax()
    df['drawdown'] = df['cum_returns'] / df['rolling_max'] - 1
    max_drawdown = df['drawdown'].min()
    print("Maximum Drawdown:", max_drawdown)
    print("\n")

def compute_volume_stats(df):
    print("\n===== Volume Analysis =====")
    print("Volume Mean:", df['volume'].mean())
    print("Volume Std:", df['volume'].std())
    print("Volume Skewness:", df['volume'].skew())
    print("Volume Kurtosis:", df['volume'].kurt())
    print("\n")

def metrics():
    # data
    # loader = OHLCVDataLoader()
    # df = loader.get_data()

    # # metrics 
    # compute_basic_stats(df)
    # compute_return_stats(df)
    # compute_volatility(df)
    # compute_drawdowns(df)
    # compute_volume_stats(df)

    # bootstrap sharpe ratio check 
    #load data
    loader = OHLCVDataLoader()
    df = loader.get_last_7_years()
    

    #print("CV:")
    #print(f"{relative_standard_dev(df["close"].std(), df["close"].mean())}")
    #compute_basic_stats(df)

    #df_rolling = rolling_volatility(df, price_col='close', lookback=25)
    #plot_wrt_time(df_rolling, "rolling_volatility", "EWMA Volatility span")
    #df_ewma_vol = ewma_volatility(df)
    #plot_wrt_time(df_ewma_vol, "ewma_volatility", "EWMA Volatility span")
    #plt.show()
    #plot_rolling_volatility(df_rolling)

    #sr_rolling = rolling_sharpe_ratio()

    # strategy
    in_sample_strategy = ewmac_strategy(df, 32, 128)
    print(f"no BS = {sharpe_ratio(in_sample_strategy['strategy_returns'].dropna())}")
    
    bootstrapped_srs = bootstrap_sharpe(df)
    print(f"yes BS = {np.median(bootstrapped_srs):.4f}")


    '''
    different SR when using BS, expected lower, got very bery low compared to single sharpe ratio 

    how to move on ? 
    Plot the distribution of bootstrapped Sharpe ratios — is it centered near 0 or skewed?

    Compute confidence intervals (e.g., 5th and 95th percentiles).
    
    Evaluate your strategy across different market regimes (bull, bear, sideways).
    
    Consider more robust signals, or strategies with fewer assumptions.
    '''

    #plot_close_time(df)

    # in_sample_sr = sharpe_ratio(in_sample_strategy['strategy_returns'].dropna())
    # print(f"In-sample Sharpe Ratio: {in_sample_sr}")
