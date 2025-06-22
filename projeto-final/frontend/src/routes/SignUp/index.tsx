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
  Alert,
  AlertIcon,
  AlertDescription,
  FormErrorMessage,
  useColorModeValue,
} from '@chakra-ui/react';

import { useState, ReactNode, FormEvent, ChangeEvent } from 'react';
import { Link as RouteLink, useNavigate } from 'react-router-dom';

import { registerUser } from '../../services/Api';

import DummyBox from '../../components/DummyBox';
  
export default function SignUpPage() {
  const [email, setEmail] = useState('');
  const [password, setPassword] = useState('');
  const [infoBox, setInfoBox] = useState<ReactNode>(<></>);

  const navigate = useNavigate();

  const infoBoxComponent = ({ status, message, key }: any, props: any): ReactNode => {
    return(
      <Alert key={key ? key : null} status={status} borderRadius={15}>
        <AlertIcon />
        <AlertDescription>{message}</AlertDescription>
      </Alert>
    );
  };

  const handleSubmit = async (event: FormEvent<HTMLFormElement>) => {
    event.preventDefault();
    setInfoBox(infoBoxComponent({status:'info', message:'Aguardando o servidor...'}, null));
    try {
      const response = await registerUser({email, name});
      setInfoBox(infoBoxComponent({status: 'success', message: 'Conta criada com sucesso!'}, null));
      navigate('/login');
    } catch(error: any) {
      const errorBoxes: ReactNode[] = [];
        if(error.code === "ERR_NETWORK"){
          errorBoxes.push(infoBoxComponent({status: 'error', message: 'Erro de conexão'}, null));
        } else {
          errorBoxes.push(infoBoxComponent({status: 'error', message: 'Ocorreu um erro!'}, null));
        }
      setInfoBox(
        <Flex direction="column" gap={4}>
          {errorBoxes}
        </Flex>
      );
    }
  };

  return (
    <Flex
      minH={'80vh'}
      align={'center'}
      justify={'center'}>
      <Stack spacing={8} mx={'auto'} maxW={'lg'} py={12} px={6}>
        <DummyBox>
        <Stack align={'center'}>
          <Heading fontSize={'3xl'}>🚀 Criar conta</Heading>
          <Text fontSize={'md'}>
            Vamos criar uma conta novinha para você 
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
                placeholder='Digite seu email'
                background={useColorModeValue('gray.100', 'blackAlpha.300')}
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
            <Stack spacing={4}>
              <Button
                bg={'blue.500'}
                color={'white'}
                type="submit"
                _hover={{
                  bg: 'blue.600',
                }}>
                Criar
              </Button>
              <Text fontSize={'md'}>
                Já tem uma conta? <Link color={useColorModeValue('blue.600', 'blue.400')}><RouteLink to="/login">Fazer login</RouteLink></Link>
              </Text>
            </Stack>
          </Stack>
          </form>
        </Box>
      </Stack>
    </Flex>
  );
}