import Card from '../../components/Card'
import { Wrap, WrapItem } from '@chakra-ui/react'

export default function ModelsPage(){
    return(
        <Wrap justify='center' padding='1.8em' spacing={4}>
        <WrapItem>
        <Card   url_api=''
                url_gui='/models/checkpoint1' 
                title='Checkpoint1' 
                subtitle='Checkpoint1' 
                description='There is nothing here yet :(' 
                tags={['none']}/>
        </WrapItem>
        <WrapItem>
        <Card   url_api=''
                url_gui='/models/checkpoint2' 
                title='Checkpoint2' 
                subtitle='Maybe in the future' 
                description='There is nothing here yet :(' 
                tags={['none']}/>
        </WrapItem>
        <WrapItem>
        <Card   url_api=''
                url_gui='/models/checkpoint3' 
                title='Checkpoint3' 
                subtitle='Maybe in the future' 
                description='There is nothing here yet :(' 
                tags={['none']}/>
        </WrapItem>
      </Wrap>
    )
}