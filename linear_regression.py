import pandas as pd
from sklearn.model_selection import train_test_split
from sklearn.linear_model import LinearRegression
from sklearn.metrics import mean_squared_error
import numpy as np

# Ask for input CSV file name
input_file = input("Enter the input CSV file name: ")

# Read the CSV file into a DataFrame
df = pd.read_csv(input_file)

# Display available headers
print("Available headers in the CSV file:")
print(df.columns)

# Ask the user to choose features and target variable
features = input("Enter the features (comma-separated): ").split(',')
target_variable = input("Enter the target variable: ")

# Define features (X) and target variable (y)
X = df[features]
y = df[target_variable]

# Split the dataset into training and testing sets
X_train, X_test, y_train, y_test = train_test_split(X, y, test_size=0.2, random_state=42)

# Create a linear regression model
model = LinearRegression()

# Train the model
model.fit(X_train, y_train)

# Make predictions on the test set
y_pred = model.predict(X_test)

# Evaluate the model (optional)
mse = mean_squared_error(y_test, y_pred)
print(f'Mean Squared Error: {mse}')

# Allow user to enter values for a new prediction
new_data_point_str = input("Enter values for a new prediction (comma-separated): ")
new_data_point = np.array([float(value) for value in new_data_point_str.split(',')]).reshape(1, -1)

# Make a prediction for the new data point
prediction = model.predict(new_data_point)
print(f'Predicted {target_variable} for new data point: {prediction[0]}')
