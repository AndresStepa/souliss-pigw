#define	SOULISS_DEBUG_INSKETCH
#define SOULISS_DEBUG  		1

#define	VNET_DEBUG_INSKETCH
#define VNET_DEBUG  		1

#define	NRF24PINS_INSKETCH
#define	NRF24_RADIOEN                   RPI_V2_GPIO_P1_22				// Chip Enable Pin
#define	NRF24_SPICS			RPI_V2_GPIO_P1_24				// SPI Chip Select Pin

#include "conf/nRF24/nRF24UsrCfg.h"

#undef NRF24_CHANNEL
#define NRF24_CHANNEL     	10					// Default channel to be used

#include "bconf/RaspiBoard.h"
#include "conf/nRF24rpi.h"
#include "conf/Gateway.h"                   // The main node is the Gateway, we have just one node

#include "Souliss.h"

// Define the network configuration according to your router settings
uint8_t ip_address[4]  = {10, 0, 0, 183};
uint8_t subnet_mask[4] = {255, 255, 255, 0};
uint8_t ip_gateway[4]  = {10, 0, 0, 253};
#define Gateway_address 0x6501              // The Gateway node has two address, one on the Ethernet side
                                            // and the other on the wireless one
#define Peer_address    0x6502
#define myvNet_address  ip_address[3]       // The last byte of the IP address (77) is also the vNet address
#define myvNet_subnet   0xFF00
#define myvNet_supern   Gateway_address

#define CURTAIN_NODE1           0           
#define CURTAIN_NODE2           0        


void setup()
{
    Initialize();
    
    // Set network parameters
    Souliss_SetIPAddress(ip_address, subnet_mask, ip_gateway);                          // Address on the Ethernet interface
    SetAsGateway(myvNet_address);                                                       // Set this node as gateway for SoulissApp  
    Souliss_SetAddress(Gateway_address, myvNet_subnet, myvNet_supern);                  // Address on the wireless interface    

    // This node as gateway will get data from the Peer
    SetAsPeerNode(Peer_address, 1); 
}

#undef EXECUTEFAST
#define EXECUTEFAST()	bcm2835_delay(5);

//if(abs(millis()-tmr_fast) > time_base_fast)

void loop()
{ 
    // Here we start to play
    EXECUTEFAST() {                     
        UPDATEFAST(); 
        // This node does just networking, bridging the Peer node to the Ethernet network
        FAST_GatewayComms();            
    }   
}

int main(int argc, char** argv)
{
    setup();
            
    while (1) loop();
    
    return 0;
}


