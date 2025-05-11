import pandas as pd
from pymongo import MongoClient

# MongoDB connection details
MONGO_USER = 'admin'
MONGO_PASS = 'admin'
MONGO_DB_NAME = 'Smart_tank'
COLLECTION_NAME = 'smartwater_data'  # Change based on your topic

# Connect to MongoDB
uri = f"mongodb+srv://{MONGO_USER}:{MONGO_PASS}@cluster0.vppoohr.mongodb.net/?retryWrites=true&w=majority&appName=Cluster0"
client = MongoClient(uri, tls=True, tlsAllowInvalidCertificates=True)
db = client[MONGO_DB_NAME]
collection = db[COLLECTION_NAME]

# Fetch all documents
data = list(collection.find({}))

# Convert to DataFrame
df = pd.DataFrame(data)

# Drop MongoDB-specific _id field
if '_id' in df.columns:
    df.drop(columns=['_id'], inplace=True)

# Save to CSV
df.to_csv('smartwater_data.csv', index=False)
print("✅ Data exported to smartwater_data.csv")
