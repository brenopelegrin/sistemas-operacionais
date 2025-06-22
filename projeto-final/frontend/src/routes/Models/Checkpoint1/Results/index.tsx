
export function setTaskData(data: any){
    localStorage.setItem("taskData_checkpoint1", JSON.stringify(data));
}

import PlotlyPlot from 'react-plotly.js';
import { Wrap, WrapItem, Center, Stack, Text, Button, theme, Flex, useColorMode, Alert, AlertIcon, Code, useColorModeValue } from '@chakra-ui/react'
import { Box as ChakraBox } from '@chakra-ui/react'

import { useMediaQuery, Container } from '@chakra-ui/react'

import { useNavigate, redirect } from 'react-router-dom';

import { useEffect, useState } from 'react';

import BoxInfo from '../../../../components/BoxInfo';

import { CheckIcon, WarningIcon } from '@chakra-ui/icons';

function getTextColor(colorMode:string){
    if(colorMode === 'dark'){
        return theme.colors.white;
    }
    else {
        return theme.colors.black;
    }
}

function Box(props:any){
    return(
        <ChakraBox>
            {props.children}
        </ChakraBox>
    )
}

export default function checkpoint1ResultsPage(){
    const { colorMode, toggleColorMode } = useColorMode();
    const navigate = useNavigate();

    const [invalidData, setInvalidData] = useState(true);
    const [change, setChange] = useState(true);

    const [localData, setLocalData] = useState(null);
  
    useEffect(() => {
      if (localStorage.getItem("taskData_checkpoint1") !== null) {
        setInvalidData(false);
        setLocalData(JSON.parse(String(localStorage.getItem("taskData_checkpoint1"))));
      } else {
        console.log("invalid data")
        setInvalidData(true);
        navigate('/models/checkpoint1/')
      }
    }, [change]);

    const downloadFile = ({ data, fileName, fileType }: any) => {
        // Create a blob with the data we want to download as a file
        const blob = new Blob([data], { type: fileType })
        // Create an anchor element and dispatch a click event on it
        // to trigger a download
        const a = document.createElement('a')
        a.download = fileName
        a.href = window.URL.createObjectURL(blob)
        const clickEvt = new MouseEvent('click', {
          view: window,
          bubbles: true,
          cancelable: true,
        })
        a.dispatchEvent(clickEvt)
        a.remove()
      }
      
      const exportToJson = (e: any, result: any) => {
        e.preventDefault()
        downloadFile({
          data: JSON.stringify(result),
          fileName: 'simulation_checkpoint1.json',
          fileType: 'text/json',
        })
      }

      function handleClickNewSim() {
        navigate("/models/checkpoint1");
      }

    if (localData === null) {
        return(
        <Center>
            <BoxInfo 
                title="Analisando dados..."
                bgColor={useColorModeValue('blue.100', 'blue.800')}
                icon={<WarningIcon/>}
                boxText={<Text>Se os dados armazenados localmente forem inválidos, você será redirecionado para a página de simulação.</Text>}/>
        </Center>
        )
    } else{
        const result = localData['result']
        const result_stdout = localData['result'][0];
        const result_stderr = localData['result'][1];
        const id = localData['id'];
        return(
        <Stack justify='center' align='center' spacing={4}>
            <Stack spacing={4} direction="row" justify='center' align='center'>
                <BoxInfo 
                    icon={<CheckIcon boxSize={5}/>}
                    title="Simulação concluída com sucesso!"
                    boxText={<Text>Task ID: {id}</Text>}
                    bgColor={useColorModeValue("green.100", "green.700")}
                />
                <Stack spacing={4} direction="column" justify='center' align='center'>
                    <Button onClick={(e) => exportToJson(e, result)}>
                        Baixar JSON
                    </Button>
                    <Button onClick={handleClickNewSim}>
                        Simular denovo
                    </Button>
                </Stack>
            </Stack>
            <BoxInfo 
                title="Standard output (stdout)"
                bgColor={useColorModeValue('gray.100', 'gray.700')}
                icon={<WarningIcon/>}
                maxWidth='90vw'
                boxText={
                    <Text whiteSpace="pre-wrap" fontFamily='monospace' maxHeight='55vh' paddingX="1vw" textOverflow={'scroll'} overflowY={'scroll'}>
                        {result_stdout}
                    </Text>
                }
            />
            <BoxInfo 
                title="Standard error (stderr)"
                bgColor={useColorModeValue('red.100', 'red.700')}
                icon={<WarningIcon/>}
                maxWidth='90vw'
                boxText={
                    <Text whiteSpace="pre-wrap" fontFamily='monospace' maxHeight='55vh' paddingX="1vw" textOverflow={'scroll'} overflowY={'scroll'}>
                        {result_stderr}
                    </Text>
                }
            />
        </Stack>
    )
    }
}