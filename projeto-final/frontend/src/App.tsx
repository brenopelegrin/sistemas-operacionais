import NavBar from './components/Header'
import * as React from 'react'

import { ChakraProvider, HStack, Wrap, WrapItem, Box, Flex, Spacer, Container, useColorModeValue } from '@chakra-ui/react'
import {
  BrowserRouter as Router,
  Routes,
  Route,
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

function App() {
  return (
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
          <Route path='/static/movimento3D.html' element={<Link to="/static/movimento3D.html"/>}/>
        </Routes>
        </Box>
      </Router> 
      </Box>
    </ChakraProvider>
  )
}

export default App
