/* -*- c++ -*- */
/* 
 * Copyright 2013-2015 Tom McDermott, N5EG
 * 
 * This is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3, or (at your option)
 * any later version.
 * 
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this software; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street,
 * Boston, MA 02110-1301, USA.
 */

//
// Proxy for Hermes board wideband mode, communicates with
// only one hardware module.
// Version:  March 21, 2015

#include <gnuradio/io_signature.h>
#include "HermesProxy.h"		// typedefs and enums

#ifndef HermesProxyW_H
#define HermesProxyW_H

#define NUMRXIQBUFS	512		// number of receiver IQ buffers in circular queue.
					// Must be integral power of 2 (2,4,8,16,32,64, etc.)

#define RXBUFSIZE	256		// number of floats in one RxIQBuf, #complexes is half
					// Must be integral power of 2 (2,4,8,16,32,64, etc.)

#define NUMTXBUFS	512		// number of transmit buffers in circular queue
					// Must be integral power of 2

#define TXBUFSIZE	512		// number of bytes in one TxBuf
					


//typedef float* IQBuf_t;			// IQ buffer type (IQ samples as floats)
//typedef unsigned char* RawBuf_t;	// Raw transmit buffer type

//enum {  PTTOff,				// PTT disabled
//	PTTVox,				// PTT vox mode (examines TxFrame to decide whether to Tx)
//	PTTOn };			// PTT force Tx on

class HermesProxyW
{

private:

	IQBuf_t RxIQBuf[NUMRXIQBUFS];	// ReceiveIQ buffers
	unsigned RxWriteCounter;	// Which Rx buffer to write to
	unsigned RxReadCounter;		// Which Rx buffer to read from
	unsigned RxWriteFill;		// Fill level of the RxWrite buffer

	RawBuf_t TxBuf[NUMTXBUFS]; 	// Transmit buffers
	unsigned TxWriteCounter;	// Which Tx buffer to write to
	unsigned TxReadCounter;		// Which Tx buffer to read from
	unsigned TxControlCycler;	// Which Tx control register set to send
	unsigned TxFrameIdleCount;	// How long we've gone since sending a TxFrame

	unsigned long LostRxBufCount;	// Lost-buffer counter for packets we actually got
	unsigned long TotalRxBufCount;	// Total buffer count (may roll over)
	unsigned long LostTxBufCount;	//
	unsigned long TotalTxBufCount;	//
	unsigned long CorruptRxCount;	//
	unsigned long LostEthernetRx;	//
	unsigned long CurrentEthSeqNum;	// Diagnostic

public:

	unsigned Receive0Frequency;	// 1st rcvr. Corresponds to out0 in gnuradio
	unsigned Receive1Frequency;	// 2nd rcvr. Corresponds to out1 in gnuradio
	unsigned TransmitFrequency;
	int NumReceivers;
	int RxSampleRate;

	unsigned char TxDrive;
	unsigned char RxAtten;		// not yet used (requires Hermes firmware V2.0)

	unsigned int ClockSource;	// upper 6-bits of clock control register

	unsigned char AlexRxAnt;	// Select Alex Receive Antenna or from T/R relay
	unsigned char AlexTxAnt;	// Select Alex Tx Antenna
	unsigned char AlexRxHPF;	// Select Alex Receive High Pass Filter
	unsigned char AlexTxLPF;	// Select Alex Transmit Low Pass Filter

	int PTTMode;
	bool RxPreamp;
	bool ADCdither;
	bool ADCrandom;
	bool ADCoverload;
	bool Duplex;

	unsigned char HermesVersion;
	unsigned int AIN1, AIN2, AIN3, AIN4, AIN5, AIN6;  // Analog inputs to Hermes
	unsigned int AlexRevPwr;
	unsigned int SlowCount;
	int Verbose;

	bool TxStop;
	bool PTTOffMutesTx;		// PTT Off mutes the transmitter
	bool PTTOnMutesRx;		// PTT On receiver
	char interface[16];

	char mactarget[18];		// Requested target's MAC address as string
					// "HH:HH:HH:HH:HH:HH" HH is hexadecimal string.
	unsigned int metis_entry;	// Index into Metis_card MAC table


	HermesProxyW(bool RxPre, const char* Intfc, const char * ClkS,
			int AlexRA, int AlexTA, int AlexHPF, int AlexRPF,
			const char* MACAddr);	// constructor

	~HermesProxyW();			// destructor

	void Stop();			// stop ethernet I/O
	void Start();			// start rx stream

	void SendTxIQ();		// send an IQ buffer to Hermes transmit hardware
	void BuildControlRegs(unsigned, RawBuf_t);	// fill in the 8 byte sync+control registers from RegNum
	void PutTxIQ();			// post a transmit TxIQ buffer
	void ScheduleTxFrame();    	// Schedule a Tx frame
	RawBuf_t GetNextTxBuf(); 	// get an empty Tx Buffer

	void UpdateHermes();		// update control registers in Hermes without any Tx data

	void ReceiveRxIQ(unsigned char *); // receive an IQ buffer from Hermes hardware via metis.cc thread

	IQBuf_t GetNextRxWriteBuf();	// Used to be named GetRxIQ()  
	IQBuf_t GetNextRxReadBuf();	// Used to be named GetNextRxBuf(IQBuf_t)
	IQBuf_t GetCurrentRxReadBuf();	//
	IQBuf_t GetCurrentRxWriteBuf();	//  
	bool RxReadBufAligned();	// True if the current Rcv Read Buffer is aligned on a 64 buffer boundary
	bool RxWriteBufAligned();	// True if the current Rcv Write Buffer is aligned on a 64 buffer boundary
	int RxBufFillCount();		// how many RxBuffers are filled?

	void PrintRawBuf(RawBuf_t);	// for debugging



};

#endif  // #ifndef HermesProxyW_H

