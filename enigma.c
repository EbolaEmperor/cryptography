#include <stdio.h>
#include <string.h>

const char reflector[]="YRUHQSLDPXNGOKMIEBFZCWVJAT"; 
const char rotor_table[][27] = 
{
    "EKMFLGDQVZNTOWYHXUSPAIBRCJ",
    "AJDKSIRUXBLHWTMCQGZNPYFVOE",
    "BDFHJLCPRTXVZNYEIWGAKMUSQO",
    "ESOVPZJAYQUIRHXLNFTGKDCMWB",
    "VZBRGITYUPSDNHLXAWMJQOFECK"
};
const char rotor_inverse_table[][27] = 
{
    "UWYGADFPVZBECKMTHXSLRINQOJ",
    "AJPCZWRLFBDKOTYUQGENHXMIVS",
    "TAGBPCSDQEUFVNZHYIXJWLRKOM",
    "HZWVARTNLGUPXQCEJMBSKDYOIF",
    "QCYLXWENFTZOSMVJUDKGIARPHB"
};
const char step_char[]="RFWKA";

char plug[27]="ABCDEFGHIJKLMNOPQRSTUVWXYZ";
char plainText[512], word[32];
int rotor_choose[3];
int rotor_setting[3];
int message_key[3];

void roll()
{
    message_key[2] = (message_key[2]+1)%26;
    if(message_key[2] != step_char[rotor_choose[2]]-'A'
    && message_key[1] != (step_char[rotor_choose[1]]-'A'+25)%26 ) return;
    message_key[1] = (message_key[1]+1)%26;
    if(message_key[1] != step_char[rotor_choose[1]]-'A') return;
    message_key[0] = (message_key[0]+1)%26;
}

char getnext(char inp)
{
    roll();
    inp = plug[inp-'A'];
    for(int i = 2; i >= 0; i--)
    {
        int idx = rotor_choose[i];
        int delta = message_key[i] - rotor_setting[i];
        inp = rotor_table[idx][(inp-'A'+delta+26)%26];
        inp = (inp-'A'-delta+26)%26 + 'A';
    }
    inp = reflector[inp - 'A'];
    for(int i = 0; i < 3; i++)
    {
        int idx = rotor_choose[i];
        int delta = message_key[i] - rotor_setting[i];
        inp = rotor_inverse_table[idx][(inp-'A'+delta+26)%26];
        inp = (inp-'A'-delta+26)%26 + 'A';
    }
    inp = plug[inp-'A'];
    return inp;
}

void encrypt(char *s)
{
    int n = strlen(s);
    for(int i = 0; i < n; i++)
        s[i] = getnext(s[i]);
}

int found(char *s, char *subs)
{
    int n = strlen(s), m = strlen(subs);
    for(int i = 0; i < n; i++)
    {
        int flag = 1;
        for(int j = 0; j < m; j++)
            if(s[i+j] != subs[j])
            {
                flag = 0;
                break;
            }
        if(flag) return 1;
    }
    return 0;
}

int diff(char *s, char *t)
{
    int n = strlen(s);
    for(int i = 0; i < n; i++)
        if(s[i]==t[i]) return 0;
    return 1;
}

void decrypt(char *plainText)
{
    static char s[512];
    for(rotor_choose[1] = 0; rotor_choose[1] < 5; rotor_choose[1]++)
        for(rotor_choose[2] = 0; rotor_choose[2] < 5; rotor_choose[2]++)
        {
            if(rotor_choose[0]==rotor_choose[1] || rotor_choose[0]==rotor_choose[2] || rotor_choose[1]==rotor_choose[2]) continue;
            for(char c1 = 'A'; c1 <= 'Z'; c1++)
                for(char c2 = 'A'; c2 <= 'Z'; c2++)
                    for(char c3 = 'A'; c3 <= 'Z'; c3++)
                    {
                        message_key[0] = c1 - 'A';
                        message_key[1] = c2 - 'A';
                        message_key[2] = c3 - 'A';
                        memcpy(s, plainText, sizeof(s));
                        encrypt(s);
                        if(found(s, word) && diff(s, plainText))
                        {
                            message_key[0] = c1 - 'A';
                            message_key[1] = c2 - 'A';
                            message_key[2] = c3 - 'A';
                            memcpy(plainText, s, sizeof(s));
                            return;
                        }
                    }
        }
}

int main()
{
    char s[10];
    for(int i = 0; i < 10; i++)
    {
        scanf("%s", s);
        plug[s[0]-'A'] = s[1];
        plug[s[1]-'A'] = s[0];
    }
    scanf("%s", s);
    rotor_setting[0] = s[0]-'A';
    rotor_setting[1] = s[1]-'A';
    rotor_setting[2] = s[2]-'A';
    scanf("%s%s", plainText, word);
    scanf("%d", rotor_choose);
    rotor_choose[0]--;
    decrypt(plainText);
    printf("MessageKey=%c%c%c\n", 'A'+message_key[0], 'A'+message_key[1], 'A'+message_key[2]);
    printf("PlainText=%s\n", plainText);
    printf("RotorNum=%d%d%d\n", rotor_choose[0]+1, rotor_choose[1]+1, rotor_choose[2]+1);
}