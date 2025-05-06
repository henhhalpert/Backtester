'''                                                     Performance Metrics                                     '''

import numpy as np

## Risk-Adjusted Performance Metrics:

# sharpe ratio 
def sharpe_ratio(returns, risk_free_rate=0.0):
    excess_returns = returns - risk_free_rate
    return np.sqrt(252) * excess_returns.mean() / excess_returns.std()

def rolling_sharpe_ratio(returns, window, risk_free_rate=0.0):
    excess_returns = returns - risk_free_rate
    rolling_mean = excess_returns.rolling(window).mean()
    rolling_std = excess_returns.rolling(window).std()
    sharpe = rolling_mean / rolling_std
    return sharpe

def relative_standard_dev(std, mean):
    return ( std / mean ) * 100


def rolling_volatility(df, price_col='close', lookback=25, annualize=True, trading_days=252):
    """
    Computes the rolling standard deviation (volatility) of log returns.

    Parameters:
        df (pd.DataFrame): DataFrame containing price data.
        price_col (str): Name of the column with price data.
        lookback (int): Rolling window size (in periods).
        annualize (bool): Whether to annualize the volatility.
        trading_days (int): Number of trading days per year (default: 252).

    Returns:
        pd.DataFrame: Original DataFrame with added columns:
                      'log_return' and 'rolling_volatility'
    """
    df = df.copy()
    df['log_return'] = np.log(df[price_col] / df[price_col].shift(1))
    df['rolling_volatility'] = df['log_return'].rolling(window=lookback).std()

    if annualize:
        df['rolling_volatility'] *= np.sqrt(trading_days)

    return df

def ewma_volatility(df, price_col='close', span=25, annualize=True, trading_days=252):
    """
    Computes exponentially weighted moving volatility (standard deviation of returns).

    Parameters:
        df (pd.DataFrame): DataFrame with price data.
        price_col (str): Name of the column with prices.
        span (int): EWMA span (controls decay; higher = slower decay).
        annualize (bool): Whether to annualize the result.
        trading_days (int): Days in a year for annualization.

    Returns:
        pd.DataFrame: Copy of original df with 'ewma_volatility' column.
    """
    df = df.copy()
    df['log_return'] = np.log(df[price_col] / df[price_col].shift(1))
    
    # EWMA of squared returns
    ewma_var = df['log_return']**2
    ewma_var = ewma_var.ewm(span=span, adjust=False).mean()
    
    # Volatility = sqrt of EWMA variance
    df['ewma_volatility'] = np.sqrt(ewma_var)

    if annualize:
        df['ewma_volatility'] *= np.sqrt(trading_days)

    return df
