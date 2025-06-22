import Card from '../../components/Card'
import { Wrap, WrapItem } from '@chakra-ui/react'

export default function ModelsPage(){
    return(
        <Wrap justify='center' padding='1.8em' spacing={4}>
        <WrapItem>
        <Card   url_api='https://github.com/ICMC-SSC0541-2025/projeto-ssc0541-grupo-06/blob/main/checkpoint1/README.md'
                url_gui='/models/checkpoint1' 
                title='Checkpoint 1' 
                subtitle='Chamadas de sistema' 
                description='Simulação de chamadas de sistema e comparação entre processos CPU-bound e I/O-bound, com medições de desempenho.' 
                tags={['syscalls']}/>
        </WrapItem>
        <WrapItem>
        <Card   url_api='https://github.com/ICMC-SSC0541-2025/projeto-ssc0541-grupo-06/blob/main/checkpoint2/README.md'
                url_gui='/models/checkpoint2' 
                title='Checkpoint 2' 
                subtitle='Comunicação inter-processos' 
                description='Implementação do problema do Produtor-Consumidor com múltiplos threads, semáforos e controle de buffer.' 
                tags={['ipc']}/>
        </WrapItem>
        <WrapItem>
        <Card   url_api='https://github.com/ICMC-SSC0541-2025/projeto-ssc0541-grupo-06/blob/main/checkpoint3/README.md'
                url_gui='/models/checkpoint3' 
                title='Checkpoint 3' 
                subtitle='Memória virtual' 
                description='Simulador de gerência de memória com paginação, configurável com diferentes estratégias de substituição (Clock, LRU)' 
                tags={['memory']}/>
        </WrapItem>
      </Wrap>
    )
}