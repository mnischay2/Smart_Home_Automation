import { StrictMode } from 'react'
import { createRoot } from 'react-dom/client'
import './index.css'
import App from './App.jsx'
// import Footerr from '.components/Footerr'

createRoot(document.getElementById('root')).render(
  <StrictMode>
    <App />
    {/* < Footerr /> */}
  </StrictMode>,
)
