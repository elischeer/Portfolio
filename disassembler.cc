#include <stdio.h>
#include <string.h>
#define BASESRCREG 0x01C0
#define LIMIT 40
#define PCOFFSET9 0x01FF
#define DESTREG 0x0E00

void printAssembly(char filename[]);
void printBr(int, int);
void printAdd(int);
void printLd(int, int);
void printSt(int, int);
void printJsrJsrr(int, int);
void printAnd(int);
void printLdr(int);
void printStr(int);
void printRti(int);
void printNot(int);
void printLdi(int, int);
void printSti(int, int);
void printJmpRet(int);
void printLea(int, int);
void printTrap(int);

//ent main out before submitting
//int main()
//{
//	char filename[] = "test1.hex";
//	printAssembly(filename);
//}

void printAssembly(char filename[])
{
	FILE *infile;	
	infile = fopen(filename, "r");
	
	if (infile == NULL)	{
		printf("File %s is null.\n", filename);			
	} else {
    int count = 0;
    int instruction = 0;
    int pc = 0;
    //Read the initial PC as an integer and save
    fscanf(infile, "%x", &pc);
    //While not at end of file and not over the limit
    while (fscanf(infile, "%x", &instruction) != EOF && count < LIMIT)
    {
      count++;  //Count number of lines for limit
      pc++; //Increment the PC
      int opc = instruction >> 12; //Figure out the oppcode from the high order 4 bits of the instruction
      int instTrimOpc = instruction & 0x0FFF;
      switch(opc)
      {
          case 0:
          printBr(instTrimOpc, pc);
          break;
          case 1:
          printAdd(instTrimOpc);
          break;
          case 2:
          printLd(instTrimOpc, pc);
          break;
          case 3:
          printSt(instTrimOpc, pc);
          break;
          case 4:
          printJsrJsrr(instTrimOpc, pc);
          break;
          case 5:
          printAnd(instTrimOpc);
          break;
          case 6:
          printLdr(instTrimOpc);
          break;
          case 7:
          printStr(instTrimOpc);
          break;
          case 8:
          printRti(instTrimOpc);
          break;
          case 9:
          printNot(instTrimOpc);
          break;
          case 10:
          printLdi(instTrimOpc, pc);
          break;
          case 11:
          printSti(instTrimOpc, pc);
          break;
          case 12:
          printJmpRet(instTrimOpc);
          break;
          case 13:
          break;
          case 14:
          printLea(instTrimOpc, pc);
          break;
          case 15:
          printTrap(instTrimOpc);
          break;
          default:
          break;
      }
    }
  }
}

void printBr(int instruction, int pc)
{
    int neg = (instruction & 0x0800) >> 11;
    int zer = (instruction & 0x0400) >> 10;
    int pos = (instruction & 0x0200) >> 9;
    int ofst = (instruction & PCOFFSET9) << 23;
    ofst = ofst >> 23;
    printf("BR");
    if(neg)
        printf("N");
    if(zer)
        printf("Z");
    if(pos)
        printf("P");
        printf("\tx%X\n", pc + ofst);
}

void printAdd(int instruction)
{
    int destinRegis = (instruction & DESTREG) >> 9;
    int sourceRegOne = (instruction & BASESRCREG) >> 6;
    printf("ADD\tR%d, R%d, ",destinRegis, sourceRegOne);
    if(instruction & 0x0020)
    {
        int immed = (instruction & 0x0001F) << 27;
        immed = immed >> 27;
        printf("#%d\n", immed);
    }
    else
    {
        int sourceRegTwo = instruction & 0x0007;
        printf("R%d\n", sourceRegTwo);
    }
}
void printLd(int instruction, int pc)
{
    int destinRegis = (instruction & DESTREG) >> 9;
    int ofst = (instruction & PCOFFSET9) << 23;
    ofst = ofst >> 23;
    printf("LD\tR%d, x%X\n", destinRegis, pc + ofst);
}

void printSt(int instruction, int pc)
{
    int destinRegis = (instruction & DESTREG) >> 9;
    int ofst = (instruction & PCOFFSET9) << 23;
    ofst = ofst >> 23;
    printf("ST\tR%d, x%X\n", destinRegis, pc + ofst);
}

void printJsrJsrr(int instruction, int pc)
{
    if(instruction >> 11)
    {
        int ofst = (instruction & 0x07FF) << 21;
        ofst = ofst >> 21;
        printf("JSR\tx%X\n", pc + ofst);
    }
    else
    {
        int baseRegis = (instruction & BASESRCREG) >> 6;
        printf("JSRR\tR%d\n", baseRegis);
    }
}

void printAnd(int instruction)
{
    int destinRegis = (instruction & DESTREG) >> 9;
    int sourceRegOne = (instruction & BASESRCREG) >> 6;
    if((instruction & 0x0020) >> 5)
    {
        int immed = (instruction & 0x001F) << 27;
        immed = immed >> 27;
        printf("AND\tR%d, R%d, #%d\n", destinRegis, sourceRegOne, immed);
    }
    else
    {
        int sourceRegTwo = instruction & 0x0007;
        printf("AND\tR%d, R%d, R%d\n", destinRegis, sourceRegOne, sourceRegTwo);
    }
}
void printLdr(int instruction)
{
    int destinRegis = (instruction & DESTREG) >> 9;
    int baseRegis = (instruction & BASESRCREG) >> 6;
    int ofst = (instruction & 0x003F) << 26;
    ofst = ofst >> 26;
    printf("LDR\tR%d, R%d, #%d\n", destinRegis, baseRegis, ofst);
}

void printStr(int instruction)
{
    int sourceRegis = (instruction & DESTREG) >> 9;
    int baseRegis = (instruction & BASESRCREG) >> 6;
    int ofst = (instruction & 0x003F) << 26;
    ofst = ofst >> 26;
    printf("STR\tR%d, R%d, #%d\n", sourceRegis, baseRegis, ofst);
}

void printRti(int instruction)
{
    printf("RTI\n");
}

void printNot(int instruction)
{
    int destinRegis = (instruction & DESTREG) >> 9;
    int sourceRegis = (instruction & BASESRCREG) >> 6;
    printf("NOT\tR%d, R%d\n", destinRegis, sourceRegis);
}

void printLdi(int instruction, int pc)
{
    int destinRegis = (instruction & DESTREG) >> 9;
    int ofst = (instruction & PCOFFSET9) << 23;
    ofst = ofst >> 23;
    printf("LDI\tR%d, x%X\n", destinRegis, ofst);
}

void printSti(int instruction, int pc)
{
    int sourceRegis = (instruction & DESTREG) >> 9;
    int ofst = (instruction & PCOFFSET9) << 23;
    ofst = ofst >> 23;
    printf("STI\tR%d, x%X\n", sourceRegis, pc + ofst);
}

void printJmpRet(int instruction)
{
    int baseRegis = (instruction & BASESRCREG) >> 6;
    if(baseRegis == 7)
    {
        printf("RET\n");
    }
    else
    {
        printf("JMP\tR%d\n", baseRegis);
    }
}

void printLea(int instruction, int pc)
{
    int destinRegis = (instruction & DESTREG) >> 9;
    int ofst = (instruction & PCOFFSET9) << 23;
    ofst = ofst >> 23;
    printf("LEA\tR%d, x%X\n", destinRegis, pc + ofst);
}

void printTrap(int instruction)
{
    int trpVec = instruction & 0x00FF;
    switch(trpVec)
    {
        case 0x0020:
        printf("GETC\n");
        break;
        case 0x0021:
        printf("OUT\n");
        break;
        case 0x0022:
        printf("PUTS\n");
        break;
        case 0x0023:
        printf("IN\n");
        break;
        case 0x0024:
        printf("PUTSP\n");
        break;
        case 0x0025:
        printf("HALT\n");
        break;
        default:
        break;
    }
}

