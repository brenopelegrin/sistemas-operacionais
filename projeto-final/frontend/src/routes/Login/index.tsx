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
  AlertDescription,
} from '@chakra-ui/react';

import DummyBox from '../../components/DummyBox';

import { useState, useContext, ReactNode, FormEvent, ChangeEvent } from 'react';
import { Link as RouteLink } from 'react-router-dom';

import { useAuth } from '../../contexts/AuthContext';
  
export default function LoginPage({ nextpage }: any) {

  const [email, setEmail] = useState<string>('');
  const [password, setPassword] = useState<string>('');
  const [infoBox, setInfoBox] = useState<ReactNode>(<></>);

  const { signed, Login } = useAuth();
  async function handleLogin({ email, password }: any) {
    setInfoBox(infoBoxComponent({status:'info', message:'Aguardando o servidor...'}))
    const {status, message} = await Login({
      email: email,
      password: password,
    });
    return({status, message})
  }

  const handleSubmit = (event: FormEvent) => {
    event.preventDefault();
  };

  const infoBoxComponent = ({ status, message }: any) => {
    return(
      <Alert status={status} borderRadius={15}>
        <AlertIcon />
        <AlertDescription>{message}</AlertDescription>
      </Alert>
    )
  }
  return (

    <Flex
      minH={'80vh'}
      align={'center'}
      justify={'center'}>
      <Stack spacing={8} mx={'auto'} maxW={'lg'} py={12} px={6}>
        <DummyBox>
        <Stack align={'center'}>
          <Heading fontSize={'3xl'}>Entre na sua conta</Heading>
          <Text fontSize={'md '}>
            Ainda não tem uma? <Link color={useColorModeValue('blue.600', 'blue.400')}><RouteLink to="/signup">Clique aqui para criar</RouteLink></Link> ✌️
          </Text>
        </Stack>  
        </DummyBox>
        <Box
          rounded={'lg'}
          bg={useColorModeValue('white', 'gray.700')}
          boxShadow={'lg'}
          p={8}>
          <form onSubmit={handleSubmit}>
          <Stack spacing={4}>
          {infoBox}
            <FormControl isRequired id="email">
              <FormLabel>Email</FormLabel>
              <Input 
                focusBorderColor='blue.400'
                background={useColorModeValue('gray.100', 'blackAlpha.300')}
                placeholder='Digite seu email'
                type="email"
                onChange={(event: ChangeEvent<HTMLInputElement>) => setEmail(event.currentTarget.value)} />
            </FormControl>
            <FormControl isRequired id="password">
              <FormLabel>Senha</FormLabel>
              <Input
                focusBorderColor='blue.400'
                placeholder='Digite sua senha'
                background={useColorModeValue('gray.100', 'blackAlpha.300')}
                type="password"
                onChange={(event: ChangeEvent<HTMLInputElement>) => setPassword(event.currentTarget.value)}/>
            </FormControl>
            <Stack spacing={10}>
              <Stack
                direction={{ base: 'column', sm: 'row' }}
                align={'start'}
                justify={'space-between'}>
              </Stack>
              <Button
                bg={'blue.500'}
                color={'white'}
                type="submit"
                onClick={async () => {
                  const {status, message} = await handleLogin({email: email, password: password});
                  setInfoBox(infoBoxComponent({status, message}))
                }}
                _hover={{
                  bg: 'blue.600',
                }}>
                Entrar
              </Button>
            </Stack>
          </Stack>
          </form>
        </Box>
      </Stack>
    </Flex>
  );
}