import pandas as pd
from sqlalchemy import create_engine
from utils.design_patterns import SingletonMeta

DB_CONFIG = {
    "dbname": "backtester_local",
    "user": "postgres",
    "password": "1111",
    "host": "localhost",
    "port": "5432",
}

class OHLCVDataLoader(metaclass=SingletonMeta):
    def __init__(self):
        self._df = self._fetch_ohlcv_data()

    def _fetch_ohlcv_data(self):
        engine = create_engine(
            f"postgresql+psycopg2://{DB_CONFIG['user']}:{DB_CONFIG['password']}@{DB_CONFIG['host']}:{DB_CONFIG['port']}/{DB_CONFIG['dbname']}"
        )
        query = """
            SELECT time, open, high, low, close, volume
            FROM ohlcv_data
            ORDER BY time ASC;
        """
        df = pd.read_sql(query, engine, parse_dates=['time'])
        df['time'] = pd.to_datetime(df['time'], errors='coerce')
        df = df.dropna(subset=['time', 'close'])
        return df

    def get_data(self):
        return self._df

    # in / out sample getters 
    # 75/25
    def get_in_sample75(self):
        split_index = int(len(self._df) * 0.75)
        return self._df.iloc[:split_index].copy()
    def get_out_sample25(self):
        split_index = int(len(self._df) * 0.75)
        return self._df.iloc[split_index:].copy()
    # 70/30 
    def get_in_sample70(self):
        split_index = int(len(self._df) * 0.70)
        return self._df.iloc[:split_index].copy()
    def get_out_sample30(self):
        split_index = int(len(self._df) * 0.70)
        return self._df.iloc[split_index:].copy()
    
