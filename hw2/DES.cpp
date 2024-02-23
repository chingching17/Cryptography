#include <fstream>
#include <cstring>
#include <time.h>
#include "Function_Of_DES.h"

using namespace std;

int main(){
	clock_t start, end;
	double cpu_time_used;
	double totaltime = 0;
	
	ifstream file("DES-Key-Plaintext.txt");
	if(!file.is_open())  cout << "file isn't open correctly'!";
	ofstream out("out.txt");
	
	string str, str1, output;
	char k[8], p[8];
	bitset<64> pla_text, cipher;

	double avg = 0;
	int count_times = 0;
	
	while(getline(file,str)){
		count_times++;
		
		//key
		str1 = str.substr(0,8);
		strcpy(k, str1.c_str()); 
		
		//plaintext
		str1 = str.substr(9,16);
		strcpy(p, str1.c_str()); 
		
		pla_text = CharToBitset(p);
		key = CharToBitset(k);
		
		for(int i = 0 ; i < 1000 ; i++){
			
			start = clock();
			
			generateSubKey();
			cipher = Encrypt(pla_text);
			
			end = clock();
			cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;

			totaltime += cpu_time_used;
			
		}
		output = BitsetToHex(cipher);
		for(int i = 0 ; i < 16 ; i++){
			cout << output[i];
			out << output[i];
		}
		cout << endl;
		out << endl;
			
		// cout << "cpu_time_used " << totaltime / 1000 << "(ms)" << endl;
		avg += totaltime / 1000;
			
		pla_text.reset();
		cipher.reset();
	}
	
	cout << "average of cpu_time_used " << avg / count_times * 1000 << "(ms)" << endl;
	out << "average of cpu_time_used " << avg / count_times * 1000 << "(ms)" << endl;
	
	file.close();
	out.close();
	 
	return 0;
}
