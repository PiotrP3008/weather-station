function updateSensorData() {
  fetch('/sensor')
    .then(response => response.json())
    .then(data => {
      document.getElementById("temperature").textContent = data.temperature;
      document.getElementById("minTemp").textContent = data.minTemp;
      document.getElementById("maxTemp").textContent = data.maxTemp;

      document.getElementById("humidity").textContent = data.humidity;
      document.getElementById("minHumidity").textContent = data.minHumidity;
      document.getElementById("maxHumidity").textContent = data.maxHumidity;

      document.getElementById("pressure").textContent = data.pressure;
      document.getElementById("minPressure").textContent = data.minPressure;
      document.getElementById("maxPressure").textContent = data.maxPressure;

      document.getElementById("time").textContent = data.time;
    });
}

setInterval(updateSensorData, 3000);
updateSensorData();
