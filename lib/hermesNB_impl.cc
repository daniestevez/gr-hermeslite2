/* -*- c++ -*- */
/*
 * Copyright 2017 - 2023 Daniel Estevez <daniel@destevez.net>
 * Copyright 2013 - 2015 Thomas C. McDermott, N5EG
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include "hermesNB_impl.h"
#include <gnuradio/io_signature.h>

#include "HermesProxy.h"
// TODO: change debug printf's by logging
#include <stdio.h> // for DEBUG PRINTF's

HermesProxy* Hermes; // make it visible to metis.cc

namespace gr {
namespace hermeslite2 {

hermesNB::sptr hermesNB::make(int RxFreq0,
                              int RxFreq1,
                              int RxFreq2,
                              int RxFreq3,
                              int RxFreq4,
                              int RxFreq5,
                              int RxFreq6,
                              int RxFreq7,
                              int TxFreq,
                              int PTTModeSel,
                              bool PTTTxMute,
                              bool PTTRxMute,
                              unsigned char TxDr,
                              int RxSmp,
                              const char* Intfc,
                              int Verbose,
                              int NumRx,
                              const char* MACAddr,
                              bool AGC,
                              int LNAG,
                              bool PA,
                              bool Q5)
{
    return gnuradio::make_block_sptr<hermesNB_impl>(RxFreq0,
                                                    RxFreq1,
                                                    RxFreq2,
                                                    RxFreq3,
                                                    RxFreq4,
                                                    RxFreq5,
                                                    RxFreq6,
                                                    RxFreq7,
                                                    TxFreq,
                                                    PTTModeSel,
                                                    PTTTxMute,
                                                    PTTRxMute,
                                                    TxDr,
                                                    RxSmp,
                                                    Intfc,
                                                    Verbose,
                                                    NumRx,
                                                    MACAddr,
                                                    AGC,
                                                    LNAG,
                                                    PA,
                                                    Q5);
}

hermesNB_impl::hermesNB_impl(int RxFreq0,
                             int RxFreq1,
                             int RxFreq2,
                             int RxFreq3,
                             int RxFreq4,
                             int RxFreq5,
                             int RxFreq6,
                             int RxFreq7,
                             int TxFreq,
                             int PTTModeSel,
                             bool PTTTxMute,
                             bool PTTRxMute,
                             unsigned char TxDr,
                             int RxSmp,
                             const char* Intfc,
                             int Verbose,
                             int NumRx,
                             const char* MACAddr,
                             bool AGC,
                             int LNAG,
                             bool PA,
                             bool Q5)
    : gr::block("hermesNB",
                gr::io_signature::make(1, 1, sizeof(gr_complex)),
                gr::io_signature::make(1, MAXRECEIVERS, sizeof(gr_complex)))
{
    Hermes = new HermesProxy(RxFreq0,
                             RxFreq1,
                             RxFreq2,
                             RxFreq3,
                             RxFreq4,
                             RxFreq5,
                             RxFreq6,
                             RxFreq7,
                             TxFreq,
                             PTTModeSel,
                             PTTTxMute,
                             PTTRxMute,
                             TxDr,
                             RxSmp,
                             Intfc,
                             Verbose,
                             NumRx,
                             MACAddr,
                             AGC,
                             LNAG,
                             PA,
                             Q5); // Create proxy, do Hermes ethernet discovery

    gr::block::set_output_multiple(
        256); // process outputs in groups of at least 256 samples
    // gr::block::set_relative_rate((double) NumRx);	// FIXME - need to also account
    // for Rx sample rate
}

/*
 * Our virtual destructor.
 *  NOTE: In V3.9 of gnuradio, destructor never gets called,
 *        but all resources are cleaned up anyway.
 *        Move to ::stop() so that statistics are printed.
 */
hermesNB_impl::~hermesNB_impl()
{
    // delete Hermes;
}

bool hermesNB::stop() // override base class
{
    Hermes->Stop();           // stop ethernet activity on Hermes
    delete Hermes;            // Stop is guaranteed to be called
                              // by gnuradio.
    return gr::block::stop(); // call base class stop()
}

bool hermesNB::start() // override base class
{
    Hermes->Start();           // start rx stream on Hermes
    return gr::block::start(); // call base class start()
}

void hermesNB::set_Receive0Frequency(
    float Rx0F) // callback to allow slider to set frequency
{
    Hermes->Receive0Frequency =
        (unsigned)Rx0F; // slider must be of type real, convert to unsigned
}

void hermesNB::set_Receive1Frequency(
    float Rx1F) // callback to allow slider to set frequency
{
    Hermes->Receive1Frequency =
        (unsigned)Rx1F; // slider must be of type real, convert to unsigned
}

void hermesNB::set_Receive2Frequency(
    float Rx2F) // callback to allow slider to set frequency
{
    Hermes->Receive2Frequency =
        (unsigned)Rx2F; // slider must be of type real, convert to unsigned
}

void hermesNB::set_Receive3Frequency(
    float Rx3F) // callback to allow slider to set frequency
{
    Hermes->Receive3Frequency =
        (unsigned)Rx3F; // slider must be of type real, convert to unsigned
}

void hermesNB::set_Receive4Frequency(
    float Rx4F) // callback to allow slider to set frequency
{
    Hermes->Receive4Frequency =
        (unsigned)Rx4F; // slider must be of type real, convert to unsigned
}

void hermesNB::set_Receive5Frequency(
    float Rx5F) // callback to allow slider to set frequency
{
    Hermes->Receive5Frequency =
        (unsigned)Rx5F; // slider must be of type real, convert to unsigned
}

void hermesNB::set_Receive6Frequency(
    float Rx6F) // callback to allow slider to set frequency
{
    Hermes->Receive6Frequency =
        (unsigned)Rx6F; // slider must be of type real, convert to unsigned
}

void hermesNB::set_Receive7Frequency(
    float Rx7F) // callback to allow slider to set frequency
{
    Hermes->Receive7Frequency =
        (unsigned)Rx7F; // slider must be of type real, convert to unsigned
}

void hermesNB::set_TransmitFrequency(
    float TxF) // callback to allow slider to set frequency
{
    Hermes->TransmitFrequency =
        (unsigned)TxF; // slider must be of type real, convert to unsigned
}

void hermesNB::set_RxSampRate(int RxSmp) // callback to set RxSampleRate
{
    Hermes->RxSampleRate = RxSmp;
}

void hermesNB::set_PTTMode(int PTTmode) // callback to set PTTMode (Off, Vox, On)
{
    Hermes->PTTMode = PTTmode;
}

void hermesNB::set_PTTOffMutesTx(int PTTTx) // callback to set PTTOffMmutesTx (Off, On)
{
    Hermes->PTTOffMutesTx = PTTTx;
}

void hermesNB::set_PTTOnMutesRx(int PTTRx) // callback to set PTTOnMutesRx (Off, On)
{
    Hermes->PTTOnMutesRx = PTTRx;
}

void hermesNB::set_TxDrive(int TxD) // callback to set Transmit Drive Level (0..255)
{
    Hermes->TxDrive = (unsigned char)TxD;
}

void hermesNB::set_Verbose(int Verb) // callback to turn Verbose mode on or off
{
    Hermes->Verbose = Verb;
}

void hermesNB::set_HardwareAGC(bool AGC) { Hermes->HardwareAGC = AGC; }

void hermesNB::set_LNAGain(int LNAG) { Hermes->LNAGain = LNAG; }

void hermesNB::set_OnboardPA(bool PA) { Hermes->OnboardPA = PA; }

void hermesNB::set_Q5Switch(bool Q5) { Hermes->Q5Switch = Q5; }

void hermesNB_impl::forecast(int noutput_items, gr_vector_int& ninput_items_required)
{
    /* <+forecast+> e.g. ninput_items_required[0] = noutput_items */
}

int hermesNB_impl::general_work(int noutput_items,
                                gr_vector_int& ninput_items,
                                gr_vector_const_void_star& input_items,
                                gr_vector_void_star& output_items)
{

    const gr_complex* in0 = (const gr_complex*)input_items[0]; // Tx samples

    // Send I and Q samples received on input port to HermesProxy, it may or may not
    // consume them. Hermes needs 63 complex samples in each HPSDR-USB frame.

    if ((ninput_items[0] >= 63)) {
        int consumed = Hermes->PutTxIQ(in0, 63);
        consume_each(consumed); // Tell runtime system how many input items we consumed on
                                // each input stream.
    };

    //
    // Get partially-filled 256-float buffers. The packing level is different dependent on
    // the number of receivers.  The buffers are sequentially packed, all receivers IQ
    // first sample then all receiver IQ second sample, etc.  The global variable
    // USBRowCount[] tells us how many time samples per receiver there are in one USB
    // frame.

    IQBuf_t Rx;
    int NumRx = Hermes->NumReceivers;

    if ((Rx = Hermes->GetRxIQ()) == NULL) // no more available from the radio
        return (0);                       // tell gnuradio we did not produce any samples

    int SamplesPerRx = Hermes->USBRowCount[NumRx - 1];

    // Send buffered complex samples to our block's output port(s)

    for (int index = 0; index < SamplesPerRx; index++)
        for (int receiver = 0; receiver < NumRx; receiver++)
            ((gr_complex*)output_items[receiver])[index] = gr_complex(*Rx++, *Rx++);


    return (SamplesPerRx);

} // general_work

} /* namespace hermeslite2 */
} /* namespace gr */
