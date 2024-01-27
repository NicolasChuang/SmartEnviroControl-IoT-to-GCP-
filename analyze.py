import pandas as pd

input_file = input("Enter the input CSV file name: ")

# Read the CSV file into a DataFrame
df = pd.read_csv(input_file)

# Calculate the statistics
statistics = df.describe().transpose()[['min', '25%', '50%', '75%', 'max']]

# Print the results
print(statistics)