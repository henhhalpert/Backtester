-- 								========= Responsibility: validity of entries insterted to ohlcv hypertable =========
-- 										     ALL queries when checking data should return nil. 
--									  If some entries do return something, those entries must be deleted.

-- #1. Missing Values Check 
SELECT COUNT(*) AS bad_entry_count
FROM ohlcv_data
WHERE open IS NULL
   OR high IS NULL
   OR low IS NULL
   OR close IS NULL
   OR volume IS NULL
   OR time IS NULL;
   
-- #2. logical consistency check, these must always hold:
SELECT COUNT(*) AS bad_entry_count
FROM ohlcv_data
WHERE NOT (
    low <= open AND open <= high
    AND low <= close AND close <= high
    AND low <= high
);

-- #3. reasonable values: all values must be greater than 0 (let us assume that volume can be 0 at times):
SELECT COUNT(*) AS bad_entry_count
FROM ohlcv_data
WHERE open <= 0
   OR high <= 0
   OR low <= 0
   OR close <= 0
   OR volume < 0;

-- #4.timestamp validation: 
-- #a. check for null timestamps, was already checked in #1.
SELECT COALESCE(SUM(cnt - 1), 0) AS bad_entry_count
FROM (
    SELECT COUNT(*) AS cnt
    FROM ohlcv_data
    GROUP BY time
    HAVING COUNT(*) > 1
) AS subquery;


-- #5 check for duplicate data:
SELECT COALESCE(SUM(cnt - 1), 0) AS bad_entry_count
FROM (
    SELECT COUNT(*) AS cnt
    FROM (
        SELECT md5(open::text || high::text || low::text || close::text || volume::text) AS row_hash
        FROM ohlcv_data
    ) AS hashed
    GROUP BY row_hash
    HAVING COUNT(*) > 1
) AS subquery;

