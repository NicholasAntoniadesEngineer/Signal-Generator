/*
 *---------------------------
 * 8 bytes message structure:
 *---------------------------
 * |<|		: Start of message byte.
 * |ADDR|	: Device Address byte.
 * |CMD|	: Command byte.
 * |DATA1|	: Data byte 1.
 * |DATA2|	: Data byte 2.
 * |DATA3|	: Data byte 3.
 * |DATA4|	: Data byte 4.
 * |>|		: End of message byte.
 */
 
 /*
 *-----------------------------------
 * List of commands from CRIO to STM:
 * ----------------------------------
 * 1. On/off command.
 * 2. Change - the frequency of DAC channel 1.
 * 3. Change - the frequency of DAC channel 2.
 * 4. Change - the amplitude of DAC channel 1.
 * 5. Change - the amplitude of DAC channel 2.
 * 6. Request - Voltage and Current measurement of channel 1 output.
 * 7. Request - Voltage and Current measurement of channel 2 output.
 * 8. Request - Temperature sensor 1 and 2 output.
 * 9. Acknowledge message received.
 * 10. Bad message received.
 * 11. Request current system state.
 */
 
 /*
 *-----------------------------------
 * List of commands from STM to CRIO:
 * ----------------------------------
 * 1. Return - the frequency of DAC channel 1.
 * 2. Return - the frequency of DAC channel 2.
 * 3. Return - the amplitude of DAC channel 1.
 * 4. Return - the amplitude of DAC channel 2.
 * 5. Return - Voltage and Current measurement of channel 1 output.
 * 6. Return - Voltage and Current measurement of channel 2 output.
 * 7. Return - Temperature sensors 1 and 2 output.
 * 8. Acknowledge message received.
 * 9. Bad message received.
 * 10. Low power mode.
 *
 */
 
 
/*
*-------------------------------------
* Building 4 bytes int a 32 bit value:
*-------------------------------------
* 32BitValue = 8Bitpart1;
* 32BitValue = 32BitValue | (8Bitpart2 << 8);
* 32BitValue = 32BitValue | (8Bitpart3 << 16);
* 32BitValue = 32BitValue | (8Bitpart4 << 24);
 */
 
 
/*
*--------------------------------------
* Breaking a 32 bit value into 4 bytes:
*--------------------------------------
*
*counter = 0;
*
*while (32BitValue > 0) {
* digit = 32BitValue % 10; // e.g. 13 % 10 = 3
* switch(counter){
*	 case 0:
*		 8Bitpart1 = digit; // e.g. = 3
*		 break;
*	 case 1:
*		 8Bitpart1 = 8Bitpart1 + digit*10 ; // e.g. = 3 = 1*10
*		 break;
*	 case 2:
*		 8Bitpart2 = digit;
*		 break;
*	 case 3:
*		 8Bitpart2 = 8Bitpart2 + digit*10;
*		 break;
*	 case 4:
*		 8Bitpart3 = digit;
*		 break;
*	 case 5:
*		 8Bitpart3 = 8Bitpart3 + digit*10 ;
*		 break;
*	 case 6:
*		 8Bitpart4 = digit;
*		 break;
*	 case 7:
*		 8Bitpart4 = 8Bitpart3 + digit*10;
*		 break;
*	default:
*		 break;
* }
* counter ++;
* 32BitValue /= 10;
*}
*/