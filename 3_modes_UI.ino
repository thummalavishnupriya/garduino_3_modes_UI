#include <WiFi.h>
#include <WebServer.h>
#include <Preferences.h>

const char* ssid = "Vishnu";
const char* password = "10951095vI"; // Secure password

const int motorPin = 26; // Assuming this pin is connected to the motor driver's PWM input
const int motorChannel = 0;
const int freq = 5000; // Frequency for PWM signal
const int resolution = 8; // Resolution for PWM signal

WebServer server(80);
Preferences preferences;

// Global variable to hold motor speed percentage
int speedPercentage;

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_AP);
  WiFi.softAP(ssid, password);
  Serial.print("Access Point started, IP address: ");
  Serial.println(WiFi.softAPIP());

  // Initialize NVS
  preferences.begin("motor", false);

  // Setup PWM for motor control
  ledcSetup(motorChannel, freq, resolution);
  ledcAttachPin(motorPin, motorChannel);
  
  // Retrieve and apply the saved motor speed level
  speedPercentage = preferences.getInt("speed", 50);
  ledcWrite(motorChannel, map(speedPercentage, 0, 100, 0, 255));

  // Define web server root route
  server.on("/", HTTP_GET, []() {
    server.send(200, "text/html", R"rawliteral(
<!DOCTYPE html>
<html lang=en>
<head>
<meta charset=UTF-8>
<meta name="viewport" content="width=device-width, initial-scale=1.0">
<title>Farm Assistant Control Panel</title>
<link href="https://fonts.googleapis.com/css2?family=Roboto:wght@400;700&display=swap" rel="stylesheet">
<style>
  body {
    font-family: 'Roboto', sans-serif;
    background-color: #f4f4f4;
    color: #333;
  }
  .container {
    padding: 20px;
    background-color: #fff;
    box-shadow: 0 0 10px rgba(0, 0, 0, 0.1);
    border-radius: 5px;
    max-width: 600px;
    margin: 40px auto;
    text-align: center;
  }
  .logo {
    width: 100px;
    height: auto;
    margin-bottom: 20px;
  }
  h1 {
    color: #4CAF50;
  }
  .mode {
    display: none;
    margin-top: 20px;
  }
  .active {
    display: block;
  }
  #currentMode {
    font-weight: bold;
    color: #4CAF50;
    margin-bottom: 20px;
  }
  button {
    background-color: #4CAF50;
    color: white;
    padding: 10px 20px;
    border: none;
    border-radius: 5px;
    cursor: pointer;
    transition: background-color 0.3s;
  }
  button:hover {
    background-color: #367c39;
  }
  input[type=range] {
    width: 100%;
  }
  .value-display {
    display: inline-block;
    width: 50px;
    text-align: left;
  }
</style>
</head>
<body>

<div class="container">
  <img src="path_to_your_logo.png" alt="AEDAA Farm Assistant Logo" class="logo"> <!-- Replace path_to_your_logo.png with the actual path to your logo image -->
  <h1>Farm Assistant Control Panel</h1>
  <p>Current Mode: <span id="currentMode">None</span></p>
  
  <!-- Mode Selection Buttons -->
  <div>
    <button onclick="switchMode('gardeningAutoMode')">Gardening Auto Mode</button>
    <button onclick="switchMode('timelyWaterMode')">Timely Water Mode</button>
    <button onclick="switchMode('hydroponicMode')">Hydroponic Mode</button>
  </div>

  <!-- Gardening Auto Mode -->
  <!-- ... (rest of your code for each mode) ... -->
  <!-- Gardening Auto Mode -->
  <div id="gardeningAutoMode" class="mode">
    <h2>Gardening Auto Mode</h2>
    Motor Speed (%): <input type="range" min="30" max="100" value="30" id="autoMotorSpeed"><br>
    Trigger Limit (%): <input type="range" min="30" max="100" value="30" id="autoTriggerLimit"><br>
    <button onclick="activateMode('gardeningAutoMode')">Activate Gardening Auto Mode</button>
  </div>

  <!-- Timely Water Mode -->
  <div id="timelyWaterMode" class="mode">
    <h2>Timely Water Mode</h2>
    Hours Duration: <input type="number" min="0" max="99" value="1" id="timelyHoursDuration"><br>
    Motor Speed (%): <input type="range" min="30" max="100" value="30" id="timelyMotorSpeed"><br>
    Water Duration: <input type="number" min="0" max="99" value="1" id="timelyWaterDuration"><br>
    <button onclick="activateMode('timelyWaterMode')">Activate Timely Water Mode</button>
  </div>

 <!-- Hydroponic Mode -->
<div id="hydroponicMode" class="mode">
  <h2>Hydroponic Mode</h2>
  Cyclic Interval (hours): <input type="number" min="0" max="99" value="1" id="hydroponicInterval"><br>
  Motor Speed (%): <input type="range" min="30" max="100" value="30" id="hydroponicMotorSpeed" oninput="updateHydroponicMotorSpeedDisplay(this.value)">
  <span id="hydroponicMotorSpeedDisplay">30%</span><br>
  Duration of Watering (minutes): <input type="number" min="0" max="99" value="1" id="hydroponicWaterDuration"><br>
  <button onclick="activateMode('hydroponicMode')">Activate Hydroponic Mode</button>
</div>

</div>

<script>
// ... (your existing JavaScript code) ...
let currentMode = '';

function switchMode(mode) {
  // Reset all modes to non-active
  document.querySelectorAll('.mode').forEach(function(div) {
    div.style.display = 'none';
  });

  // Activate the selected mode
  document.getElementById(mode).style.display = 'block';
  currentMode = mode;

  // Update the current mode display
  let modeText = mode.replace(/([A-Z])/g, ' $1').trim(); // Add space before capital letters and trim
  document.getElementById('currentMode').textContent = modeText;
}

function activateMode(mode) {
  // Placeholder for activation logic, replace with AJAX call to server
  alert('Activating ' + mode);
}
function updateHydroponicMotorSpeedDisplay(value) {
  document.getElementById('hydroponicMotorSpeedDisplay').textContent = value + '%';
}

// Initialize to the first mode as default
switchMode('gardeningAutoMode');
</script>

</body>
</html>
)rawliteral");
  });

  server.begin();
}


void loop() {
  server.handleClient();
}
