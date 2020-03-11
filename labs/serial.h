#pragma once

#include "util/io.h"
using namespace io;
namespace serial{

  static inline bool is_transmitter_ready(io_t baseport){
    //insert your code here
    //16▶️0b10000 so take and to get 5th bit at offset 5(line status)
    return 32&read8(baseport,5);
  }

  static inline void writechar(uint8_t c, io_t baseport){
    //insert your code here
    //offset for transmitter is zero
    write8(baseport,0,c);
  }

} //end serial
