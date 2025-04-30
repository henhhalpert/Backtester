import pandas as pd
import matplotlib.pyplot as plt
from matplotlib.dates import DateFormatter
from .data_loader import *

def move_figure(f, x, y):
    """Move figure window to (x, y) coordinates."""
    backend = plt.get_backend()
    if backend == 'tkagg':
        f.canvas.manager.window.wm_geometry("+%d+%d" % (x, y))
    elif backend == 'wxagg':
        f.canvas.manager.window.SetPosition((x, y))
    elif backend == 'qt':
        f.canvas.manager.window.move(x, y)
    else:
        print(f"Move not supported for backend {backend}")

# ========== 3. Visualization ==========
def plot_close_price(df):
    """Plot the Close prices over time."""
    fig = plt.figure(figsize=(12, 6))
    #plt.figure(figsize=(12, 6))
    plt.plot(df['time'], df['close'], label='Close Price', color='blue')
    plt.title("Close Prices Over Time")
    plt.xlabel("Time")
    plt.ylabel("Price")
    plt.grid(True)
    plt.legend()
    plt.gca().xaxis.set_major_formatter(DateFormatter('%Y-%m-%d %H:%M'))
    plt.xticks(rotation=45)
    plt.tight_layout()
    move_figure(fig, 0, 0)  # Top-left

def plot_ohlc(df):
    """Plot Open, High, Low, Close on the same chart."""
    fig = plt.figure(figsize=(12, 6))
    plt.plot(df['time'], df['open'], label='Open', linestyle='--')
    plt.plot(df['time'], df['high'], label='High', linestyle='--')
    plt.plot(df['time'], df['low'], label='Low', linestyle='--')
    plt.plot(df['time'], df['close'], label='Close', linewidth=2)
    plt.title("OHLC Prices Over Time")
    plt.xlabel("Time")
    plt.ylabel("Price")
    plt.legend()
    plt.grid(True)
    plt.gca().xaxis.set_major_formatter(DateFormatter('%Y-%m-%d %H:%M'))
    plt.xticks(rotation=45)
    plt.tight_layout()
    move_figure(fig, 1350, 0)

def plot_volume(df):
    """Plot Volume traded over time."""
    fig =  fig = plt.figure(figsize=(10, 5))
    plt.bar(df['time'], df['volume'], width=0.5, color='purple')
    plt.title("Volume Over Time")
    plt.xlabel("Time")
    plt.ylabel("Volume")
    plt.grid(True)
    plt.gca().xaxis.set_major_formatter(DateFormatter('%Y-%m-%d %H:%M'))
    plt.xticks(rotation=45)
    plt.tight_layout()
    move_figure(fig, 800, 700)


def visualize():
    df = fetch_ohlcv_data()

    # first visualization 
    plot_close_price(df)
    plot_ohlc(df)
    plot_volume(df)

    plt.show()