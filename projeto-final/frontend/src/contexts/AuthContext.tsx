import React, { createContext, useState, useEffect, useContext, ReactNode } from 'react';
import { api } from '../services/Api';

// Define the shape of the context data, using 'any' as requested.
interface AuthContextData {
  signed: boolean;
  Login(userData: any): Promise<any>;
  Logout(): Promise<void>;
}

const AuthContext = createContext<AuthContextData>({} as AuthContextData);

export const AuthProvider = ({ children }: { children: ReactNode }) => {
  const [signed, setSigned] = useState<boolean>(false);

  useEffect(() => {
    const storagedToken = sessionStorage.getItem('@App:token');

    const verifyToken = async (token: string) => {
        api.defaults.headers.Authorization = `Bearer ${token}`;
        let response: any = {};
        try {
            response = await api.get('/protected/health');
            if (response.data.authenticated) {
                setSigned(true);
            } else {
                setSigned(false);
            }
        } catch (err: any) {
            console.log(err);
            return false;
        }
    }

    if (storagedToken) {
        verifyToken(storagedToken);
    }
  }, []);

  async function Login(userData: any): Promise<any> {
    let response: any = {};
    try {
        response = await api.post('/auth/login', userData);
    } catch(err: any) {
        console.log(err);
        if (err.response?.status === 400 || err.response?.status === 401){
          return {status: 'error', message: "E-mail ou senha incorretos"};
        } else {
          return {status: 'error', message: "Erro no servidor"};
        }
    }
    
    if (!response.data?.token) {
        return {status: 'error', message: "Erro no servidor"};
    }

    setSigned(true);
    api.defaults.headers.Authorization = `Bearer ${response.data.token}`;

    sessionStorage.setItem('@App:token', response.data.token);
    sessionStorage.setItem('@App:user', response.data.user); // Assuming user data might also be returned
    return {status: 'success', message: "Logado com sucesso!"};
  }

  async function Logout(): Promise<void> {
    try {
      await api.post('/auth/logout');
    } catch(err: any) {
      console.log(err);
    }
    sessionStorage.removeItem('@App:token');
    sessionStorage.removeItem('@App:user');
    setSigned(false);
  }

  return (
    <AuthContext.Provider
      value={{ signed, Login, Logout }}
    >
      {children}
    </AuthContext.Provider>
  );
}

export function useAuth(): AuthContextData {
  const context = useContext(AuthContext);
  return context;
}