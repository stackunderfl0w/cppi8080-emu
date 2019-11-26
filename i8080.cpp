#include "i8080.h"
#include <iostream>
using namespace std;
#define M memory[get_hl()]
class i8080
{
    private:
    bool debug=false;
    bool cpm=false;
    unsigned char OPCODES_CYCLES[256] = {
//  0   1   2   3   4   5   6   7   8   9   A   B   C   D   E   F
    4,  10, 7,  5,  5,  5,  7,  4,  4,  10, 7,  5,  5,  5,  7,  4,  // 0
    4,  10, 7,  5,  5,  5,  7,  4,  4,  10, 7,  5,  5,  5,  7,  4,  // 1
    4,  10, 16, 5,  5,  5,  7,  4,  4,  10, 16, 5,  5,  5,  7,  4,  // 2
    4,  10, 13, 5,  10, 10, 10, 4,  4,  10, 13, 5,  5,  5,  7,  4,  // 3
    5,  5,  5,  5,  5,  5,  7,  5,  5,  5,  5,  5,  5,  5,  7,  5,  // 4
    5,  5,  5,  5,  5,  5,  7,  5,  5,  5,  5,  5,  5,  5,  7,  5,  // 5
    5,  5,  5,  5,  5,  5,  7,  5,  5,  5,  5,  5,  5,  5,  7,  5,  // 6
    7,  7,  7,  7,  7,  7,  7,  7,  5,  5,  5,  5,  5,  5,  7,  5,  // 7
    4,  4,  4,  4,  4,  4,  7,  4,  4,  4,  4,  4,  4,  4,  7,  4,  // 8
    4,  4,  4,  4,  4,  4,  7,  4,  4,  4,  4,  4,  4,  4,  7,  4,  // 9
    4,  4,  4,  4,  4,  4,  7,  4,  4,  4,  4,  4,  4,  4,  7,  4,  // A
    4,  4,  4,  4,  4,  4,  7,  4,  4,  4,  4,  4,  4,  4,  7,  4,  // B
    5,  10, 10, 10, 11, 11, 7,  11, 5,  10, 10, 10, 11, 17, 7,  11, // C
    5,  10, 10, 10, 11, 11, 7,  11, 5,  10, 10, 10, 11, 17, 7,  11, // D
    5,  10, 10, 18, 11, 11, 7,  11, 5,  5,  10, 4,  11, 17, 7,  11, // E
    5,  10, 10, 4,  11, 11, 7,  11, 5,  5,  10, 4,  11, 17, 7,  11  // F
    };
    //unsigned char OPCODES_CYCLES[256] = {4,10,7,6,5,5,7,4,0,11,7,6,5,5,7,4      ,4,10,7,6,5,5,7,4,4,11,7,6,5,5,7,4      ,4,10,16,6,5,5,7,4,4,11,16,6,5,5,7,4        ,4,10,13,6,10,10,10,4,4,11,13,6,5,5,7,4     ,5,5,5,5,5,5,7,5,5,5,5,5,5,5,7,5        ,5,5,5,5,5,5,7,5,5,5,5,5,5,5,7,5    ,5,5,5,5,5,5,7,5,5,5,5,5,5,5,7,5    ,7,7,7,7,7,7,7,7,5,5,5,5,5,5,7,5    ,4,4,4,4,4,4,7,4,4,4,4,4,4,4,7,4    ,4,4,4,4,4,4,7,4,4,4,4,4,4,4,7,4    ,4,4,4,4,4,4,7,4,4,4,4,4,4,4,7,4    ,4,4,4,4,4,4,7,4,4,4,4,4,4,4,7,4    ,11,10,15,10,18,11,7,11,11,10,15,00,18,17,7,11      ,11,10,15,10,18,11,7,11,11,4,15,10,18,4,7,11    ,11,10,15,4,18,11,7,11,11,4,10,4,18,11,7,11     ,11,10,15,4,18,11,7,11,11,6,15,4,18,4,7,11};

    int par_table[256]={1,0,0,1,0,1,1,0,0,1,1,0,1,0,0,1,0,1,1,0,1,0,0,1,1,0,0,1,0,1,1,0,0,1,1,0,1,0,0,1,1,0,0,1,0,1,1,0,1,0,0,1,0,1,1,0,0,1,1,0,1,0,0,1,0,1,1,0,1,0,0,1,1,0,0,1,0,1,1,0,1,0,0,1,0,1,1,0,0,1,1,0,1,0,0,1,1,0,0,1,0,1,1,0,0,1,1,0,1,0,0,1,0,1,1,0,1,0,0,1,1,0,0,1,0,1,1,0,0,1,1,0,1,0,0,1,1,0,0,1,0,1,1,0,1,0,0,1,0,1,1,0,0,1,1,0,1,0,0,1,1,0,0,1,0,1,1,0,0,1,1,0,1,0,0,1,0,1,1,0,1,0,0,1,1,0,0,1,0,1,1,0,1,0,0,1,0,1,1,0,0,1,1,0,1,0,0,1,0,1,1,0,1,0,0,1,1,0,0,1,0,1,1,0,0,1,1,0,1,0,0,1,1,0,0,1,0,1,1,0,1,0,0,1,0,1,1,0,0,1,1,0,1,0,0,1};
    int codes[256];
    unsigned short pc=0;
    unsigned short sp=0;
    //int memory[0x1000];
    bool carry=false;
    bool halfcarry=false;
    bool zero=false;
    bool sign=false;
    bool parity=false;
    bool interrupts=false;
    
    int tc=-1;

    unsigned char B=0;
    unsigned char C=0;
    unsigned char D=0;
    unsigned char E=0;
    unsigned char H=0;
    unsigned char L=0;
    unsigned char A=0;


    bool extra_ship_location=0;//0=1500 1=1000 points
    bool display_coin_in_demo=0;//0=ON
    bool extra_ships=0;//extra starting ships 0-3(3-6 total)
    unsigned char port_2;
    unsigned char port_4lo;
    unsigned char port_4hi;
    // paired registers helpers (setters and getters)
                
    inline void set_af(unsigned short int val) {
        A = val >> 8;
        set_psw (val & 0xFF);
    }
    inline void set_bc(unsigned short int val) {
        B = val >> 8;
        C = val & 0xFF;
    }

    inline void set_de(unsigned short int val) {
        D = val >> 8;
        E = val & 0xFF;
    }

    inline void set_hl(unsigned short int val) {
        H = val >> 8;
        L = val & 0xFF;
    }

    inline unsigned short int get_af() {
        return (A << 8) | get_psw();
    }

    inline unsigned short int get_bc() {
        return (B << 8) | C;
    }

    inline unsigned short int get_de() {
        return (D << 8) | E;
    }

    inline unsigned short int get_hl() {
        return (H << 8) | L;
    }
    inline unsigned short int get_psw() {
        return(sign<<7)|(zero<<6)|(halfcarry<<4)|(parity<<2)|(1<<1)|carry;
    }
    inline void set_psw(unsigned char x) {
        sign=(x>>7);
        zero=(x>>6)&1;
        halfcarry=(x>>4)&1;
        parity=(x>>2)&1;
        carry=x&1;
    }
    inline unsigned char read_byte(unsigned short adr){
        return memory[adr];
    }
    inline void write_byte(unsigned short adr,unsigned char x){
        memory[adr]=x;
    }
    inline void write_word(unsigned short adr,unsigned short x){
        write_byte(adr,x & 0xFF);
        write_byte(adr + 1, x >> 8);
    }
    inline unsigned char next_byte() {
        return read_byte(pc++);
    }
    inline unsigned short next_word(){
        unsigned short result=read_word(pc);
        pc+=2;
        return result;
    }
    inline unsigned short read_word(unsigned short adr) {
        return read_byte(adr + 1) << 8 |read_byte(adr);
    }
    inline void INR(unsigned char& x){
        x++;
        halfcarry = (x & 0xF) == 0;
        check_ZSP(x);
    }
    inline void DCR(unsigned char& x){
        x--;
        halfcarry = (x & 0xF) != 0xF;
        check_ZSP(x);
    }
    inline void MOV(unsigned char& x,unsigned char y){
        x=y;
    }
    inline void jump(unsigned short adr){
        pc=adr;
    }
    inline void cond_jump(bool cond){
        unsigned short adr=next_word();
        if(cond){
            jump(adr);
        }
        //else{cycles-=5;}
    }
    inline void call(unsigned short adr){
        PUSH(pc);
        jump(adr);
    }
    inline void cond_call(bool cond){
        unsigned short adr=next_word();
        if(cond){
            call(adr);
            cycles+=6;
        }
        //else{cycles-=7;}
    }
    inline void ret(){
        pc=POP();
    }
    inline void cond_ret(bool cond){
        if(cond){
            ret();
            cycles+=6;
        }
        //else{cycles-=6;}
    }
    inline void ADD(unsigned char& x,unsigned char y){
        carry=((int)x+y)>255;
        halfcarry = (x & 0x0F) + (y & 0x0F)> 0x0F;
        x+=y;
        check_ZSP(x);
    }
    inline void ADC(unsigned char& x,unsigned char y){
        halfcarry = (x & 0x0F) + (y & 0x0F)+carry> 0x0F;
        bool cy = ((int)x+y+carry)>255;
        x+=y+carry;
        check_ZSP(x);
        carry=cy;
    }
    inline void SUB(unsigned char& x,unsigned char y){
        carry=((int)x-y)<0;
        //halfcarry = (((x-y) ^ x ^ y) & 0x10) != 0;
        //halfcarry = (((x & 0xF) - (y & 0xF)) < 0);
        halfcarry = (x & 0x0F) - (y & 0x0F) >= 0;
        //unsigned char result = x - y;
        //unsigned char cy = result ^ a ^ b;
        //halfcarry= carry & (1 << bit_no);

        x-=y;
        check_ZSP(x);
    }
    inline void SBB(unsigned char& x,unsigned char y){
        bool cy=((int)x-y-carry)<0;
        halfcarry = (x & 0x0F) - (y & 0x0F) -carry>= 0;
        x-=y+carry;
        check_ZSP(x);
        carry=cy;
    }

    inline void ANA(unsigned char& x,unsigned char y){
        carry=false;
        halfcarry = ((x | y) & 0x08) != 0;
        x&=y;
        check_ZSP(x);
    }
    inline void XRA(unsigned char& x,unsigned char y){
        carry=false;
        halfcarry =false;
        x^=y;
        check_ZSP(x);
    }
    inline void ORA(unsigned char& x,unsigned char y){
        carry=false;
        halfcarry=false;
        x|=y;
        check_ZSP(x);
    }
    inline void CMP(unsigned char x, unsigned char y){
        carry=y>x;
        unsigned char z=x-y;
        halfcarry = ~(x ^ z ^ y) & 0x10;
        check_ZSP(z);
    }
    inline void PUSH(unsigned short x){
        memory[sp-1]=x >> 8;;
        memory[sp-2]=x & 0xFF;
        sp-=2;
    }
    inline unsigned short POP(){
        unsigned short temp=(memory[sp+1] << 8) | memory[sp];
        sp+=2;
        return temp;
    }
    inline void RLC(unsigned char& x){
        carry=x>>7;
        x=(x<<1)|carry;
    }
    inline void RAL(unsigned char& x){
        bool cy = carry;
        carry = x >> 7;
        x = (x << 1) | cy;
    }
    inline void RRC(unsigned char& x){
        carry=x&0b1;
        x=(x>>1)|(carry<<7);
    }
    inline void RAR(unsigned char& x){
        bool cy = carry;
           carry=x&0x1;
        x = (x >> 1) | (cy << 7);
    }
    inline void DAA(unsigned char& x){
        bool cy = carry;
        unsigned char correction = 0;

        unsigned char lsb = A & 0x0F;
        unsigned char msb = A >> 4;

        if (halfcarry || lsb > 9) {
            correction += 0x06;
        }
        if (carry || msb > 9 || (msb >= 9 && lsb > 9)) {
            correction += 0x60;
            cy = 1;
        }
        ADD(A, correction);
        carry = cy;
        /*if(halfcarry|(x&0xf)>9){x+=6;halfcarry=true;}else{halfcarry=false;}
        if(carry|x>>4>9){x+=6<<4;carry=true;}
        check_ZSP(x);*/
    }
    inline void check_zero(unsigned char x){
        zero = x==0;
    }
    inline void check_sign(unsigned char x){
        sign = (x & 0x80)!=0;
    }
    inline void check_parity(unsigned char x){
        parity = par_table[x]==1;
    }
    inline void check_ZSP(unsigned char x){
        check_zero(x);
        check_sign(x);
        check_parity(x);
    }
    inline void check_carry(unsigned char x){
        carry =(x & 0b100000000) != 0;
    }
    inline unsigned char in(unsigned char port){
        switch(port){
            case 1:
                //get_key();
                //System.out.println(coin+(two_player_button<<1)+(one_player_button<<2)+(1<<3)+(player_one_fire<<4) +(player_one_left<<5)+(player_one_right<<6));
                //cout<<(coin|(two_player_button<<1)|(one_player_button<<2)|(1<<3)|(player_one_fire<<4)|(player_one_left<<5)|(player_one_right<<6))<<endl;
                return coin|(two_player_button<<1)|(one_player_button<<2)|(1<<3)|(player_one_fire<<4)|(player_one_left<<5)|(player_one_right<<6);
            case 2:
                return extra_ships|(extra_ship_location<<3)|(player_two_fire<<4)|(player_two_left<<5)|(player_two_right<<6)|(display_coin_in_demo<<7);

            case 3:
                return (((((int)port_4hi<<8)|(int)port_4lo)<<port_2)>>8);

                default:return 0;
        }
        return 0;
    }
    //takes input from cpu (used for bitshift register input and sound);
    inline void out(unsigned char a,unsigned char port){
        switch(port){
            case 2:
                port_2=a;
                break;
            case 3://play sound
                //since the original sound hardware is analogue we need to check if the sound is already suppost to be
                // playing or else it will run multiple times stack and slow down the entire program
                //special case, ufo sfx needs to loop
                /*if (a%2==1){if(!sounds_playing[0]|x>65){ Main.screen.playAudio(sounds[0]);sounds_playing[0]=true;x=0;} }
                else{sounds_playing[0]=false;}
                x++;
                if ((a>>1)%2==1){if(!sounds_playing[1]){ Main.screen.playAudio(sounds[1]);sounds_playing[1]=true;} }
                else{sounds_playing[1]=false;}
                if ((a>>2)%2==1){if(!sounds_playing[2]){ Main.screen.playAudio(sounds[2]);sounds_playing[2]=true;} }
                else{sounds_playing[2]=false;}
                if ((a>>3)%2==1){if(!sounds_playing[3]){ Main.screen.playAudio(sounds[3]);sounds_playing[3]=true;} }
                else{sounds_playing[3]=false;}
                //if ((a>>4)%2==1){if(!sounds_playing[4]){ Main.screen.playAudio(sounds[4]);sounds_playing[4]=true;} }
                //else{sounds_playing[4]=false;}*/
                break;
            case 4://input for bitshift register
                port_4lo=port_4hi;
                port_4hi=a;
                break;
            case 5://play sound 2(secord set of sounds)
                /*if (a%2==1){if(!sounds_playing[5]){ Main.screen.playAudio(sounds[5]);sounds_playing[5]=true;} }
                else{sounds_pld6aying[5]=false;}
                if ((a>>1)%2==1){if(!sounds_playing[6]){ Main.screen.playAudio(sounds[6]);sounds_playing[6]=true;} }
                else{sounds_playing[6]=false;}
                if ((a>>2)%2==1){if(!sounds_playing[7]){ Main.screen.playAudio(sounds[7]);sounds_playing[7]=true;} }
                else{sounds_playing[7]=false;}
                if ((a>>3)%2==1){if(!sounds_playing[8]){ Main.screen.playAudio(sounds[8]);sounds_playing[8]=true;} }
                else{sounds_playing[8]=false;}
                if ((a>>4)%2==1){if(!sounds_playing[9]){ Main.screen.playAudio(sounds[9]);sounds_playing[9]=true;} }
                else{sounds_playing[9]=false;}*/
                break;
            default:break;
        }
    }
    
public:
    bool player_one_fire;
    bool player_one_left;
    bool player_one_right;
    bool one_player_button;
    bool coin;
    bool player_two_fire;
    bool player_two_left;
    bool player_two_right;
    bool two_player_button;
    bool keys[16];
    unsigned char memory[0x10000];
    long long cycles;
    i8080(){
        cycles=0;
        for (int i = 0; i < 16; ++i){
            keys[i]=false;
        }
        for (int i = 0; i < 256; ++i)
        {
            codes[i]=0;
        }
        
        sp=0;
        if(cpm){
            for(int i=0;i<0x10000;i++){
                memory[i]=0;
            }
            FILE *file = fopen("roms/z80/tests/zexdoc.com","rb");
            if (file != NULL)
            {
                cout<<endl<<"File opened"<<endl;
            }
            fseek(file, 0L, SEEK_END);
            int fsize = ftell(file);
            fseek(file, 0L, SEEK_SET);
            unsigned char *buffer = (unsigned char*) malloc(fsize);
            fread(buffer, fsize, 1, file);
            fclose(file);
            cout<<"     "<<fsize<<" bytes loaded"<<endl;
            cout<<"     "<<"Header-"<<buffer<<endl;
            for (int i = 0; i < fsize; ++i){
                memory[0x100 + i] = buffer[i];
                // Print for debug purposes
                //printf("%02X", memory[0x200 + i]);
                //cout<<endl;
            }
            if(cpm){
                pc=0x100;
                memory[5]=0xc9;
                //sp=0x2400;
                // inject "out 1,a" at 0x0000 (signal to stop the test)
                memory[0x0000] = 0xD3;
                memory[0x0001] = 0x00;

                // inject "in a,0" at 0x0005 (signal to output some characters)
                memory[0x0005] = 0xDB;
                memory[0x0006] = 0x00;
                memory[0x0007] = 0xC9;
            }
        }
        //cout<<hex<<memory[0x200];
        //cout<<hex<<memory[0x201]<<endl;
        cout<<"     "<<"Starting opcode "<<hex<<((memory[pc])<<8) + memory[pc+1]<<endl;
    }
    ~i8080(){

    }
    void cycle(){
        tc++;
        //if(tc==0x90000000){cout<<endl;exit(0);}
        //if(tc==2000000){debug=true;}
        if(false){
            cout<<hex<<"True counter:"<<tc<<endl;
            cout<<dec<<"cycles:"<<cycles<<endl;
            cout<<hex<<"    Program counter:"<<pc<<endl;
            cout<<"    "<<get_af()<<" "<<get_bc()<<" "<<get_de()<<" "<<get_hl()<<" "<<sp<<" ";
            if(zero){cout<<"z";}
            if(sign){cout<<"s";}
            if(halfcarry){cout<<"h";}
            if(parity){cout<<"p";}
            if(carry){cout<<"c";}
            cout<<endl;
            cout<<"    Opcode:"<<(int)memory[pc]<<"   "<<(int)memory[pc+1]<<" "<<(int)memory[pc+2]<<endl;
            try{
                std::cout<<"    "<<(int)M<<endl;
            }
            catch (int e)
            {
                cout << "An exception occurred. Exception Nr. " << e << '\n';
            }
        }
        if(false){
            FILE * fp;

               fp = fopen ("file.txt", "a");
            fprintf(fp,
            "TC: %08X, PC: %04X, AF: %04X, BC: %04X, DE: %04X, HL: %04X, SP: %04X, M: %04X, CYC: %lu",
            tc,
            pc,
            get_af(),
            get_bc(),
            get_de(),
            get_hl(),
            sp,
            M,
            cycles);

            fprintf(fp,"\t(%02X %02X %02X %02X)\n",
            read_byte(pc),
            read_byte(pc+1),
            read_byte(pc+2),
            read_byte(pc+3));
            //if (tc==0xc8c7fb8){exit(0);}
            fclose(fp);
        }

        if(cpm){
            if(pc==5){
                if(C==9){
                    int z=0;
                    while((char)memory[get_de()+z]!='$'){
                        cout<<((unsigned char)memory[get_de()+z]);
                        z++;
                    }
                    //cout<<"("<<tc<<")";
                }
                else if (C==2){
                    cout<<(char)E;
                }
                //cout<<dec<<tc<<hex<<endl;
            }
            if(pc==0){
                cout<<endl<<"exiting cpm"<<endl;
                exit(0);
            }
        }
        unsigned char opcode=next_byte();
        if(codes[opcode]==0){
            codes[opcode]=tc;
        }
        /*for (int i = 0; i < 256; ++i){
            if(codes[i]!=0){
                cout<<hex<<i<<" "<<dec<<codes[i]<<endl;
            }
        }*/
        run_op(opcode);
        cycles+=OPCODES_CYCLES[opcode];
        //if(tc==0x2000000){exit(5);}
        //cout<<pc<<"    "<<(int)regs[A]<<" "<<(int)regs[B]<<" "<<(int)regs[C]<<" "<<(int)regs[D]<<" "<<(int)regs[E]<<" "<<(int)regs[H]<<" "<<(int)regs[L]<<endl;

    }
    void run_innterrupt(unsigned char x){
        if(interrupts){
            run_op(x);
        }
    }
    void run_op(unsigned char opcode){
        //int d16=(op3<<8)+d8;
        cout<<hex;
        switch(opcode){
            case 0x0:if(debug){cout<<"NOP"<<endl;}
                break;
            case 0x1:if(debug){cout<<"LXI B "<<"d16"<<endl;}
                set_bc(next_word());
                break;
            case 0x2:if(debug){cout<<"STAX B "<<endl;}
                write_byte(get_bc(),A);
                break;
            case 0x3:if(debug){cout<<"INX B "<<endl;}
                set_bc(get_bc()+1);
                break;
            case 0x4:if(debug){cout<<"INR B "<<endl;}
                INR(B);
                break;
            case 0x5:if(debug){cout<<"DCR B "<<endl;}
                DCR(B);
                break;
            case 0x6:if(debug){cout<<"MVI B "<<"d8"<<endl;}
                B=next_byte();
                break;
            case 0x7:if(debug){cout<<"RLC "<<endl;}
                RLC(A);
                break;
            case 0x8:if(debug){cout<<"*NOP"<<endl;}
                break;
            case 0x9:if(debug){cout<<"DAD B"<<endl;}
                carry=(((int)get_hl()+get_bc())>0xffff);
                set_hl(get_hl()+get_bc());
                break;
            case 0xa:if(debug){cout<<"LDAX B "<<endl;}
                A=memory[get_bc()];
                break;
            case 0xb:if(debug){cout<<"DCX B "<<endl;}
                set_bc(get_bc()-1);
                break;
            case 0xc:if(debug){cout<<"INR C "<<endl;}
                INR(C);
                break;
            case 0xd:if(debug){cout<<"DCR C "<<endl;}
                DCR(C);
                break;
            case 0xe:if(debug){cout<<"MVI C "<<"d8"<<endl;}
                C=next_byte();
                break;
            case 0xf:if(debug){cout<<"RRC A "<<endl;}
                RRC(A);
                break;
            case 0x10:if(debug){cout<<"*NOP"<<endl;}
                break;
            case 0x11:if(debug){cout<<"LXI D "<<"d16"<<endl;}
                set_de(next_word());
                break;
            case 0x12:if(debug){cout<<"STAX D "<<endl;}
                write_byte(get_de(),A);
                break;
            case 0x13:if(debug){cout<<"INX D "<<endl;}
                set_de(get_de()+1);
                break;
            case 0x14:if(debug){cout<<"INR D "<<endl;}
                INR(D);
                break;
            case 0x15:if(debug){cout<<"DCR D "<<endl;}
                DCR(D);
                break;
            case 0x16:if(debug){cout<<"MVI D "<<"d8"<<endl;}
                D=next_byte();
                break;
            case 0x17:if(debug){cout<<"RAL  "<<endl;}
                RAL(A);
                break;
            case 0x18:if(debug){cout<<"*NOP"<<endl;}
                break;
            case 0x19:if(debug){cout<<"DAD D"<<endl;}
                carry=(((int)get_hl()+get_de())>0xffff);
                set_hl(get_hl()+get_de());
                break;
            case 0x1a:if(debug){cout<<"LDAX D "<<endl;}
                A=memory[get_de()];
                break;
            case 0x1b:if(debug){cout<<"DCX D "<<endl;}
                set_de(get_de()-1);
                break;
            case 0x1c:if(debug){cout<<"INR E "<<endl;}
                INR(E);
                break;
            case 0x1d:if(debug){cout<<"DCR E "<<endl;}
                DCR(E);
                break;
            case 0x1e:if(debug){cout<<"MVI E "<<"d8"<<endl;}
                E=next_byte();
                break;
            case 0x1f:if(debug){cout<<"RAR "<<endl;}
                RAR(A);
                break;
            case 0x20:if(debug){cout<<"*NOP"<<endl;}
                break;
            case 0x21:if(debug){cout<<"LXI H "<<"d16"<<endl;}
                set_hl(next_word());
                break;
            case 0x22:if(debug){cout<<"SHLD "<<"d16"<<endl;}
                write_word(next_word(),get_hl());
                break;
            case 0x23:if(debug){cout<<"INX H "<<endl;}
                set_hl(get_hl()+1);
                break;
            case 0x24:if(debug){cout<<"INR H "<<endl;}
                INR(H);
                break;
            case 0x25:if(debug){cout<<"DCR H "<<endl;}
                DCR(H);
                break;
            case 0x26:if(debug){cout<<"MVI H "<<"d8"<<endl;}
                H=next_byte();
                break;
            case 0x27:if (debug){cout<<"Daa"<<endl;}//Here be dragons
                DAA(A);
                break;
            case 0x28:if(debug){cout<<"*NOP"<<endl;}
                break;
            case 0x29:if(debug){cout<<"DAD H"<<endl;}
                carry=(((int)get_hl()*2)>0xffff);
                set_hl(get_hl()*2);
                break;
            case 0x2a:if(debug){cout<<"LHLD "<<"d16"<<endl;}
                set_hl(read_word(next_word()));
                break;
            case 0x2b:if(debug){cout<<"DCX H "<<endl;}
                set_hl(get_hl()-1);
                break;
            case 0x2c:if(debug){cout<<"INR L "<<endl;}
                INR(L);
                break;
            case 0x2d:if(debug){cout<<"DCR L "<<endl;}
                DCR(L);
                break;
            case 0x2e:if(debug){cout<<"MVI L "<<"d8"<<endl;}
                L=next_byte();
                break;
            case 0x2f:if(debug){cout<<"CMA "<<endl;}
                A^=0xff;
                break;
            case 0x30:if(debug){cout<<"*NOP"<<endl;}
                break;
            case 0x31:if(debug){cout<<"Lxi SP "<<"d16"<<endl;}
                sp=next_word();
                break;
            case 0x32:if(debug){cout<<"STA "<<"d16"<<endl;}
                write_byte(next_word(),A);
                break;
            case 0x33:if(debug){cout<<"INX SP "<<endl;}
                sp++;
                break;
            case 0x34:if(debug){cout<<"INR M "<<endl;}
                INR(M);
                break;
            case 0x35:if(debug){cout<<"DCR M "<<endl;}
                DCR(M);
                break;
            case 0x36:if(debug){cout<<"MVI M "<<"d8"<<endl;}
                write_byte(get_hl(),next_byte());
                break;
            case 0x37:if(debug){cout<<"STC "<<endl;}
                carry=true;
                break;
            case 0x38:if(debug){cout<<"*NOP"<<endl;}
                break;
            case 0x39:if(debug){cout<<"DAD SP"<<endl;}
                carry=(((int)get_hl()+sp)>0xffff);
                set_hl(get_hl()+sp);
                break;
            case 0x3a:if(debug){cout<<"LDA "<<"d16"<<endl;}
                A=memory[next_word()];
                break;
            case 0x3b:if(debug){cout<<"DCX SP "<<endl;}
                sp--;
                break;
            case 0x3c:if(debug){cout<<"INR A "<<endl;}
                INR(A);
                break;
            case 0x3d:if(debug){cout<<"DCR A "<<endl;}
                DCR(A);
                break;
            case 0x3e:if(debug){cout<<"MVI A "<<"d8"<<endl;}
                A=next_byte();
                break;
            case 0x3f:if(debug){cout<<"CMC "<<endl;}
                carry=!carry;
                break;
            case 0x40:if(debug){cout<<"MOV B,B "<<endl;}
                break;
            case 0x41:if(debug){cout<<"MOV B,B "<<endl;}
                MOV(B,C);
                break;
            case 0x42:if(debug){cout<<"MOV B,D "<<endl;}
                MOV(B,D);
                break;
            case 0x43:if(debug){cout<<"MOV B,E "<<endl;}
                MOV(B,E);
                break;
            case 0x44:if(debug){cout<<"MOV B,H "<<endl;}
                MOV(B,H);
                break;
            case 0x45:if(debug){cout<<"MOV B,L "<<endl;}
                MOV(B,L);
                break;
            case 0x46:if(debug){cout<<"MOV B,M "<<endl;}
                MOV(B,M);
                break;
            case 0x47:if(debug){cout<<"MOV B,A "<<endl;}
                MOV(B,A);
                break;
            case 0x48:if(debug){cout<<"MOV C,B "<<endl;}
                MOV(C,B);
                break;
            case 0x49:if(debug){cout<<"MOV C,C "<<endl;}
                break;
            case 0x4a:if(debug){cout<<"MOV C,D "<<endl;}
                MOV(C,D);
                break;
            case 0x4b:if(debug){cout<<"MOV C,E "<<endl;}
                MOV(C,E);
                break;
            case 0x4c:if(debug){cout<<"MOV C,H "<<endl;}
                MOV(C,H);
                break;
            case 0x4d:if(debug){cout<<"MOV C,L "<<endl;}
                MOV(C,L);
                break;
            case 0x4e:if(debug){cout<<"MOV C,M "<<endl;}
                MOV(C,M);
                break;
            case 0x4f:if(debug){cout<<"MOV C,A "<<endl;}
                MOV(C,A);
                break;

            case 0x50:if(debug){cout<<"MOV D,B "<<endl;}
                MOV(D,B);
                break;
            case 0x51:if(debug){cout<<"MOV D,B "<<endl;}
                MOV(D,C);
                break;
            case 0x52:if(debug){cout<<"MOV D,D "<<endl;}
                break;
            case 0x53:if(debug){cout<<"MOV D,E "<<endl;}
                MOV(D,E);
                break;
            case 0x54:if(debug){cout<<"MOV D,H "<<endl;}
                MOV(D,H);
                break;
            case 0x55:if(debug){cout<<"MOV D,L "<<endl;}
                MOV(D,L);
                break;
            case 0x56:if(debug){cout<<"MOV D,M "<<endl;}
                MOV(D,M);
                break;
            case 0x57:if(debug){cout<<"MOV D,A "<<endl;}
                MOV(D,A);
                break;
            case 0x58:if(debug){cout<<"MOV E,B "<<endl;}
                MOV(E,B);
                break;
            case 0x59:if(debug){cout<<"MOV E,C "<<endl;}
                MOV(E,C);
                break;
            case 0x5a:if(debug){cout<<"MOV E,D "<<endl;}
                MOV(E,D);
                break;
            case 0x5b:if(debug){cout<<"MOV E,E "<<endl;}
                break;
            case 0x5c:if(debug){cout<<"MOV E,H "<<endl;}
                MOV(E,H);
                break;
            case 0x5d:if(debug){cout<<"MOV E,L "<<endl;}
                MOV(E,L);
                break;
            case 0x5e:if(debug){cout<<"MOV E,M "<<endl;}
                MOV(E,M);
                break;
            case 0x5f:if(debug){cout<<"MOV E,A "<<endl;}
                MOV(E,A);
                break;

            case 0x60:if(debug){cout<<"MOV H,B "<<endl;}
                MOV(H,B);
                break;
            case 0x61:if(debug){cout<<"MOV H,B "<<endl;}
                MOV(H,C);
                break;
            case 0x62:if(debug){cout<<"MOV H,D "<<endl;}
                MOV(H,D);
                break;
            case 0x63:if(debug){cout<<"MOV H,E "<<endl;}
                MOV(H,E);
                break;
            case 0x64:if(debug){cout<<"MOV H,H "<<endl;}
                break;
            case 0x65:if(debug){cout<<"MOV H,L "<<endl;}
                MOV(H,L);
                break;
            case 0x66:if(debug){cout<<"MOV H,M "<<endl;}
                MOV(H,M);
                break;
            case 0x67:if(debug){cout<<"MOV H,A "<<endl;}
                MOV(H,A);
                break;
            case 0x68:if(debug){cout<<"MOV L,B "<<endl;}
                MOV(L,B);
                break;
            case 0x69:if(debug){cout<<"MOV L,C "<<endl;}
                MOV(L,C);
                break;
            case 0x6a:if(debug){cout<<"MOV L,D "<<endl;}
                MOV(L,D);
                break;
            case 0x6b:if(debug){cout<<"MOV L,E "<<endl;}
                MOV(L,E);
                break;
            case 0x6c:if(debug){cout<<"MOV L,H "<<endl;}
                MOV(L,H);
                break;
            case 0x6d:if(debug){cout<<"MOV L,L "<<endl;}
                break;
            case 0x6e:if(debug){cout<<"MOV L,M "<<endl;}
                MOV(L,M);
                break;
            case 0x6f:if(debug){cout<<"MOV L,A "<<endl;}
                MOV(L,A);
                break;

            case 0x70:if(debug){cout<<"MOV M,B "<<endl;}
                write_byte(get_hl(),B);
                break;
            case 0x71:if(debug){cout<<"MOV M,B "<<endl;}
                write_byte(get_hl(),C);
                break;
            case 0x72:if(debug){cout<<"MOV M,D "<<endl;}
                write_byte(get_hl(),D);
                break;
            case 0x73:if(debug){cout<<"MOV M,E "<<endl;}
                write_byte(get_hl(),E);
                break;
            case 0x74:if(debug){cout<<"MOV M,H "<<endl;}
                write_byte(get_hl(),H);
                break;
            case 0x75:if(debug){cout<<"MOV M,L "<<endl;}
                write_byte(get_hl(),L);
                break;
            case 0x76:if(debug){cout<<"MOV M,M "<<endl;}
                break;
            case 0x77:if(debug){cout<<"MOV M,A "<<endl;}
                write_byte(get_hl(),A);
                break;
            case 0x78:if(debug){cout<<"MOV A,B "<<endl;}
                MOV(A,B);
                break;
            case 0x79:if(debug){cout<<"MOV A,C "<<endl;}
                MOV(A,C);
                break;
            case 0x7a:if(debug){cout<<"MOV A,D "<<endl;}
                MOV(A,D);
                break;
            case 0x7b:if(debug){cout<<"MOV A,E "<<endl;}
                MOV(A,E);
                break;
            case 0x7c:if(debug){cout<<"MOV A,H "<<endl;}
                MOV(A,H);
                break;
            case 0x7d:if(debug){cout<<"MOV A,L "<<endl;}
                MOV(A,L);
                break;
            case 0x7e:if(debug){cout<<"MOV A,M "<<endl;}
                MOV(A,M);
                break;
            case 0x7f:if(debug){cout<<"MOV A,A "<<endl;}
                break;

            case 0x80:if(debug){cout<<"ADD A B "<<endl;}
                ADD(A,B);
                break;
            case 0x81:if(debug){cout<<"ADD A C "<<endl;}
                ADD(A,C);
                break;
            case 0x82:if(debug){cout<<"ADD A D "<<endl;}
                ADD(A,D);
                break;
            case 0x83:if(debug){cout<<"ADD A E "<<endl;}
                ADD(A,E);
                break;
            case 0x84:if(debug){cout<<"ADD A H "<<endl;}
                ADD(A,H);
                break;
            case 0x85:if(debug){cout<<"ADD A L "<<endl;}
                ADD(A,L);
                break;
            case 0x86:if(debug){cout<<"ADD A M "<<endl;}
                ADD(A,M);
                break;
            case 0x87:if(debug){cout<<"ADD A A "<<endl;}
                ADD(A,A);
                break;

            case 0x88:if(debug){cout<<"ADC A B "<<endl;}
                ADC(A,B);
                break;
            case 0x89:if(debug){cout<<"ADC A C "<<endl;}
                ADC(A,C);
                break;
            case 0x8a:if(debug){cout<<"ADC A D "<<endl;}
                ADC(A,D);
                break;
            case 0x8b:if(debug){cout<<"ADC A E "<<endl;}
                ADC(A,E);
                break;
            case 0x8c:if(debug){cout<<"ADC A H "<<endl;}
                ADC(A,H);
                break;
            case 0x8d:if(debug){cout<<"ADC A L "<<endl;}
                ADC(A,L);
                break;
            case 0x8e:if(debug){cout<<"ADC A M "<<endl;}
                ADC(A,M);
                break;
            case 0x8f:if(debug){cout<<"ADC A A "<<endl;}
                ADC(A,A);
                break;

            case 0x90:if(debug){cout<<"SUB A B "<<endl;}
                SUB(A,B);
                break;
            case 0x91:if(debug){cout<<"SUB A C "<<endl;}
                SUB(A,C);
                break;
            case 0x92:if(debug){cout<<"SUB A D "<<endl;}
                SUB(A,D);
                break;
            case 0x93:if(debug){cout<<"SUB A E "<<endl;}
                SUB(A,E);
                break;
            case 0x94:if(debug){cout<<"SUB A H "<<endl;}
                SUB(A,H);
                break;
            case 0x95:if(debug){cout<<"SUB A L "<<endl;}
                SUB(A,L);
                break;
            case 0x96:if(debug){cout<<"SUB A M "<<endl;}
                SUB(A,M);
                break;
            case 0x97:if(debug){cout<<"SUB A A "<<endl;}
                SUB(A,A);
                break;

            case 0x98:if(debug){cout<<"SBB A B "<<endl;}
                SBB(A,B);
                break;
            case 0x99:if(debug){cout<<"SBB A C "<<endl;}
                SBB(A,C);
                break;
            case 0x9a:if(debug){cout<<"SBB A D "<<endl;}
                SBB(A,D);
                break;
            case 0x9b:if(debug){cout<<"SBB A E "<<endl;}
                SBB(A,E);
                break;
            case 0x9c:if(debug){cout<<"SBB A H "<<endl;}
                SBB(A,H);
                break;
            case 0x9d:if(debug){cout<<"SBB A L "<<endl;}
                SBB(A,L);
                break;
            case 0x9e:if(debug){cout<<"SBB A M "<<endl;}
                SBB(A,M);
                break;
            case 0x9f:if(debug){cout<<"SBB A A "<<endl;}
                SBB(A,A);
                break;

            case 0xa0:if(debug){cout<<"ANA A B "<<endl;}
                ANA(A,B);
                break;
            case 0xa1:if(debug){cout<<"ANA A C "<<endl;}
                ANA(A,C);
                break;
            case 0xa2:if(debug){cout<<"ANA A D "<<endl;}
                ANA(A,D);
                break;
            case 0xa3:if(debug){cout<<"ANA A E "<<endl;}
                ANA(A,E);
                break;
            case 0xa4:if(debug){cout<<"ANA A H "<<endl;}
                ANA(A,H);
                break;
            case 0xa5:if(debug){cout<<"ANA A L "<<endl;}
                ANA(A,L);
                break;
            case 0xa6:if(debug){cout<<"ANA A M "<<endl;}
                ANA(A,M);
                break;
            case 0xa7:if(debug){cout<<"ANA A A "<<endl;}
                ANA(A,A);
                break;

            case 0xa8:if(debug){cout<<"XRA A B "<<endl;}
                XRA(A,B);
                break;
            case 0xa9:if(debug){cout<<"XRA A C "<<endl;}
                XRA(A,C);
                break;
            case 0xaa:if(debug){cout<<"XRA A D "<<endl;}
                XRA(A,D);
                break;
            case 0xab:if(debug){cout<<"XRA A E "<<endl;}
                XRA(A,E);
                break;
            case 0xac:if(debug){cout<<"XRA A H "<<endl;}
                XRA(A,H);
                break;
            case 0xad:if(debug){cout<<"XRA A L "<<endl;}
                XRA(A,L);
                break;
            case 0xae:if(debug){cout<<"XRA A M "<<endl;}
                XRA(A,M);
                break;
            case 0xaf:if(debug){cout<<"XRA A A "<<endl;}
                XRA(A,A);
                break;

            case 0xb0:if(debug){cout<<"ORA A B "<<endl;}
                ORA(A,B);
                break;
            case 0xb1:if(debug){cout<<"ORA A C "<<endl;}
                ORA(A,C);
                break;
            case 0xb2:if(debug){cout<<"ORA A D "<<endl;}
                ORA(A,D);
                break;
            case 0xb3:if(debug){cout<<"ORA A E "<<endl;}
                ORA(A,E);
                break;
            case 0xb4:if(debug){cout<<"ORA A H "<<endl;}
                ORA(A,H);
                break;
            case 0xb5:if(debug){cout<<"ORA A L "<<endl;}
                ORA(A,L);
                break;
            case 0xb6:if(debug){cout<<"ORA A M "<<endl;}
                ORA(A,M);
                break;
            case 0xb7:if(debug){cout<<"ORA A A "<<endl;}
                ORA(A,A);
                break;

            case 0xb8:if(debug){cout<<"CMP A B "<<endl;}
                CMP(A,B);
                break;
            case 0xb9:if(debug){cout<<"CMP A C "<<endl;}
                CMP(A,C);
                break;
            case 0xba:if(debug){cout<<"CMP A D "<<endl;}
                CMP(A,D);
                break;
            case 0xbb:if(debug){cout<<"CMP A E "<<endl;}
                CMP(A,E);
                break;
            case 0xbc:if(debug){cout<<"CMP A H "<<endl;}
                CMP(A,H);
                break;
            case 0xbd:if(debug){cout<<"CMP A L "<<endl;}
                CMP(A,L);
                break;
            case 0xbe:if(debug){cout<<"CMP A M "<<endl;}
                CMP(A,M);
                break;
            case 0xbf:if(debug){cout<<"CMP A A "<<endl;}
                CMP(A,A);
                break;


            case 0xc0:if(debug){cout<<"RNZ "<<endl;}
                cond_ret(!zero);
                break;
            case 0xc1:if(debug){cout<<"POP B "<<endl;}
                set_bc(POP());
                break;
            case 0xc2:if(debug){cout<<"JNZ "<<"d16"<<endl;}
                cond_jump(!zero);
                break;
            case 0xc3:if(debug){cout<<"JMP "<<"d16"<<endl;}
                jump(next_word());
                break;
            case 0xc4:if(debug){cout<<"CNZ "<<"d16"<<endl;}
                cond_call(!zero);
                break;
            case 0xc5:if(debug){cout<<"PUSH B "<<endl;}
                PUSH(get_bc());
                break;
            case 0xc6:if(debug){cout<<"ADI A "<<"d8"<<endl;}
                ADD(A,next_byte());
                break;
            case 0xc7:if(debug){cout<<"Rst 0"<<endl; }
                call(0x0);
                break;
            case 0xc8:if(debug){cout<<"RZ "<<endl;}
                cond_ret(zero);
                break;
            case 0xc9:if(debug){cout<<"Ret "<<endl;}
                ret();
                break;
            case 0xca:if(debug){cout<<"JZ "<<"d16"<<endl;}
                cond_jump(zero);
                break;
            case 0xcb:if(debug){cout<<"*JMP "<<"d16"<<endl;}
                jump(next_word());
                break;
            case 0xcc:if(debug){cout<<"CZ "<<"d16"<<endl;}
                cond_call(zero);
                break;
            case 0xcd:if(debug){cout<<"CALL "<<"d16"<<endl;}
                call(next_word());
                break;
            case 0xce:if(debug){cout<<"ACI A "<<"d8"<<endl;}
                ADC(A,next_byte());
                break;
            case 0xcf:if(debug){cout<<"Rst 8"<<endl; }
                call(0x8);
                break;
            case 0xd0:if(debug){cout<<"RNC "<<endl;}
                cond_ret(!carry);
                break;
            case 0xd1:if(debug){cout<<"POP D "<<endl;}
                set_de(POP());
                break;
            case 0xd2:if(debug){cout<<"JNC "<<"d16"<<endl;}
                cond_jump(!carry);
                break;
            case 0xd3:if(debug){cout<<"OUT (later)"<<"d8"<<endl;}
                out(A,next_byte());
                break;
            case 0xd4:if(debug){cout<<"CNC "<<"d16"<<endl;}
                cond_call(!carry);
                break;
            case 0xd5:if(debug){cout<<"PUSH D "<<endl;}
                PUSH(get_de());
                break;
            case 0xd6:if(debug){cout<<"SUI A "<<"d8"<<endl;}
                SUB(A,next_byte());
                break;
            case 0xd7:if(debug){cout<<"Rst 10"<<endl; }
                call(0x10);
                break;
            case 0xd8:if(debug){cout<<"RC "<<endl;}
                cond_ret(carry);
                break;

            case 0xda:if(debug){cout<<"JC "<<"d16"<<endl;}
                cond_jump(carry);
                break;
            case 0xdb:if(debug){cout<<"IN (later) "<<"d8"<<endl; }
                if(!cpm){
                   A=in(next_byte()); 
                }
                else{
                    pc++;
                }
                break;
            case 0xdc:if(debug){cout<<"CC "<<"d16"<<endl;}
                cond_call(carry);
                break;
            case 0xde:if(debug){cout<<"SBI A "<<"d8"<<endl;}
                SBB(A,next_byte());
                break;
            case 0xdf:if(debug){cout<<"Rst 18"<<endl; }
                call(0x18);
                break;
            case 0xe0:if(debug){cout<<"RPO "<<endl;}
                cond_ret(!parity);
                break;
            case 0xe1:if(debug){cout<<"POP H "<<endl;}
                set_hl(POP());
                break;
            case 0xe2:if(debug){cout<<"JPO "<<"d16"<<endl;}
                cond_jump(!parity);
                break;
            case 0xe3:if(debug){cout<<"XTHL "<<endl;}{
                unsigned short temp=POP();
                PUSH(get_hl());
                set_hl(temp);
            }
                break;
            case 0xe4:if(debug){cout<<"CPO "<<"d16"<<endl;}
                cond_call(!parity);
                break;
            case 0xe5:if(debug){cout<<"PUSH H "<<endl;}
                PUSH(get_hl());
                break;
            case 0xe6:if(debug){cout<<"ANI A "<<"d8"<<endl;}
                ANA(A,next_byte());
                break;
            case 0xe7:if(debug){cout<<"Rst 20"<<endl; }
                call(0x20);
                break;
            case 0xe8:if(debug){cout<<"RPE "<<endl;}
                cond_ret(parity);
                break;
            case 0xe9:if(debug){cout<<"PCHL "<<endl;}
                pc=(get_hl());
                break;
            case 0xea:if(debug){cout<<"JPE "<<"d16"<<endl;}
                cond_jump(parity);
                break;
            case 0xeb:if(debug){cout<<"XCHG "<<endl;}{
                unsigned short temp=get_de();
                set_de(get_hl());
                set_hl(temp);}
                break;
            case 0xec:if(debug){cout<<"CPE "<<"d16"<<endl;}
                cond_call(parity);
                break;

            case 0xee:if(debug){cout<<"XRI A "<<"d8"<<endl;}
                XRA(A,next_byte());
                break;
            case 0xef:if(debug){cout<<"Rst 28"<<endl; }
                call(0x28);
                break;
            case 0xf0:if(debug){cout<<"RNZ "<<endl;}
                cond_ret(!sign);
                break;
            case 0xf1:if(debug){cout<<"POP PSW "<<endl;}
                set_af(POP());
                break;
            case 0xf2:if(debug){cout<<"JP "<<"d16"<<endl;}
                cond_jump(!sign);
                break;
            case 0xf3:if(debug){cout<<"DI "<<endl;}
                interrupts=false;
                break;
            case 0xf4:if(debug){cout<<"CP "<<"d16"<<endl;}
                cond_call(!sign);
                break;
            case 0xf5:if(debug){cout<<"PUSH PSW "<<endl;}
                PUSH(get_af());
                break;
            case 0xf6:if(debug){cout<<"ORA A "<<"d8"<<endl;}
                ORA(A,next_byte());
                break;
            case 0xf7:if(debug){cout<<"Rst 30"<<endl; }
                call(0x30);
                break;
            case 0xf8:if(debug){cout<<"RNZ "<<endl;}
                cond_ret(sign);
                break;
            case 0xf9:if(debug){cout<<"SPHL "<<endl;}
                sp=get_hl();
                break;
            case 0xfa:if(debug){cout<<"JM "<<"d16"<<endl;}
                cond_jump(sign);
                break;
            case 0xfb:if(debug){cout<<"EI "<<endl;}
                interrupts=true;
                break;
            case 0xfc:if(debug){cout<<"CM "<<"d16"<<endl;}
                cond_call(sign);
                break;

            case 0xfe:if(debug){cout<<"CPI "<<"d8"<<endl;}
                CMP(A,next_byte());
                break;
            case 0xff:if(debug){cout<<"Rst 38"<<endl; }
                call(0x38);
                break;
            default:cout<<"opcode not implemented "<<(int)opcode<<endl; exit(0x15);
        }
    }
};
