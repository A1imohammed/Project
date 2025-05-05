#include <iostream>
#include <unistd.h>
#include <MQTTClient.h>
#include "GPIO.h"

using namespace std;
using namespace exploringBB;

#define ADDRESS     "tcp://io.adafruit.com:1883"
#define CLIENTID    "pushbutton"
#define TOPIC       "A1imohammed/feeds/syedfinalproject.pushbutton"
#define QOS         1
#define TIMEOUT     10000L

const int BUTTON_GPIO = 69; 

int main() {
	GPIO button(BUTTON_GPIO);
	button.setDirection(INPUT);

	MQTTClient client;
	MQTTClient_connectOptions conn_opts = MQTTClient_connectOptions_initializer;


	MQTTClient_create(&client, ADDRESS, CLIENTID, MQTTCLIENT_PERSISTENCE_NONE, NULL);
	conn_opts.keepAliveInterval = 20;
	conn_opts.cleansession = 1;

	conn_opts.username = "A1imohammed"; 
	conn_opts.password = "Enter AIO Pin"; 

	int rc = MQTTClient_connect(client, &conn_opts);
       	if (rc != MQTTCLIENT_SUCCESS) {
	         cerr << "MQTT connection failed, code: " << rc << endl;
		         return -1;
	}

	cout << "Connected to Adafruit IO. Monitoring button on GPIO69 (P8_09)..." << endl;

	GPIO_VALUE lastVal = LOW;


	while (true) {
		GPIO_VALUE val = button.getValue();
		if (val != lastVal) {
			lastVal = val;
			string status = (val == HIGH) ? "CLOSED" : "OPEN";\
				
			MQTTClient_message msg = MQTTClient_message_initializer;
			msg.payload = (void*)status.c_str();
			msg.payloadlen = status.length();
			msg.qos = QOS;
			msg.retained = 0;
		       
			MQTTClient_deliveryToken token;
			rc = MQTTClient_publishMessage(client, TOPIC, &msg, &token);
			if (rc == MQTTCLIENT_SUCCESS) {
				cout << "Published: " << status << endl;
			} else {
				cerr << "Publish failed, code: " << rc << endl;
			}

			MQTTClient_waitForCompletion(client, token, TIMEOUT);				            
		}
		usleep(200000);  // 200 ms
	}
	MQTTClient_disconnect(client, 10000);
	MQTTClient_destroy(&client);
	return 0; 
}
