void bit16print(unsigned char x){  // 以十六进制输出一个字节
    static char str[2];
    str[0] = str[1] = '0';
    unsigned char len = 0;
    while(x){
        unsigned char d = x & 15;
        x >>= 4;
        if(d>=0 && d<=9) str[len++] = '0'+d;
        else str[len++] = 'A'+(d-10);
    }
    putchar(str[1]);
    putchar(str[0]);
}

void des_cfb_encrypt(unsigned char p[], int n, unsigned char des_seed_iv[],
/*=================*/ unsigned char des_iv[], unsigned char des_seed_key[]){
    // 借助已知明文密文获得密钥流，进而RC4加密得到des_key
    unsigned char des_key[8], des_v[8];
    for(register int i = 0; i < 8; i++)
        des_key[i] = des_seed_key[i] ^ (des_seed_iv[i]^des_iv[i]);
    
    // 用CFB模式进行加密
    unsigned char *cipherText = (unsigned char*) malloc(n);
    for(register int i = 0; i < n; i++){
        memcpy(des_v, des_iv, 8);
        des_encrypt(des_v, des_key);
        cipherText[i] = p[i] ^ des_v[0];
        for(register int k = 0; k < 7; k++)
            des_iv[k] = des_iv[k+1];
        des_iv[7] = cipherText[i];
        bit16print(cipherText[i]);
    }
    free(cipherText);
}