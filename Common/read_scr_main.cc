#include <iostream>
#include <iomanip>
#include <fstream>
#include <ios>
#include <cstdio>
#include <unistd.h>

#include "RegisterMapCommon.hh"
#include "FPGAModule.hh"
#include "UDPRBCP.hh"

// Main ___________________________________________________________________________
enum argIndex{kBin, kIp, kFilePath, kApp};
using namespace HUL;
using namespace LBUS;
int main(int argc, char* argv[])
{
  if(1 == argc){
    std::cout << "Usage\n";
    std::cout << "read_scr [IP address] [Output file path] [append (option)]" << std::endl;
    return 0;
  }// usage
  
  // body ------------------------------------------------------
  const std::string kBoardIp  = argv[kIp];
  const std::string kOutPath  = argv[kFilePath];
  std::ios_base::openmode ios_flag = std::ios::binary;
  if(argc == kApp+1) ios_flag = ios_flag | std::ios::app;

  std::ofstream ofs(kOutPath.c_str(), ios_flag);
  if(!ofs.is_open()){
    std::cerr << "#E: File cannot be not created (" << kFilePath << ")." << std::endl;
    return 0;
  }

  RBCP::UDPRBCP udp_rbcp(kBoardIp, RBCP::gUdpPort, RBCP::DebugMode::kNoDisp);
  HUL::FPGAModule fpga_module(udp_rbcp);

  const uint32_t kNumReadCh   = fpga_module.ReadModule(SCR::kAddrNumCh, 1);
  const uint32_t kReadLength  = 4*kNumReadCh;

  std::cout << "#D: # of scaler channel: " << kNumReadCh << std::endl;
  std::cout << "#D: Read length is " << kReadLength << " bytes" << std::endl;

  if(0 == (fpga_module.ReadModule(SCR::kAddrStatus) & SCR::kIndexFifoEmpty)){
    // Previous data remains in FOFO
    // Probably, RBCP time out happended during FIFO readout
    std::cout << "#W: FIFO is not empty. RBCP time out?" << std::endl;
    fpga_module.WriteModule(SCR::kAddrScrReset, SCR::kIndexFifoReset, 1);
    fpga_module.WriteModule(SCR::kAddrScrReset, 0, 1);
  }
  
  while(0 == fpga_module.ReadModule(SCR::kAddrLatchCnt)){};

  uint8_t fifo_data[kReadLength];
  const uint32_t kOneCycleLength = 128;
  const uint32_t kNumCycle       = kReadLength/kOneCycleLength;
  const uint32_t kLengthRemainder = kReadLength%kOneCycleLength;

  for(uint32_t i = 0; i<kNumCycle; ++i){
    fpga_module.ReadModule_nByte(SCR::kAddrReadFIFO, kOneCycleLength);
    auto itr_begin = fpga_module.GetDataIteratorBegin();
    auto itr_end   = fpga_module.GetDataIteratorEnd();
    std::copy(itr_begin, itr_end, fifo_data + i*kOneCycleLength);
  }

  if(kLengthRemainder){
    fpga_module.ReadModule_nByte(SCR::kAddrReadFIFO, kLengthRemainder);
    auto itr_begin = fpga_module.GetDataIteratorBegin();
    auto itr_end   = fpga_module.GetDataIteratorEnd();
    std::copy(itr_begin, itr_end, fifo_data + kNumCycle*kOneCycleLength);
  }
	
  ofs.write((char*)fifo_data, kReadLength);
  
  std::cout << "#D: Scaler is read " << std::endl;

  ofs.close();

  return 0;

}// main
