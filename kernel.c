#define VIDEO	( (volatile char *) 0x000B8000 ) /* video memory address */
#define COLS	80 // width
#define ROWS	24 // height
#define COLR	0x0A  // color, 7=green char on black 

int CURR_LOC=0;

char gethjkl();
void kclear_screen();
void kprint();
void kputchar(char s);
void scrollLine();
void kclear_screen();
void kprintint(int i);
int strlen(char *s);
int pow(int i, int p);
void wumpus();
unsigned char * memcpy(unsigned char *dest, const unsigned char *src, int count);
unsigned char *memset(unsigned char *dest, unsigned char val, int count);
unsigned short *memsetw(unsigned short *dest, unsigned short val, int count);
unsigned char inportb (unsigned short _port);
void outportb (unsigned short _port, unsigned char _data);
void present();

volatile char * CURS = VIDEO; //+ ROWS*COLS*2;

int rand_seed=0; //semi-random number generated based on 
                 //gethjkl() waiting time

void kmain (){ 
	char hello[] = "Hello World!";
    char l2[]="Look at me!\nI can scroll!";

    kclear_screen();
    kclear_screen();
    present();
    kclear_screen();
    kprint(hello);
    kprint(l2);
    int i=0;
    int j=1;
    kprintint(i);
    kputchar('\n');
    while(j <10000){
        kprintint(j);
        kputchar('\n');
        j=(i+(i=j));
    }
    char a = 97;
    kputchar(a);
    a++;
    kputchar(a);
    a++;
    kputchar(a);
    kputchar('\n');
    kprint("press h");
    gethjkl();
    kprint("wumpus time...");
    kprintint(rand_seed);
    wumpus();
 }   

int strlen(char * s){
    //counts the elems of a string
    int i;
    for (i=0;*s!=0;s++) i++;
    return i;
}

unsigned char * memcpy(unsigned char *dest, const unsigned char *src, int count){
    //simple memcpy implementation
    for (;count>0;count--) *(dest+count-1)=*(src+count-1);
    return dest;
}

unsigned char *memset(unsigned char *dest, unsigned char val, int count){
    //simple memset implementation
    for (;count>0;count--) *(dest+count-1)=val;
    return dest;
}

unsigned short *memsetw(unsigned short *dest, unsigned short val, int count){
    //memset using short vals
    for (;count>0;count--) *(dest+count-1)=val;
    return dest;
}


unsigned char inportb (unsigned short _port){
    //reads from a given port
    unsigned char rv;
    __asm__ __volatile__ ("inb %1, %0" : "=a" (rv) : "dN" (_port));
    return rv;
}

void outportb (unsigned short _port, unsigned char _data){
    //writes to a given port
    __asm__ __volatile__ ("outb %1, %0" : : "dN" (_port), "a" (_data));
}

void kclear_screen(){
    //clears the screen
    char * p;
    for(p=(char *)VIDEO;p<VIDEO+(COLS*ROWS*2);p+=2) *p=*(p+1)=0;
}

void kprint(char *s){ 
    //prints a line to the screen

    int i;
    
    if (CURR_LOC != 0) scrollLine();
    CURR_LOC=0;

    for(i=0;*s != 0;s++){
        if( *s=='\n') {
            scrollLine();
            i=0;
            continue;
    
        } 
         
        if(i >= COLS){
            scrollLine();
            i=0;
        }
        *(VIDEO+(i+ROWS*COLS)*2)=*s;
        *(VIDEO+(i+ROWS*COLS)*2+1)=COLR;
        i++;
      }; 
    scrollLine();
}  

void kputchar(char s){
    //puts a char to the screen at CURR_LOC
       if (s =='\n'){
            scrollLine();
            CURR_LOC=0;
            return;
       }
       if (CURR_LOC >= ROWS){
           scrollLine();
           CURR_LOC =0;
       }
       *(VIDEO+(CURR_LOC+ROWS*COLS)*2)=s;
       *(VIDEO+(CURR_LOC+ROWS*COLS)*2+1)=COLR;
       CURR_LOC++;
}

void kprintint(int i){
    //converts an int to a string and prints it
    int p=1;
    char e= (i%10) ? 0: 1;
    for (p=1;i/p>10;p*=10);
    while (i){
        kputchar(48+(i/p));
        i%=p;
        p/=10;
    }
    if (e) kputchar(48);
} 

void scrollLine(){
    //moves everything on screen up one row
    int i,j;
    for(i=0;i<=ROWS;i++){
        for(j=0;j<=COLS;j++){
            *(VIDEO+((i*COLS+j)*2)) = *(VIDEO+(((i+1)*COLS+j)*2));
            *(VIDEO+((i*COLS+j)*2)+1) = *(VIDEO+(((i+1)*COLS+j)*2)+1);
        }
    }

    for(i=0;i<=ROWS;i++){
        *(VIDEO+(i+ROWS*COLS)*2)=*(VIDEO+(i+ROWS*COLS)*2+1)=0;
    }
}

int pow(int i, int p){
    // returns i to the power of p
    for(;p>0;p--) i*=i;
    return i;
}

char gethjkl(){
    //blocks and waits for h,j,k, or l to be pressed then released
    char k=0;
    while (k<0x23 || k>0x26){
        k=inportb(0x60);
        rand_seed*=274876858367;
    }
    while (inportb(0x60) != (0xA3-0x23)+k ) rand_seed+=274876858367;
    k-=0x23;
    k="hjkl"[(int)k];
    return k;
}

void wumpus(){
    //plays a game of hunt the wumpus on a char sized grid
    char player = rand_seed & 0x0F;
    char wump = (rand_seed & 0xF0)>> 4;
    char pit[6];
    int i;
    for (i=0;i<6;i++) pit[i] = (rand_seed & (0xF << (8+4*i))) >> (8+4*i);
    char action;
    char direction; 
    char arrows=5;
    for(;;){
        if( player+1==wump || player-1==wump || player+32==wump ||player-32==wump) kprint("you smell a wumpus...");
        for(i=0; i<6; i++){
            if( player+1==pit[i] || player-1==pit[i] || player+32==pit[1] ||player-32==pit[i]) kprint("you feel a breeze...");
        }
        kprint("move (hj) or shoot (kl)?");
        action = gethjkl();
        if (action=='h' || action=='j'){
            kprint("where to?");
            direction=gethjkl();
            if (direction=='h')player--;
            else if (direction=='j')player-=32;
            else if (direction=='k')player+=32;
            else if (direction=='l')player++;
        } else {
            kprint("shoot where?");
            arrows--;
            direction=gethjkl();
            if (direction=='h') i=player-1;
            else if (direction=='j') i=player-32;
            else if (direction=='k') i=player+32;
            else if (direction=='l') i=player+1;
            if (i==wump) {
                kprint("You kill the wumpus! PETA would be so proud...");
                return;
            }
        }
        if (arrows <5){
            i=rand_seed&0x3;
            if (i==0) wump--;
            else if (i==1) wump-=32;
            else if (i==2) wump+=32;
            else if (i==3) wump++;
        }
        if (wump==player){
            kprint("You are eaten by the wumpus... harsh...");
            return;
        }
        for (i=0;i<6;i++){
            if (player==pit[i]){
                kprint("You fall in a pit and die... lame...");
                return;
            }
        }
        if (arrows<=0){
            kprint("you are out of arrows and will surely die...");
            return;
        }
    }
}

void trans(){
    gethjkl();
    kclear_screen();
}

void present(){

    int i;
    //slide 1
    kprint("         wumpOS         ");
    kprint("      Aaron Laursen     ");
    for(i=0;i<10;i++) scrollLine();

    trans();

    //slide 2
    kprint("What is it?");
    kprint("===========");
    kprint("");
    kprint("- Very Basic Operating System");
    kprint("  - prints to screen");
    kprint("  - claculates Fibinacci numbers");
    kprint("  - reads keyboard input (only 4 letters)");
    for(i=0;i<10;i++) scrollLine();
    trans();

    //slide 3
    kprint("Why is it?");
    kprint("==========");
    kprint("- Operating systems are cool");
    kprint("- Learn about low-level OS");
    kprint("- Thematic appropriateness");
    for(i=0;i<10;i++) scrollLine();
    trans();

    //slide 4
    kprint("=========");
    kprint("= DEMO! =");
    kprint("=========");
    for(i=0;i<10;i++) scrollLine();
    trans();

}
