#include <iostream>
#include <bitset>
#include <string>
#include "array_DES.h"

using namespace std;

bitset<64> key;
bitset<48> subkey[16];
string Hex = "0123456789ABCDEF";

bitset<64> CharToBitset(const char s[8]){
	bitset<64> bits,temp;
	for(int i=0; i<8; i++){
		for(int j=0; j<8; j++){
			bits[i*8 + 7 - j] = (s[i] >> j) & 1; // Move to next j bits in array
		}
	}
	return bits;
}

string BitsetToHex(bitset<64> c){
	char output[16];
	int n,count=0;
	for(int i = 0 ; i < 64 ; i += 4){
		n = c[i] * 8 + c[i+1] * 4 + c[i+2] * 2 + c[i+3];
		output[count++] = Hex[n];
	}
	
	return output;
}

bitset<28> LeftShift(bitset<28>, int);
void generateSubKey(){
	
	bitset<56> reduced_key;
	bitset<28> left, right;
	bitset<48> sub_key;
	
	//PC1 64bits to 56bits
	for(int i = 0 ; i < 56 ; i++){
		reduced_key[i] = key[PC_1[i] - 1];
	}
	

	// generate subkey
	for(int round = 0 ; round < 16 ; round++){
		
		//divided to left and right
		for(int i = 0; i < 28; i++) left[i] = reduced_key[i];
		for(int i = 28; i < 56; i++) right[i-28] = reduced_key[i];
		
		//left shift
		left = LeftShift(left, shift_bits[round]);
		right = LeftShift(right, shift_bits[round]);

		//¦X¨Ö 
		for(int i = 0; i < 28; i++) reduced_key[i] = left[i];
		for(int i = 28; i < 56; i++) reduced_key[i] = right[i-28];
		
		//PC2
		for(int i = 0; i < 48; i++) sub_key[i] = reduced_key[PC_2[i] - 1];
		
		//Save subkey
		subkey[round] = sub_key;
	}
}

bitset<28> LeftShift(bitset<28> s, int shift){
	bitset<28> output = s;
	
	if(shift == 1){
		for(int i = 0; i < 27; i++){
			if(i - shift < 0) output[i-shift+28] = s[i]; //for i = 0 output[27]
			else output[i] = s[i+shift]; // output[1]~[26]
		}
		output[0] = s[1];
	}
	else if(shift == 2){
		for(int i = 0; i < 26; i++){
			if(i - shift < 0) output[i-shift+28] = s[i]; //for i = 0, 1 output[26][25]
			else output[i] = s[i+shift]; // output[2]~[24], new[25]
		}
		output[27] = s[1];
		output[0] = s[2];
		output[1] = s[3];
	}
	
	return output;
}

/*
	||||||||	  F		||||||||| 
*/

bitset<32> func(bitset<32> R, bitset<48> K){
	bitset<48> expand_R;
	bitset<32> result;
	
	// expand 32 to 48bits
	for(int i=0;i<48;i++){
		expand_R[i] = R[E[i] - 1];
	}
	
	// XOR
	expand_R = expand_R ^ K;
	
	// S-box
	int x = 0;
	for(int i = 0; i < 48; i+=6){
		
		int row = expand_R[i] * 2 + expand_R[i+5]; // b1b6
		int col = expand_R[i+1] * 8 + expand_R[i+2] * 4 + expand_R[i+3] * 2 + expand_R[i+4]; // b2b3b4b5
		int num_decimal = S_BOX[i/6][row][col];
		
		//decimal to binary
		bitset<4> num_binary(num_decimal); // ex. 8 = 0001(It looks inversely)
		
		result[x] = num_binary[3]; // put in inversely
		result[x+1] = num_binary[2];
		result[x+2] = num_binary[1];
		result[x+3] = num_binary[0];
		x += 4;
	}
	
	// Permutation(P)
	bitset<32> output = result;
	for(int i=0;i<32;i++){
		output[i] = result[P[i] - 1];
	}
//	cout << "output "<< output << endl;
	
	return output;
}


bitset<64> Encrypt(bitset<64> pla_text){
	bitset<64> M0, cipher;
	bitset<32> left, right, tmp;
	
	// Initial Permutation(IP)
	for(int i=0;i<64;i++){
		M0[i] = pla_text[IP[i] - 1];
	}
	
	//divided into left and right
	for(int i = 0 ; i < 32; i++){
		left[i] = M0[i];
		right[i] = M0[i+32];
	}
	
	//16 iterations
	for(int times = 0; times < 16; times++){
		tmp = right;
		right = left ^ func(right, subkey[times]);
		left = tmp;
	}
	
	//combine
	for(int i = 0; i < 32; i++){
		cipher[i] = right[i];
	}
	for(int i = 32; i < 64; i++){
		cipher[i] = left[i-32];
	}
	
	// IP^(-1)
	M0 = cipher;
	for(int i = 0; i < 64; i++){
		cipher[i] = M0[IP_1[i] - 1];
	}
	
	return cipher;
}
/*
// ¸Ñ±K 
bitset<64> decrypt(bitset<64> cipher) {
  bitset<64> M0;
  bitset<32> Li;
  bitset<32> Ri;
  bitset<32> temp;
  bitset<64> plain;

  // ªì©l¸m? IP
  for (int i = 0; i < 64; i++) {
    M0 [i] = cipher [IP [i] - 1];
  }

  for (int i = 0; i< 32; i++) {
    Li [i] = M0 [i];
  }
  for (int i = 32; i < 64; i++) {
    Ri [i - 32] = M0 [i];
  }

  // 16 ?­¡¥N T (°f§Ç)
  for (int times = 15; times >= 0; times--) {
    temp = Ri;
    Ri = Li^f (Ri, subKey [times]);
    Li = temp;
  }

  // °f¸m? IP^-1
  for (int i = 0; i < 32; i++) {
    plain [i] = Ri [i];
  }
  for (int i = 32; i < 64; i++) {
    plain [i] = Li [i - 32];
  }

  M0 = plain;
  for (int i = 0; i < 64; i++) {
    plain [i] = M0 [IP_[i] - 1];
  }

  return plain;
}*/

