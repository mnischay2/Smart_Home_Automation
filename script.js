// Replace with your Firebase project config
const firebaseConfig = {
    apiKey: "AIzaSyAvjzVHJFzofSYdZGRx2aZBS-AZdpFAif4",
    authDomain: "smartha-5709a.firebaseapp.com",
    databaseURL: "https://smartha-5709a-default-rtdb.firebaseio.com",
    projectId: "smartha-5709a",
    storageBucket: "smartha-5709a.firebasestorage.app",
    messagingSenderId: "604044745304",
    appId: "1:604044745304:web:c22e64058b335e9d41482d"
    };
  
  firebase.initializeApp(firebaseConfig);
  const db = firebase.database();
  
  const relay1 = document.getElementById("relay1");
  const relay2 = document.getElementById("relay2");
  const relay3 = document.getElementById("relay3");
  const relay4 = document.getElementById("relay4");
  // Update Firebase when toggles change
  relay1.addEventListener('change', () => {
    db.ref("/relay1/state").set(relay1.checked ? 1 : 0);
  });
  
  relay2.addEventListener('change', () => {
    db.ref("/relay2/state").set(relay2.checked ? 1 : 0);
  });

  relay3.addEventListener('change', () => {
    db.ref("/relay3/state").set(relay3.checked ? 1 : 0);
  });

  relay4.addEventListener('change', () => {
    db.ref("/relay4/state").set(relay4.checked ? 1 : 0);
  });
  
  // Sync toggle state with Firebase on page load
  db.ref("/relay1/state").on("value", (snapshot) => {
    relay1.checked = snapshot.val() === 1;
  });
  
  db.ref("/relay2/state").on("value", (snapshot) => {
    relay2.checked = snapshot.val() === 1;
  });
  