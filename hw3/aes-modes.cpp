#include <iostream>
#include <fstream>
#include <iomanip>
#include <cryptopp/aes.h>
#include <cryptopp/hex.h>
#include <cryptopp/modes.h>
#include <cryptopp/filters.h>
using namespace std;

ofstream out;

typedef unsigned char byte;

std::string enc_ecb(std::string plaintext, byte key[], std::string padding){

    std::string ciphertext;

    CryptoPP::ECB_Mode< CryptoPP::AES >::Encryption eecb;
    eecb.SetKey( key, CryptoPP::AES::DEFAULT_KEYLENGTH );

    if(padding == "zero"){
        CryptoPP::StringSource en(plaintext, true, new CryptoPP::StreamTransformationFilter(eecb, new CryptoPP::StringSink(ciphertext), CryptoPP::BlockPaddingSchemeDef::ZEROS_PADDING));
        return ciphertext;
    }else{
        CryptoPP::StringSource en(plaintext, true, new CryptoPP::StreamTransformationFilter(eecb, new CryptoPP::StringSink(ciphertext), CryptoPP::BlockPaddingSchemeDef::PKCS_PADDING));
        return ciphertext;
    }

}

std::string enc_cbc(std::string plaintext, byte key[], byte iv[], std::string padding){

    std::string ciphertext;

    CryptoPP::CBC_Mode< CryptoPP::AES >::Encryption ecbc;
    ecbc.SetKeyWithIV( key, CryptoPP::AES::DEFAULT_KEYLENGTH , iv );

    if(padding == "zero"){
        CryptoPP::StringSource en(plaintext, true, new CryptoPP::StreamTransformationFilter(ecbc, new CryptoPP::StringSink(ciphertext), CryptoPP::BlockPaddingSchemeDef::ZEROS_PADDING));
        return ciphertext;
    }else{
        CryptoPP::StringSource en(plaintext, true, new CryptoPP::StreamTransformationFilter(ecbc, new CryptoPP::StringSink(ciphertext), CryptoPP::BlockPaddingSchemeDef::PKCS_PADDING));
        return ciphertext;
    }
}

std::string enc_cfb(std::string plain, byte key[], byte iv[]){
    std :: string cipher;
    CryptoPP::SecByteBlock iv_(reinterpret_cast<const byte*>(&iv[0]), CryptoPP::AES::BLOCKSIZE);
    CryptoPP::AlgorithmParameters params = CryptoPP::MakeParameters(CryptoPP::Name::FeedbackSize(), 4 /*32-bits*/)
                (CryptoPP::Name::IV(), CryptoPP::ConstByteArrayParameter(iv_));
    
    CryptoPP::CFB_Mode< CryptoPP::AES >::Encryption e;
    e.SetKey(key, CryptoPP::AES::DEFAULT_KEYLENGTH, params);
    CryptoPP::StringSource(plain, true, new CryptoPP::StreamTransformationFilter(e, new CryptoPP::StringSink(cipher) ));
    
    return cipher;
}

void printHex(std::string ciphertext){
    for( unsigned int i = 0; i < ciphertext.size(); i++ ) {
        std::cout << std::setfill('0') << std::setw(2) << std::hex << (0xFF & static_cast<byte>(ciphertext[i]));
        out << std::setfill('0') << std::setw(2) << std::hex << (0xFF & static_cast<byte>(ciphertext[i]));
    }
    std::cout << std::endl;
    out << std::endl;
}

int main(int argc, char* argv[]) {

    // Homework
    byte key[ CryptoPP::AES::DEFAULT_KEYLENGTH ], iv[ CryptoPP::AES::BLOCKSIZE ];
    memset( key, 0x00, CryptoPP::AES::DEFAULT_KEYLENGTH );
    memset( iv, 0x00, CryptoPP::AES::BLOCKSIZE );

    std::string plaintext = "AES is the block cipher standard.";//"Hello World!";
    for (int i=0 ; i<10 ; i++) key[i] = ('0' + ((i+1)%10) ) ; //1234567890
    key[10] = 'A';
    key[11] = 'B';
    key[12] = 'C';
    key[13] = 'D';
    key[14] = 'E';
    key[15] = 'F';

    out.open("Out.txt");
    for (int i=0 ; i<CryptoPP::AES::BLOCKSIZE ; i++) iv[i] = '0'; //0000000000000000
    printHex(enc_cfb(plaintext, key, iv));
    printHex(enc_cbc(plaintext, key, iv, "zero"));

    for (int i=0 ; i<CryptoPP::AES::BLOCKSIZE ; i++) iv[i] = '9'; //0000000000000000
    printHex(enc_cbc(plaintext, key, iv, "pkcs"));

    for (int i=0 ; i<CryptoPP::AES::BLOCKSIZE ; i++) iv[i] = '0'; //0000000000000000
    printHex(enc_ecb(plaintext, key, "pkcs"));

    out.close();

/*

    // On-Site
    std::string skey = "RandomNumbers000";
    for (int i=0 ; i<CryptoPP::AES::DEFAULT_KEYLENGTH ; i++) key[i] = skey[i] ;
    for (int i=0 ; i<CryptoPP::AES::BLOCKSIZE ; i++) iv[i] = '9' ;

    std::string ciphertext = "188ca5a7848e568c7ffe22fa0cfc5fc1ca1636f96eae65385a125f110935490da91dc0ba28a399739da63eda7485e705f83ad17a562027bcf80523b92a823f5a8d7e93ea384e52f57b1a86f8f69aa851a85be4f7a4bcc4c7ce81548fa71175923b2887518ce2a64ba9b1e20384020c2f";
    std::string decodeciphertext;
    std::string decryptedtext;

    CryptoPP::StringSource ss(ciphertext, true, new CryptoPP::HexDecoder( new CryptoPP::StringSink(decodeciphertext)));

    CryptoPP::CBC_Mode< CryptoPP::AES >::Decryption dcbc;
    dcbc.SetKeyWithIV( key, CryptoPP::AES::DEFAULT_KEYLENGTH, iv );

    CryptoPP::StreamTransformationFilter stfDecryptor(dcbc, new CryptoPP::StringSink( decryptedtext ) , CryptoPP::BlockPaddingSchemeDef::ZEROS_PADDING);
    stfDecryptor.Put( reinterpret_cast<const unsigned char*>( decodeciphertext.c_str() ), decodeciphertext.size() );
    stfDecryptor.MessageEnd();

    std::cout << "Decrypted ciphertext (CBC, ZerosPadding): " << decryptedtext << std::endl;
*/
    return 0;
}

