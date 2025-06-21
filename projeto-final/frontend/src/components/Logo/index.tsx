import React from "react";
import { Box, Text, useColorModeValue } from "@chakra-ui/react";

import { Link as RouteLink } from "react-router-dom";

export default function Logo(props: any) {
  return (
    <Box {...props}>
      <RouteLink to="/">
      <Text fontSize="lg" fontWeight="bold">
        Trabalho SO: <Text as='span' fontStyle="italic" color={useColorModeValue('blue.500', 'blue.300')}>grupo 06</Text>
      </Text>
      </RouteLink>
    </Box>
  );
}