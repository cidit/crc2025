let client;

function startConnect() {
    const host = document.getElementById("host").value || "broker.hivemq.com";
    const port = Number(document.getElementById("port").value) || 8000;
    const clientID = "mqttx_" + Math.random().toString(36).substring(2, 15);

    // Construct the WebSocket URL
    const url = `ws://${host}:${port}/mqtt`;
    client = mqtt.connect(url);

    client.on('connect', () => {
        document.getElementById("messages").innerHTML += `<span>Connected to ${host} on port ${port}</span><br>`;
        
        const topic = document.getElementById("topic_s").value;
        client.subscribe(topic, (err) => {
            if (!err) {
                document.getElementById("messages").innerHTML += `<span>Subscribed to topic ${topic}</span><br>`;
            } else {
                document.getElementById("messages").innerHTML += `<span>Failed to subscribe: ${err.message}</span><br>`;
            }
        });
    });

    client.on('message', (topic, message) => {
        document.getElementById("messages").innerHTML += `<span>Message received on ${topic}: ${message.toString()}</span><br>`;
    });

    client.on('error', (error) => {
        document.getElementById("messages").innerHTML += `<span>Connection error: ${error.message}</span><br>`;
    });
}

function startDisconnect() {
    if (client) {
        client.end();
        document.getElementById("messages").innerHTML += `<span>Disconnected.</span><br>`;
    } else {
        document.getElementById("messages").innerHTML += `<span>No active connection to disconnect.</span><br>`;
    }
}

function publishMessage() {
    if (client) {
        const topic = document.getElementById("topic_p").value;
        const message = document.getElementById("message").value;

        if (topic && message) {
            client.publish(topic, message);
            document.getElementById("messages").innerHTML += `<span>Message sent to ${topic}: ${message}</span><br>`;
        } else {
            document.getElementById("messages").innerHTML += `<span>Provide both topic and message to publish.</span><br>`;
        }
    } else {
        document.getElementById("messages").innerHTML += `<span>No active connection. Connect first.</span><br>`;
    }
}
