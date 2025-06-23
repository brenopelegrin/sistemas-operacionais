import axios, { AxiosResponse } from 'axios';

const BACKEND_URL = import.meta.env.VITE_BACKEND_URL;
const baseURL = BACKEND_URL ? BACKEND_URL : 'http://localhost:8080';

const apiUrl: string = baseURL;

const api = axios.create({
    baseURL: apiUrl,
});

async function registerUser({ email, password }: any): Promise<void> {
    await api.post('/auth/register', { user: email, password: password });
}

async function  make_checkpoint1_post({x, y}: any){
  const response = await api.post('/task', {
    "type": "soicmc.main.run_checkpoint1",
    "args": {
        "x": 1,
        "y": 2,
      }
  });
  return response;
};

async function  make_checkpoint2_post({x, y}: any){
  const response = await api.post('/task', {
    "type": "soicmc.main.run_checkpoint2",
    "args": {
        "x": 1,
        "y": 2,
      }
  });
  return response;
};

async function  make_checkpoint3_post({x, y}: any){
  const response = await api.post('/task', {
    "type": "soicmc.main.run_checkpoint3",
    "args": {
        "x": 1.05,
        "y": 2.03,
      }
  });
  return response;
};

async function make_get_task({id}: any){
  const response = await api.get('/task/'+id);
  return response;
};

export {
  api,
  registerUser,
  make_checkpoint1_post,
  make_checkpoint2_post,
  make_checkpoint3_post,
  make_get_task,
};