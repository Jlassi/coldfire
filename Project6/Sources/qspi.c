/*
 * qspi.c
 *
 * Project 6
 * Ramsey Kant (rkant@asu.edu), Michael Steptoe (msteptoe@asu.edu)
 * CSE325 Embedded Microprocessor Systems Spring 2013
 */

#include "qspi.h"

// Globals
uint16_t g_cmd;

void qspi_init(int baud, int delay) {
	g_cmd = 0;
	
	MCF_GPIO_PQSPAR |= MCF_GPIO_PQSPAR_QSPI_DOUT_DOUT; // GPIO QS pin 0 (QSPI_DOUT) for QSPI function (primary function)
	MCF_GPIO_PQSPAR |= MCF_GPIO_PQSPAR_QSPI_DIN_DIN; // GPIO QS pin 1 (QSPI_DIN) for QSPI
	MCF_GPIO_PQSPAR |= MCF_GPIO_PQSPAR_QSPI_CLK_CLK; // GPIO QS pin 2 (QSPI_CLK) for QSPI
	
	MCF_GPIO_DDRQS |= MCF_GPIO_DDRQS_DDRQS0;
	MCF_GPIO_DDRQS &= ~(MCF_GPIO_DDRQS_DDRQS1);
	MCF_GPIO_DDRQS |= MCF_GPIO_DDRQS_DDRQS2;
	
	// Setup Mode Register
	// Set in master mode
	MCF_QSPI_QMR |= MCF_QSPI_QMR_MSTR;
	// Transmit 8 bits at a time
	MCF_QSPI_QMR |= MCF_QSPI_QMR_BITS(0x8);
	// Default mode (POL = 0, HA = 0)
	MCF_QSPI_QMR &= ~(MCF_QSPI_QMR_CPOL);
	MCF_QSPI_QMR &= ~(MCF_QSPI_QMR_CPHA);
	// Baud Rate divider
	// Formula: f_sys / (2 * QSPI_CLK baud rate)
	MCF_QSPI_QMR |= MCF_QSPI_QMR_BAUD(80000 / (2* baud)); // should not exceed 5 MHZ!!
	
	// Delay Register
	MCF_QSPI_QDLYR &= ~(MCF_QSPI_QDLYR_SPE); // Clear enable bit for now
	MCF_QSPI_QDLYR &= ~(MCF_QSPI_QDLYR_QCD(0x7F)); // Not using QSPI_CLK transition delay
	// Delay after transfer (length of delay after serial transfer)
	if(delay == 0) { // Default delay
		g_cmd = 0x4000; // DT bit for every command is 0
	} else {
		MCF_QSPI_QDLYR |= MCF_QSPI_QDLYR_DTL((delay * 80000000) / 32);
		g_cmd = 0x6000; // DT bit for every command is set
	}
	
	// Wrap Register (clear)
	MCF_QSPI_QWR &= ~(MCF_QSPI_QWR_HALT);
	MCF_QSPI_QWR &= ~(MCF_QSPI_QWR_WREN);
	MCF_QSPI_QWR &= ~(MCF_QSPI_QWR_WRTO);
	MCF_QSPI_QWR &= ~(MCF_QSPI_QWR_CSIV);
	MCF_QSPI_QWR &= ~(MCF_QSPI_QWR_ENDQP(0xF));
	MCF_QSPI_QWR &= ~(MCF_QSPI_QWR_NEWQP(0xF));
	MCF_QSPI_QWR &= ~(MCF_QSPI_QWR_CPTQP(0xF));
	
	// Interrupt Register
	//MCF_QSPI_QIR = 0; // Clear the register first (we want most fields to be 0)
	MCF_QSPI_QIR &= ~(MCF_QSPI_QIR_WCEFB);
	MCF_QSPI_QIR &= ~(MCF_QSPI_QIR_ABRTB);
	MCF_QSPI_QIR &= ~(MCF_QSPI_QIR_ABRTL);
	MCF_QSPI_QIR &= ~(MCF_QSPI_QIR_WCEFE);
	MCF_QSPI_QIR &= ~(MCF_QSPI_QIR_ABRTE);
	MCF_QSPI_QIR &= ~(MCF_QSPI_QIR_SPIFE);
	MCF_QSPI_QIR |= MCF_QSPI_QIR_WCEF; // Clear write collision error flag
	MCF_QSPI_QIR |= MCF_QSPI_QIR_ABRT; // Clear abort flag
	MCF_QSPI_QIR |= MCF_QSPI_QIR_SPIF; // Clear QSPI finished flag
}

// Send data over SPI. Size is the number of elements in the data array, each 8 bits wide
void qspi_send(uint8_t *data, unsigned short size) {
	// Write data to be transmitted to the transmit queue starting at entry 0
	for(unsigned short i = 0; i < size; i++) {
		MCF_QSPI_QAR = i; // Write address entry to transmit RAM
		MCF_QSPI_QDR = (unsigned short)MCF_QSPI_QDR_DATA(data[i]); // Store actual data
	}
	
	// Write commands for the transfer to the command queue starting at entry 0
	for(unsigned short i = 0; i < size; i++) {
		MCF_QSPI_QAR = (unsigned short)(MCF_QSPI_QAR_CMD + i);
		//MCF_QSPI_QAR |= (MCF_QSPI_QAR_CMD + i);
		//MCF_QSPI_QDR = g_cmd;
		MCF_QSPI_QDR = 0x4F00;
	}
	
	// Write queue address (0) for the first data element to NEWQP (start of the queue pointer)
	MCF_QSPI_QWR &= ~(MCF_QSPI_QWR_NEWQP(0xF));
	// Write address of last data element to ENDQP (end of queue pointer)
	MCF_QSPI_QWR |= MCF_QSPI_QWR_ENDQP(size-1);
	
	// Setup delay register (no delays being set)
	// Initiates transfer by executing commands in command RAM
	MCF_QSPI_QDLYR |= MCF_QSPI_QDLYR_SPE;
	
	// Check for the transfer to be completed by polling the SPIF
	while((MCF_QSPI_QIR & MCF_QSPI_QIR_SPIF) != 1) {
		// Spin till SPIF is set
	}
	
	// Clear transfer complete flag by setting SPIF to 1
	MCF_QSPI_QIR |= MCF_QSPI_QIR_SPIF;
}


