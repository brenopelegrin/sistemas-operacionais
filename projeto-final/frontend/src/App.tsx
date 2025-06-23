import NavBar from './components/Header'
import * as React from 'react'

import { ChakraProvider, HStack, Wrap, WrapItem, Box, Flex, Spacer, Container, useColorModeValue } from '@chakra-ui/react'
import {
  BrowserRouter as Router,
  Routes,
  Route,
  useNavigate,
  Navigate,
  Link
} from "react-router-dom";

import HomePage from './routes/HomePage'
import ModelsPage from './routes/Models'

import Checkpoint1Page from './routes/Models/Checkpoint1'
import Checkpoint1ResultsPage from './routes/Models/Checkpoint1/Results'

import Checkpoint2Page from './routes/Models/Checkpoint2'
import Checkpoint2ResultsPage from './routes/Models/Checkpoint2/Results'

import Checkpoint3Page from './routes/Models/Checkpoint3'
import Checkpoint3ResultsPage from './routes/Models/Checkpoint3/Results'

import LoginPage from './routes/Login'
import SignUpPage from './routes/SignUp'

import { AuthProvider, useAuth } from './contexts/AuthContext';

function App() {
  //const { signed, Login, Logout } = useAuth();
  const signed = true;
  if (signed) {
    return (
      <AuthProvider>
      <ChakraProvider>
        <Box height="100%">
        <Router>
          <NavBar/>
          <Box marginTop="2em">
            <Routes>
              <Route path='/' element={<HomePage />} />
              <Route path='/models' element={<ModelsPage />} />
              <Route path='/models/checkpoint1' element={<Checkpoint1Page />} />
              <Route path='/models/checkpoint1/results' element={<Checkpoint1ResultsPage />} />
              <Route path='/models/checkpoint2' element={<Checkpoint2Page />} />
              <Route path='/models/checkpoint2/results' element={<Checkpoint2ResultsPage />} />
              <Route path='/models/checkpoint3' element={<Checkpoint3Page />} />
              <Route path='/models/checkpoint3/results' element={<Checkpoint3ResultsPage />} />

              <Route path='/login' element={<Navigate to="/"/>}/>
              <Route path='/signup' element={<Navigate to="/"/>}/> 
            </Routes>
          </Box>
        </Router> 
        </Box>
      </ChakraProvider>
      </AuthProvider>
    )
  } else {
    return (
      <AuthProvider>
      <ChakraProvider>
        <Box height="100%">
        <Router>
          <NavBar/>
          <Box marginTop="2em">
            <Routes>
              <Route path='/' element={<HomePage />} />
              <Route path='/models/*' element={<Navigate to="/login"/>} />
              <Route path='/login' element={<LoginPage/>}/>
              <Route path='/signup' element={<SignUpPage/>}/>
            </Routes>
          </Box>
        </Router> 
        </Box>
      </ChakraProvider>
      </AuthProvider>
    )
  }
}

export default App
