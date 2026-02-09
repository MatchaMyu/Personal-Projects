import sys
import yfinance as yf
import pandas as pd
from sklearn.linear_model import LinearRegression
import numpy as np

ticker = sys.argv[1] if len(sys.argv) > 1 else "AAPL"
if len(sys.argv) == 1:
    print("No ticker provided; defaulting to AAPL")

# Download data
data = yf.download(ticker, period="5y")
data.to_csv("stock_raw.csv")

# Prepare dataset
data["Days"] = np.arange(len(data))
X = data[["Days"]]
y = data["Close"].squeeze()

# Train model
model = LinearRegression()
model.fit(X, y)

# Predict next day
next_day = [[len(data)]]
pred = model.predict(next_day)
prediction_value = float(np.squeeze(pred))
print(f"Predicted next closing price for {ticker}: {prediction_value:.2f}")

