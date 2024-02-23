#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <vector>
#include <sstream>
#include <cryptopp/files.h>
#include <cryptopp/modes.h>
#include <cryptopp/osrng.h>
#include <cryptopp/rsa.h>
#include <cryptopp/integer.h>
#include <cryptopp/cryptlib.h>
#include <cryptopp/hex.h>
#include <cryptopp/sha.h>
using namespace std;
using namespace CryptoPP;
typedef unsigned char byte;
ofstream out;

string hash_ans(string msg){
    ostringstream output;
    // HexEncoder encoder(new FileSink(std::cout));
    HexEncoder encoder(new FileSink(output));


    std::string digest;

    SHA256 hash;
    hash.Update((const byte*)msg.data(), msg.size());
    digest.resize(hash.DigestSize());
    hash.Final((byte*)&digest[0]);

    // std::cout << "Message: " << msg << std::endl;

    // std::cout << "Digest: ";
    StringSource(digest, true, new Redirector(encoder));
    // cout << output.str();

    // return digest;
    return output.str();

}

string convert_ASCII(string hex){
   string ascii = "";
   for (size_t i = 0; i < hex.length(); i += 2){
      //taking two characters from hex string
      string part = hex.substr(i, 2);
      //changing it into base 16
      char ch = stoul(part, nullptr, 16);
      //putting it into the ASCII string
      ascii += ch;
   }
   return ascii;
}

string int_to_hex( int i )
{
    stringstream stream;
    stream << std::setfill ('0') << std::setw(sizeof(i)*2) 
            << std::hex << i;

    string ans=stream.str();

    int toupper ( int c );
    std::for_each(ans.begin(), ans.end(), [](char & c){
        c = ::toupper(c);
    });

    return ans;
}

int main(){
    out.open("out.txt");
    // std::string msg = "Hello!";
    // hash_ans(msg);

    std::string msg1 = "Bitcoin is a cryptocurrency, a form of electronic cash.";
    cout << hash_ans(msg1) << endl;
    out << hash_ans(msg1) << endl << endl;

    // std::string msg2 = "Bitcoin";
    // string digest = hash_ans(msg2);
    // cout << "convert: " << digest << endl;
    // if(digest == "B4056DF6691F8DC72E56302DDAD345D65FEAD3EAD9299609A826E2344EB63AA4"){
    //     cout << "i success" << endl;
    // }
    // else
    // {
    //     cout << "no no " << endl;
    // }

    std::string msg2 = "Bitcoin";
    string pre = hash_ans(msg2);

    std::vector<int> nonce;
    bool whether_nonce;

    int leading_zero = 0;
    int i = -1;

    while(leading_zero < 64 || i < 2147483647)
    {
        i++;
        whether_nonce = 1;
        string str = pre;
        str.append(int_to_hex(i));
        str = convert_ASCII(str);
        str = hash_ans(str);
        for (int k = 0 ; k < leading_zero ; k++){
            if(str[k] != '0') 
            {
                whether_nonce = 0; 
                k = leading_zero;   // break
            }
        }
        if(whether_nonce == 1)
        { 
            vector<int>::iterator result = find( nonce.begin( ), nonce.end( ), i );
            if(result == nonce.end()) {
                nonce.push_back(i);
                cout << leading_zero << endl;
                out << leading_zero << endl;

                cout << pre << endl;
                out << pre << endl;

                cout << int_to_hex(i) << endl;
                out << int_to_hex(i) << endl;

                cout << str << endl;
                out << str << endl;

                pre = str;
                leading_zero++;
                i = -1;
            }
        }
    }
    out.close();
    return 0;
}
