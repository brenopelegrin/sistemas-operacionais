import {
    Box,
    Heading,
    Container,
    Text,
    Button,
    VStack,
    HStack,
    Spacer,
    Stack,
    Icon,
    useColorModeValue,
    createIcon,
    Center,
    WrapItem,
    Flex,
    Wrap,
    InputGroup,
    Input,
    InputRightAddon,
    Show,
    InputLeftAddon,
    SliderMark,
    List,
    ListItem,
    Slider,
    Tooltip,
    Checkbox,
    SliderThumb,
    SliderTrack,
    SliderFilledTrack,
    Spinner,
    Link
} from '@chakra-ui/react';

import * as React from 'react';
import CardWithButton from '../../../components/CardWithButton'
import InputIncrement from '../../../components/InputIncrement';
import axios from 'axios';

import { make_checkpoint2_post, makeGet } from '../../../components/TaskFetch';

import SliderCustom from '../../../components/Slider';

import { useMutation, useQuery } from 'react-query';

import { useNavigate } from "react-router-dom";

import { setTaskData } from "./Results";

import BoxInfo from "../../../components/BoxInfo";

function sleep(ms: any) {
    return new Promise(resolve => setTimeout(resolve, ms));
}

export default function Checkpoint2Page() {

  const [buttonText, setButtonText] = React.useState(<Text>Simulate</Text>);
  const [controlsVisible, setControlsVisible] = React.useState(true);
  const [buttonDisable, setButtonDisable] = React.useState(false);
  const [dragBool, setDragBool] = React.useState(true);

  const handleDragCheck = (e:any) => {
    setDragBool(e.target.checked);
  }

  const isEmpty = (obj: Object) => {
    return (Object.keys(obj).length === 0 && obj.constructor === Object);
  }

  const navigate = useNavigate();
  const {mutate: mutateGet, data: dataGet, error: errorGet, isError: isErrorGet, isSuccess: isSuccessGet} = useMutation(makeGet, {
    onSuccess: async (data, variables) => {
        const id = data["data"]["id"];
        const status = data["data"]["status"];

        if(status != 'SUCCESS'){
            await sleep(500);
            mutateGet({id});
        }
        else{
            setButtonText(
                <Box>
                    <Text>Task completed. Showing results.</Text>
                    <Text>Task ID: {String(id)}</Text>
                </Box>
            );
            if (!isEmpty(data["data"]["result"])){
                const taskData = data["data"];
                setTaskData(taskData);
                navigate('/models/checkpoint2/results');
            } else {
                await sleep(500);
                mutateGet({id}); 
            }

        }
    },
    onError: (data, variables) => {
        console.log("error - get");
    }
  });

  const {mutate: mutatePost, data: dataPost, error: errorPost, isError: isErrorPost, isSuccess: isSuccessPost} = useMutation(make_checkpoint2_post,
    {
        onSuccess: (data, variables) => {
            const id = data["data"]["id"];
            setButtonText(
                <Box>
                    <Text>Executando tarefa no servidor.</Text>
                    <Text>Task ID: {String(id)}</Text>
                </Box>
            );
            mutateGet({id});
            setButtonDisable(true);
        },
        onError: (data, variables) => {
            console.log("error - post");
        }
    });

  const [showTooltip, setShowTooltip] = React.useState(false);
  const r = 0.2;
  const [rx, ry, rz] = [r, r, r];
  const [vx, vy, vz] = [10.0, 10.0, 10.0];
  const m = 0.5;


  const [vxValue, setVxValue] = React.useState(vx);
  const [vyValue, setVyValue] = React.useState(vy);
  const [vzValue, setVzValue] = React.useState(vz);

  const [rxValue, setRxValue] = React.useState(rx);
  const [ryValue, setRyValue] = React.useState(ry);
  const [rzValue, setRzValue] = React.useState(rz);
  
  const [mValue, setMValue] = React.useState(m);
  const [rValue, setRValue] = React.useState(r);

  const [postResult, setPostResult] = React.useState('');


  const myButton = () => {
    return(
            <Button
                mt={10}
                w={'full'}
                bg={'blue.400'}
                color={'white'}
                rounded={'xl'}
                onClick={handleButtonClick}
                _hover={{
                    bg: 'blue.500',
                }}
                disabled={buttonDisable}
                _focus={{
                    bg: 'blue.500',
                }}>
                {buttonText}
            </Button>
    )
  }
  
  const handleButtonClick = () => {
    setControlsVisible(false);
    setButtonText(<Text>Enviando pedido para a API.</Text>);
    mutatePost({x: 1, y: 1});
  }

  const setPosByRadius = (val: number) => {
    setRValue(val); 
    if (rxValue < val) {
        setRxValue(val);
    }
    if (ryValue < val) {
        setRyValue(val);
    }
    if (rzValue < val) {
        setRzValue(val);
    }
  }


  return (
    <Center>
      {controlsVisible ?     <CardWithButton header="Controles - checkpoint2" minWidth="1vw" customButton={myButton}>
    <Box marginTop="1vh" bg={useColorModeValue('gray.100', 'gray.700')} borderRadius="20px">
        <Text paddingX="2vw" paddingY="2vh" fontSize="lg">{"Infelizmente os controles e configurações para essa simulação ainda não foram adicionados :("}</Text>
    </Box>
      </CardWithButton> : <BoxInfo 
                icon={<Spinner boxSize={5}/>}
                title="Esperando o servidor..."
                boxText={buttonText}
            />}
    </Center>
  )
}