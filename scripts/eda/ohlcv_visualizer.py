import pandas as pd
import matplotlib.pyplot as plt
import psycopg2
from matplotlib.dates import DateFormatter

def visualize():
    # DB config
    DB_NAME = "backtester_local"
    DB_USER = "postgres"
    DB_PASS = "1111"
    DB_HOST = "localhost"
    DB_PORT = "5432"

    # Connect and query
    conn = psycopg2.connect(
        dbname=DB_NAME, user=DB_USER, password=DB_PASS, host=DB_HOST, port=DB_PORT
    )

    query = """
        SELECT time, open, high, low, close, volume
        FROM ohlcv_data
        ORDER BY time ASC
        LIMIT 1000;
    """

    # store in df 
    df = pd.read_sql(query, conn, parse_dates=['time'])
    conn.close()

    # Plot
    plt.figure(figsize=(12, 6))
    plt.plot(df['time'], df['close'], label='Close Price')
    plt.title("OHLCV Close Prices")
    plt.xlabel("Time")
    plt.ylabel("Price")
    plt.grid(True)
    plt.legend()
    plt.gca().xaxis.set_major_formatter(DateFormatter('%Y-%m-%d %H:%M'))
    plt.xticks(rotation=45)
    plt.tight_layout()
    plt.show()