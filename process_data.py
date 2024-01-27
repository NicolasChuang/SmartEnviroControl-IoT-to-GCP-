import pandas as pd

# Ask for input and output file names
input_file = input("Enter the input CSV file name: ")
output_file = input("Enter the output CSV file name: ")

# Read the CSV file
df = pd.read_csv(input_file)

# Add a header to the DataFrame
df.columns = ['data']

# Split the 'data' column into separate columns
df[['temperature', 'humidity', 'pump', 'spin_duration']] = df['data'].str.split('|', expand=True)

# Drop the original 'data' column
df = df.drop(columns=['data'])

# Save the DataFrame to a new CSV file with a header
df.to_csv(output_file, index=False)

print(f"File formatted and saved as {output_file}")