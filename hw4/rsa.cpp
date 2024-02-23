#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <cryptopp/files.h>
#include <cryptopp/modes.h>
#include <cryptopp/osrng.h>
#include <cryptopp/rsa.h>
#include <cryptopp/integer.h>
using namespace std;
using namespace CryptoPP;
typedef unsigned char byte;
ofstream out;

void enc(int len, Integer n, Integer e, string message){
    AutoSeededRandomPool prng;
    RSA::PrivateKey privKey;

    privKey.GenerateRandomWithKeySize(prng, len);
    RSA::PublicKey pubKey(privKey);

    pubKey.Initialize(n, e);
    Integer m, c;

    m = Integer((const byte *)message.data(), message.size());
    c = pubKey.ApplyFunction(m);
    cout << "ciphertext: " << hex << c << endl;
    out << hex << c << endl;
    
}

void dec(int len, Integer n, Integer e, Integer d, Integer c){
    AutoSeededRandomPool prng;
    RSA::PrivateKey privKey;
    privKey.GenerateRandomWithKeySize(prng, len);

    privKey.Initialize(n, e, d);

    Integer r;
    string recovered;

    // Decrypt
    r = privKey.CalculateInverse(prng, c);

    // Round trip the message
    size_t req = r.MinEncodedSize();
    recovered.resize(req);
    r.Encode((byte *)recovered.data(), recovered.size());

    cout << "recovered: " << recovered << endl;
    out << "\"" ;
    out << recovered; 
    out << "\""  << endl;

}


int main(){
    //part1
    // Integer n1("0xb14022eef719f1bb"), e1("0x11");
    // string message1 = "Alice";
    // enc(64, n1, e1, message1);

    out.open("out.txt");
    //part2
    Integer n2("0xb2c8d1404ed5fc2f7ad1254bb428f0d5"), e2("0x11");
    string message2 = "Hello World!";
    enc(128, n2, e2, message2);

    //part3
    Integer n3("0xcf625a8e47bc1cf9a3b517b31d870108c0cd97466003842a3b394d6cd857e9b7"), e3("0x10001");
    string message3 = "RSA is public key.";
    enc(256, n3, e3, message3);

    //decryption
    // part1
    // Integer n4("0x9d001e6473dfacf9"), e4("0x10001"), d4("0x16282b21a7866bf5");
    // Integer c4("0x154c638cd3615216");
    // dec(64, n4, e4, d4, c4);

    //part2
    Integer n5("0xd6361e40b2d619970ead338912a273adb75a4ce21356304834753fe94e6de24b"), e5("0x11")
            , d5("0x12e6a85100b889c9905a939b274a91bc57ca85d52e6c464fb455c86a29d63c89");
    Integer c5("0xa1676afd68a2fc67dac32c633600b76fa90aca9f9cca5201490a20c8b01a061a");
    dec(256, n5, e5, d5, c5);

    out << "Public key: " << hex << e5 << endl;

    out.close();

}
