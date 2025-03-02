import pandas as pd

# Read the Excel file
df = pd.read_excel('coordinates.xlsx')

# Save as CSV
df.to_csv('coordinates.csv', index=False)
print("Conversion complete! File saved as coordinates.csv")
