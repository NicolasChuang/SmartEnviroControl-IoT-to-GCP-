Follow these steps to set up GCP cloud VM with connection to arduino IDE through MQTT  

Development Environment Setup in GCP:  

Step 1: Setup a Google Cloud VM  
1) Create a Virtual Machine (Ubuntu) in Google Cloud.
a. Navigate to the Google Cloud Console.  
b. Click on the Navigation menu and select "Compute Engine" > "VM instances."  
c. Click "Create Instance" to create a new virtual machine.  
d. Choose the operating system (e.g., Ubuntu) and configure other settings as needed.  
e. Click "Create" to create the VM.  
2) Connect to the VM via SSH.  
a. Once the VM is created, click the SSH button next to the VM instance in the Google Cloud Console.  
3) Setup the VM.  
a. \# sudo apt-get update  
b. \# sudo apt-get upgrade  

Step 2: Setup an MQTT server in Google Cloud VM  
1) Install MQTT broker.  
a. \# sudo apt-get install mosquitto  
2) Install MQTT clients such as publishers and subscriber.  
a. \# sudo apt-get install mosquito-clients  
3) Create a firewall rule to allow traffic on port 1883.  
4) Add the firewall rule to the VM instance's configuration.  
a. Edit the default-allow-internal rule to allow ingress traffic on TCP port 1883.  

Step 3: Setup the sensor and actuator  
1) Connect the DHT11 with Maker Feather AIoT S3.  
a. Connect the DHT11 sensor, servo actuator and motor actuator to the appropriate pins on the Maker Feather AIoT S3.  
2) Prepare Arduino IDE.  
a. Install the Arduino IDE on local machine.  
b. Install the necessary board support for Maker Feather AIoT S3 in the Arduino IDE.  
3) Write the firmware to publish data to the Google Cloud VM instance in Step 1.  
a. Use the "PubSubClient" library for MQTT communication.  
b. Copy Assg2.INO file into Arduino IDE  
c. Connect the Maker Feather AIoT S3 to the computer and upload the firmware. (change the WIFI_SSID, WIFI_PASSWORD, MQTT_SERVER)  

Step 4: Enable and Start Mosquitto Service  
1) Check the status of the Mosquitto service.  
a. \# sudo systemctl status mosquitto  
2) Enable and start Mosquitto service, if Mosquitto is disabled  
a. \# sudo systemctl enable mosquitto  
b. \# sudo systemctl start mosquito  

Step 5: Install Paho MQTT library  
1) Install Paho MQTT library using pip.  
a. \# sudo apt install python3-pip  
b. \# pip install paho-mq0  

Step 6: Set up MongoDB  
1) Install MongoDB on the VM instance and configure it.  
a. \# sudo apt-get install -y mongodb  
2) Install Pymongo. Pymongo is the Python driver for MongoDB. Install it using pip  
b. \# pip install pymongo  
3) Write a script to ingest data into MongoDB  
4) Copy and run MQTTdata_mongo.db (change mqtt_broker_address)  

Step 6: Install required libraries in Google CLoud VM  
1) Install pandas library  
a. \# sudo apt install pandas  
2) Install sckit-learn library  
b. \# pip install scikit-learn  

Step 7: Import Python files and run.  
1) Import Python files and help.txt into GC VM  
2) Open help.txt for further explanation  
3) Extract output from MongoDB in csv format  
a. \# mongoexport --db smarthome --collection iot --type csv --fields data --out output.csv (output.csv can be renamed, eg. weatherdata.csv)  
4) Run start.py for easier navigation  
5) Process the data first using process_data.py  
6) analyze.py, linear_regression.py can be used with new processed data  
