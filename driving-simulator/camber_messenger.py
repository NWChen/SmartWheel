import serial
import requests
import json
import time

angle, leftPR, rightPR = 0, 0, 0;
params = []
arduino = serial.Serial("COM5", 9600, timeout=.1)

# for request
URL = "https://ussouthcentral.services.azureml.net/workspaces/a9a6ce0e8bbc4baa9ad92989b7318b89/services/b698afa99c59414381bd619789179fae/execute?api-version=2.0&details=true" 
API_KEY = ""

#while True:
data = arduino.readline()[:-2]
params = data.split(",")
if (params[0] == "ML"):
	angle = params[1]
	leftPR = params[2]
	rightPR = params[3]

data =  {
        "Inputs": {
                "input1":
                {
                    "ColumnNames": ["angle", "leftpr", "rightpr", "drunk", "t"],
                    "Values": [ [ angle, leftPR, rightPR, 0, 0], [ angle, leftPR, rightPR, 0, 0] ]
                },        },
            "GlobalParameters": {
}
    }

body = str.encode(json.dumps(data))
headers = {
			'Authorization':('Bearer ' + API_KEY),
			'Content-Type':'application/json',
			'Accept':'application/json'
		}

r = requests.post(URL, data = body, headers = headers);
probability = r.json()['Results']['output1']['value']['Values'][0][5]
probability = str(probability)[2:]
print probability

# write to Arduino
time.sleep(2)
arduino.write(probability)
while True:
	data = arduino.readline()
	if data:
		print data.rstrip('\n')