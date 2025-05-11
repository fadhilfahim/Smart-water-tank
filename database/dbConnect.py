import logging
import paho.mqtt.client as mqtt
from pymongo import MongoClient
from datetime import datetime
import pytz
import json

# MongoDB connection details
MONGO_USER = 'admin'  # Replace with your MongoDB username
MONGO_PASS = 'admin'  # Replace with your MongoDB password
MONGO_DB_NAME = 'Smart_tank'  # Your database name

# MQTT broker details
BROKER = 'test.mosquitto.org'
PORT = 1883
TOPICS = ['smartwater/data']

class MongoDBClient:
    """Encapsulates MongoDB connections and writes."""

    def __init__(self):
        uri = f"mongodb+srv://{MONGO_USER}:{MONGO_PASS}@cluster0.vppoohr.mongodb.net/?retryWrites=true&w=majority&appName=Cluster0"
        self.client = MongoClient(uri, tls=True, tlsAllowInvalidCertificates=True)
        self.db = self.client[MONGO_DB_NAME]

    def insert(self, category, document):
        collection = self.db[category]
        collection.insert_one(document)
        logging.debug(f"Inserted into {category}: {document}")

# MQTT callback functions
def on_connect(client, userdata, flags, rc):
    logging.info(f"Connected to MQTT Broker with result code {rc}")
    for topic in TOPICS:
        client.subscribe(topic)

def on_message(client, userdata, msg):
    payload = msg.payload.decode()
    logging.info(f"Received message on {msg.topic}: {payload}")

    # Insert message into MongoDB
    db_client = MongoDBClient()
    timestamp = datetime.now(pytz.timezone('Asia/Colombo'))
    
    # # Construct document with topic-specific data
    # document = {
    #     'message': payload,
    #     'topic': msg.topic,
    #     'timestamp': timestamp
    # }
    
    data = json.loads(payload)
    message = {
        'water_level_cm': data.get('water_level_cm', None),
        'level_percent': data.get('level_percent', None),
        'volume_used_liters': data.get('volume_used_liters', None),
        'flow_rate_lpm': data.get('flow_rate_lpm', None),
        'ntu': data.get('ntu', None),
        'quality': data.get('quality', None),
        'timestamp': timestamp,
        'topic': msg.topic
    }

    # Use topic name as the collection name or use a predefined collection
    collection_name = msg.topic.replace("/", "_")  # Replacing '/' with underscores for valid collection names
    db_client.insert(collection_name, message)

# Set up logging
logging.basicConfig(level=logging.INFO)

# Set up MQTT client
mqtt_client = mqtt.Client()
mqtt_client.on_connect = on_connect
mqtt_client.on_message = on_message

# Connect to MQTT Broker
mqtt_client.connect(BROKER, PORT, 60)

# Start MQTT loop
mqtt_client.loop_forever()
