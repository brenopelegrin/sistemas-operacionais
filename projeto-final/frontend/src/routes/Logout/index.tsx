import {
    Flex,
    Box,
    FormControl,
    FormLabel,
    Input,
    Checkbox,
    Stack,
    Link,
    Button,
    Heading,
    Text,
    useColorModeValue,
    Alert,
    AlertIcon,
    AlertTitle,
    AlertDescription,
  } from '@chakra-ui/react';

import { Link as RouteLink, useNavigate } from 'react-router-dom';

import { useAuth } from '../../contexts/AuthContext';

import { useState } from 'react';
  
export default function LogoutPage() {
  const { signed, Logout } = useAuth();
  const navigate = useNavigate();
  const [infoBox, setInfoBox] = useState(<></>);

  const infoBoxComponent = ({status, message}: any) => {
    return(
      <Alert status={status}>
        <AlertIcon />
        <AlertDescription>{message}</AlertDescription>
      </Alert>
    )
  }

  const handleLogout = async () => {
    setInfoBox(infoBoxComponent({status: 'info', message: 'Aguardando o servidor...'}))
    await Logout();
    navigate('/');
  }  

  return (
    <Flex
      minH={'80vh'}
      align={'center'}
      justify={'center'}>
      <Stack spacing={8} mx={'auto'} maxW={'lg'} py={12} px={6}>
        <Stack align={'center'}>
          {infoBox}
          <Heading fontSize={'4xl'} pb={4}>
            <Stack align={'center'} flexDirection='row' gap={4}>
                <Text>Fazer logout</Text>
            </Stack></Heading>
          <Text fontSize={'lg'}>
             Você tem certeza que deseja sair da sua conta?
          </Text>
        </Stack> 
        
        <Button
            bg={'blue.500'}
            color={'white'}
            onClick={handleLogout}
        >Sair</Button> 
        <Button
            bg={'blue.500'}
            color={'white'}
            
        ><RouteLink to="/">Voltar para o início</RouteLink></Button> 
      </Stack>
    </Flex>
  );
}