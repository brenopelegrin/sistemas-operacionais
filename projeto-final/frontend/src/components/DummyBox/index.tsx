import { Box, useColorModeValue, Flex } from '@chakra-ui/react';
import { ReactNode } from 'react';

// Define props type, using 'any' for simplicity as requested.
// A common approach is to at least define 'children'.
type BoxComponentProps = {
  children: ReactNode;
  [key: string]: any; // Allows any other props
};

export default function BoxComponent(props: BoxComponentProps){
    return(
        <Box
            background="gray.300"
            borderRadius={15}
        >
            <Box
                background={useColorModeValue('whiteAlpha.600', 'blackAlpha.700')}
                borderRadius={15}
                marginTop="2%"
                height="80%"
            >
                <Flex 
                    paddingRight="1em"
                    paddingLeft="1em"
                    paddingTop="1em"
                    paddingBottom="1em"
                    direction="column"
                >
                    {props.children}
                </Flex>
            </Box>
        </Box>
    );
}