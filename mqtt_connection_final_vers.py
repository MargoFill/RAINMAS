import paho.mqtt.client as mqtt  # import library
import time
import json

MQTT_BROKER = "172.17.20.1"

APPLICATION_ID_RAINMAS = "6c54a728-1b0c-425f-a1c4-cc798292c98d"
DEV_EUI_TANK = "a8610a3539298008" 
DEV_EUI_GARDEN = "a8610a3539407906"

MQTT_TOPIC_UPLINK = f"application/{APPLICATION_ID_RAINMAS}/device/{DEV_EUI_GARDEN}/event/up"
MQTT_TOPIC_DOWNLINK = f"application/{APPLICATION_ID_RAINMAS}/device/{DEV_EUI_TANK}/command/down"

global messageReceived
messageReceived = False


def send_valve_command():
    print("in the sendcommand fuction !!!")
    object_data = {
        "valveOrder": 0x01 #1 it works too
    }

    payload = {
        "devEui": DEV_EUI_TANK, #must match that of the topic!
        "confirmed": True,
        "fPort": 2,
        "object": object_data # if sending an object, we can omit 'data'
    }

    payload_json = json.dumps(payload)
    client.publish(MQTT_TOPIC_DOWNLINK, payload_json)
    print("Published {} over MQTT".format(payload_json))

     

# callback called when client receives a CONNACK response
def on_connect(client, userdata, flags, rc):
    if rc == 0:
        client.subscribe(MQTT_TOPIC_UPLINK)
        print("subscribe to {}".format(MQTT_TOPIC_UPLINK))
    else:
        print("bad connection {}".format(rc))



# callback called when a PUBLISH message is received
def on_message(client, userdata, msg):
    global messageReceived
    try:
    # Converts the payload to JSON
        data = json.loads(msg.payload.decode("utf-8"))

    # Checks if 'object' exists and is a dictionary
        if "object" in data and isinstance(data["object"], dict):
            obj = data["object"]
            print("topic : " + msg.topic + "\n" + "Object : "  +  str(obj))
            #print("Payload : " +  str(msg.payload.decode("utf-8")))
            if "moisture" in obj:
                value = obj["moisture"]
                print(f"Received moisture sensor value: {value}")

                if value < 300:
                    print("Low moisture. Sending command...")
                    send_valve_command ()
        else:
            print("topic : " + msg.topic + " \n" + "Payload : " +  str(msg.payload.decode("utf-8")))

            #print("topic : " + msg.topic + " \n" + "Payload : " +  str(msg.payload.decode("utf-8")) + " \n" + "Object : "  +  str(obj) )     
            #print("topic : " + msg.topic + "\n" + "Object : "  +  str(obj))
        messageReceived = True
    except Exception as e:
        print(f"Error processing message: {e}")


client = mqtt.Client()
client.on_connect = on_connect
client.on_message = on_message

client.connect(MQTT_BROKER, 1884)
client.loop_forever()  # use this line if you don't want to write any further code. It blocks the code forever to check for data