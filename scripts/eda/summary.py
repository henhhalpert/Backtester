from .data_loader import *
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
    loader = OHLCVDataLoader()
    df = loader.get_data()

    # metrics 
    compute_basic_stats(df)
    compute_return_stats(df)
    compute_volatility(df)
    compute_drawdowns(df)
    compute_volume_stats(df)