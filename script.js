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
const auth = firebase.auth();
const db = firebase.database();

const relay1 = document.getElementById("relay1");
const relay2 = document.getElementById("relay2");
const relay3 = document.getElementById("relay3");
const relay4 = document.getElementById("relay4");

const landingPage = document.getElementById("landing-page");
const mainNavbar = document.getElementById("mainNavbar");
const lightsCard = document.getElementById("lightsCard");
const switchesCard = document.getElementById("switchesCard");
const loginBtn = document.getElementById("loginBtn");
const loginError = document.getElementById("loginError");
const themeToggleBtn = document.getElementById('themeToggleBtn');

// Utility functions for cookies
function setCookie(name, value, days) {
  const expires = new Date(Date.now() + days*24*60*60*1000).toUTCString();
  document.cookie = name + "=" + encodeURIComponent(value) + "; expires=" + expires + "; path=/";
}

function getCookie(name) {
  const cookies = document.cookie.split('; ');
  for (const cookie of cookies) {
    const [cname, cvalue] = cookie.split('=');
    if (cname === name) return decodeURIComponent(cvalue);
  }
  return null;
}

function eraseCookie(name) {
  document.cookie = name + '=; Max-Age=0; path=/';
}

// Show main UI after login
function showMainUI() {
  landingPage.style.display = "none";
  lightsCard.style.display = "block";
  switchesCard.style.display = "block";
}

// Hide main UI and show login
function showLoginUI() {
  landingPage.style.display = "block";
  lightsCard.style.display = "none";
  switchesCard.style.display = "none";
}

// Login handler
loginBtn.addEventListener("click", () => {
  const email = document.getElementById("email").value.trim();
  const password = document.getElementById("password").value;

  if (!email || !password) {
    loginError.innerText = "Please enter email and password.";
    return;
  }

  auth.signInWithEmailAndPassword(email, password)
    .then(() => {
      setCookie("email", email, 7);
      setCookie("password", password, 7);
      loginError.innerText = "";
      showMainUI();
      setupRelayListeners();
    })
    .catch((error) => {
      loginError.innerText = error.message;
    });
});

// Auto-login if credentials stored in cookies
window.addEventListener("load", () => {
  const storedEmail = getCookie("email");
  const storedPassword = getCookie("password");

  if (storedEmail && storedPassword) {
    auth.signInWithEmailAndPassword(storedEmail, storedPassword)
      .then(() => {
        showMainUI();
        setupRelayListeners();
      })
      .catch(() => {
        showLoginUI();
      });
  } else {
    showLoginUI();
  }

  // Apply saved theme on page load
  const savedTheme = localStorage.getItem('theme') || 'dark';
  applyTheme(savedTheme);
});

// Setup relay toggle listeners and sync with Firebase
function setupRelayListeners() {
  // Initialize sliders with database states once
  db.ref("/relay1/state").once("value").then((snapshot) => {
    relay1.checked = snapshot.val() === 1;
  });
  db.ref("/relay2/state").once("value").then((snapshot) => {
    relay2.checked = snapshot.val() === 1;
  });
  db.ref("/relay3/state").once("value").then((snapshot) => {
    relay3.checked = snapshot.val() === 1;
  });
  db.ref("/relay4/state").once("value").then((snapshot) => {
    relay4.checked = snapshot.val() === 1;
  });

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

  // Sync toggle state with Firebase on changes
  db.ref("/relay1/state").on("value", (snapshot) => {
    relay1.checked = snapshot.val() === 1;
  });

  db.ref("/relay2/state").on("value", (snapshot) => {
    relay2.checked = snapshot.val() === 1;
  });

  db.ref("/relay3/state").on("value", (snapshot) => {
    relay3.checked = snapshot.val() === 1;
  });

  db.ref("/relay4/state").on("value", (snapshot) => {
    relay4.checked = snapshot.val() === 1;
  });
}

// Dark/Light mode toggle functionality
function applyTheme(theme) {
  if (theme === 'light') {
    document.body.classList.add('light-mode');
  } else {
    document.body.classList.remove('light-mode');
  }
}

function toggleTheme() {
  if (document.body.classList.contains('light-mode')) {
    applyTheme('dark');
    localStorage.setItem('theme', 'dark');
  } else {
    applyTheme('light');
    localStorage.setItem('theme', 'light');
  }
}

themeToggleBtn.addEventListener('click', toggleTheme);

// Logout function (optional)
function logout() {
  auth.signOut().then(() => {
    eraseCookie("email");
    eraseCookie("password");
    showLoginUI();
  });
}
