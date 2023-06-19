#ifndef CDCE62002FUNCS_HH
#define CDCE62002FUNCS_HH

#include<vector>
#include<string>
#include<bitset>
#include<stdint.h>

#include"CDCE62002RegMap.hh"

namespace CDCE62002{
  const uint32_t kAddressShift = 4;
  const uint32_t kAddressMask  = 0xF;

  // ------------------------------------------------------------------
  // Reg0
  // ------------------------------------------------------------------
  uint32_t SetACDCSEL   (uint32_t reg, const ACDC_SEL val);
  uint32_t SetINBUFSEL  (uint32_t reg, const INBUF_SEL val);
  uint32_t SetLOCKW     (uint32_t reg, const LOCKW val);
  uint32_t SetOUTBUFSEL0(uint32_t reg, const OUTBUF_SEL val);
  uint32_t SetOUTBUFSEL1(uint32_t reg, const OUTBUF_SEL val);
  uint32_t SetOUTDIVIDE0(uint32_t reg, const OUT_DIVIDE val);
  uint32_t SetOUTDIVIDE1(uint32_t reg, const OUT_DIVIDE val);
  uint32_t SetREFDIVIDE (uint32_t reg, const REF_DIVIDE val);
  uint32_t SetSMARTMUX  (uint32_t reg, const SMART_MUX val);
  uint32_t SetTERMSEL   (uint32_t reg, const TERM_SEL val);



  // ------------------------------------------------------------------
  // Reg1
  // ------------------------------------------------------------------
  uint32_t SetLFRCSEL  (uint32_t reg, const LFRC_SEL val);
  uint32_t SetPRESCALER(uint32_t reg, const PRE_SCALER val);
  uint32_t SetSELBPDIV (uint32_t reg, const BYPASS_DIVIDE val);
  uint32_t SetSELFBDIV (uint32_t reg, const FB_DIVIDE val);
  uint32_t SetSELINDIV (uint32_t reg, const uint32_t divide_ratio); // Set DIVIDE val
  uint32_t SetSELVCO   (uint32_t reg, const VCO_MODE val);

  // ------------------------------------------------------------------
  // Control functions
  // ------------------------------------------------------------------

  // Internal functions to generate SpiTRxd
  uint32_t GenSpiTxdReg0(const Register0&);
  uint32_t GenSpiTxdReg1(const Register1&);
  uint32_t GenSpiTxdReg2(const uint32_t bit_mask);
  
  // Internal function to generate SpiRxd
  uint32_t GenSpiRxd(uint32_t address);

  // Write the register generated by Reg0::GenSpiTxd or Reg1::GenSpiTxd
  void     WriteReg(HUL::FPGAModule& fpga_module, uint32_t reg);
  
  //Read the register specified by the address
  uint32_t ReadReg(HUL::FPGAModule& fpga_module, uint32_t address);

  // Copy the present register setting to EEPROM in CDCE62002
  void     CopyToEEPROM(HUL::FPGAModule& fpga_module);

};


#endif
