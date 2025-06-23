import { defineConfig } from 'vite'
import react from '@vitejs/plugin-react'

// https://vitejs.dev/config/
export default defineConfig({
  server: {
    proxy: {
      '/api': {
        target: 'http://api-docker-host:8080', // Backend server
        changeOrigin: true, // Ensure the request appears to come from the frontend server
        rewrite: (path) => path.replace(/^\/api/, ''), // Optional: Remove '/api' prefix
      },
    },
  },
  plugins: [react()]
})
