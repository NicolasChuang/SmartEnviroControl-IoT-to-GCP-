import pymongo
import paho.mqtt.client as mqtt
from datetime import datetime

# MongoDB configuration
mongo_client = pymongo.MongoClient("mongodb://localhost:27017/")
db = mongo_client["smarthome"]
collection = db["iot"]

# MQTT configuration
mqtt_broker_address = '34.42.110.62'
mqtt_topic = 'iot'

def validate_payload(payload):
    try:
        # Split the payload into individual values
        temperature, humidity, pump, spin_duration = map(float, payload.split('|'))

        # Perform specific validation checks
        if 0.0 <= temperature <= 100.0 and 0.0 <= humidity <= 100.0 and pump >= 0 and spin_duration >= 0.0:
            return True
        else:
            return False

    except (ValueError, IndexError):
        return False

def on_message(client, userdata, message):
    payload = message.payload.decode('utf-8')
    print(f'Received message: {payload}')

    # Validate the payload
    if validate_payload(payload):
        # Convert MQTT timestamp to datetime
        timestamp = datetime.utcnow() # Use current UTC time
        datetime_obj = timestamp.strftime("%Y-%m-%dT%H:%M:%S.%fZ")

        # Process the payload and insert into MongoDB with proper timestamp
        document = {
            'timestamp': datetime_obj,
            'data': payload
        }
        collection.insert_one(document)
        print('Data ingested into MongoDB')
    else:
        print('Invalid payload. Discarding.')

client = mqtt.Client()
client.on_message = on_message

# Connect to MQTT broker
client.connect(mqtt_broker_address, 1883, 60)

# Subscribe to MQTT topic
client.subscribe(mqtt_topic)

# Start the MQTT loop
client.loop_forever()