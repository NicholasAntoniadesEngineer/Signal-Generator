 

// --------------------------------------
//  Breaking a 32 bit value into 4 bytes:
// --------------------------------------
/*
counter = 0;

while (32BitValue > 0) {
 digit = 32BitValue % 10; // e.g. 13 % 10 = 3
 switch(counter){
	 case 0:
		 8Bitpart1 = digit; // e.g. = 3
		 break;
	 case 1:
		 8Bitpart1 = 8Bitpart1 + digit*10 ; // e.g. = 3 = 1*10
		 break;
	 case 2:
		 8Bitpart2 = digit;
		 break;
	 case 3:
		 8Bitpart2 = 8Bitpart2 + digit*10;
		 break;
	 case 4:		 8Bitpart3 = digit;
		 break;
	 case 5:
		 8Bitpart3 = 8Bitpart3 + digit*10 ;
		 break;
	 case 6:
		 8Bitpart4 = digit;
		 break;
	 case 7:
		 8Bitpart4 = 8Bitpart3 + digit*10;
		 break;
	default:
		 break;
 }
 counter ++;
 32BitValue /= 10;
}
*/