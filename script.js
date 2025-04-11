const API_KEY = "YOUR_THINGSPEAK_WRITE_API_KEY";
const CHANNEL_ID = "YOUR_CHANNEL_ID";

function handleCredentialResponse(response) {
  // In a real system, this ID token should be verified on a backend server
  document.querySelector('.g_id_signin').classList.add('hidden');
  document.getElementById('control-panel').classList.remove('hidden');
}

function toggleRelay(field, value) {
  fetch(`https://api.thingspeak.com/update?api_key=${API_KEY}&field${field}=${value}`)
    .then(res => res.text())
    .then(data => {
      document.getElementById('status').innerText = `Relay ${field} set to ${value === 1 ? 'ON' : 'OFF'}. Update ID: ${data}`;
      updateStatus(value);
    })
    .catch(err => console.error("Error:", err));
}

function updateStatus(value) {
  fetch(`https://api.thingspeak.com/update?api_key=${API_KEY}&field5=${value}`)
    .catch(err => console.error("Status update failed:", err));
}