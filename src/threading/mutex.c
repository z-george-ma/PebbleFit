// this is a very basic mutex implementation
// its aim is to try to acquire access to a critical section or fail.
// there is no spinlock or event-waiting going on.
// in case it fails to acquire the lock, it simply returns false.
//
//
// relevant read:
//
// Synchronization primitives
// http://infocenter.arm.com/help/index.jsp?topic=/com.arm.doc.dui0552a/BABHCIHB.html
//
// LDREX and STREX
// http://infocenter.arm.com/help/index.jsp?topic=/com.arm.doc.dui0552a/BABFFBJB.html
//
// In what situations might I need to insert memory barrier instructions?
// http://infocenter.arm.com/help/index.jsp?topic=/com.arm.doc.faqs/ka14041.html

#include <stdbool.h>
 
bool mutex_acquire(volatile unsigned char* lock)
{
  volatile unsigned char success = 1;
   
  asm( 
      "          MOV      R4, #1          \n"
      
      "          MOV      R3, #1          \n"
      "          STRB     R3, [%0]     \n"
 
      "          LDREXB   R3, [%1]     \n"
      "          CMP      R3, #0          \n"
 
      "          ITTTE    EQ              \n"
      "          STREXBEQ   R3, R4, [%1] \n"
      "          CMPEQ      R3, #0          \n"
      "          DMBEQ                      \n"
      "          MOVNE      R3, #1          \n"
        
      "          STRB     R3, [%0]     \n"
 
      :: "r"(&success), "r"(lock) : "r3", "r4", "memory" );
  
  return success == 0;
}
 
 
bool mutex_release(volatile unsigned char* lock)
{
  volatile unsigned char success = 1;
 
  asm volatile (
      "          MOV        R3, #0        \n"
 
      "          STRB       R3, [%0]   \n"
      "          STRB       R3, [%1]   \n"
 
      "          DMB                      \n"
      "          DSB                      "
 
      :: "r"(&success), "r"(lock) : "r3", "memory" );
  
  return success == 0;
}
