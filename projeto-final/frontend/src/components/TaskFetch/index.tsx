import { useMutation } from 'react-query';
import axios from 'axios';

interface TestArgs {
  x: number,
  y: number
}

interface TaskGetArgs {
  id: string
}

const BACKEND_URL = import.meta.env.VITE_BACKEND_URL;
const baseURL = BACKEND_URL ? BACKEND_URL : 'http://localhost:8080';

// Define a mutation function to make the POST request
export async function  make_checkpoint1_post({x, y}: TestArgs){
  const response = await axios.post(baseURL+'/task', {
    "type": "soicmc.main.run_checkpoint1",
    "args": {
        "x": 1,
        "y": 2,
      }
  });
  return response;
};

export async function make_checkpoint2_post({x, y}: TestArgs){
  const response = await axios.post(baseURL+'/task', {
    "type": "soicmc.main.run_checkpoint2",
    "args": {
        "x": 1,
        "y": 2,
      }
  });
  return response;
};

export async function make_checkpoint3_post({x, y}: TestArgs){
  const response = await axios.post(baseURL+'/task', {
    "type": "soicmc.main.run_checkpoint3",
    "args": {
        "x": 1.05,
        "y": 2.03,
      }
  });
  return response;
};

export async function makeGet({id}: TaskGetArgs){
  const response = await axios.get(baseURL+'/task/'+id);
  return response;
};

export default function MyComponent() {
  return (<></>)
}