
/*********************************************************
    NEC V810 (upd70732) disassembler (c) Tomasz Slanina 
**********************************************************/

#include <stdio.h>
#include <stdlib.h>

#define INTEGER_5BIT(x) (((x)&0x1f)|(((x)&0x10)?0xffffffe0:0))
#define UNSIGNED_INTEGER_5BIT(x) ((x)&0x1f)
#define INTEGER_16BIT(x) (((x)&0xffff)|(((x)&0x8000)?0xffff0000:0))
#define UNSIGNED_INTEGER_16BIT(x) ((x)&0xffff)
#define DISPLACEMENT_16BIT(x) (((x)&0xffff)|(((x)&0x8000)?0xffff0000:0))
#define DISPLACEMENT_26BIT(x,y) ((y)|((x&0x3ff)<<16 )|((x&0x200)?0xfc000000:0))
#define DISPLACEMENT_9BIT(x) ((x&0x1ff)|((x&0x100)?0xfffffe00:0))

const char *const registerNames[] = {
	"R0", "R1", "R2", "SP", "R4", "R5", "R6", "R7",
	"R8", "R9", "R10", "R11", "R12", "R13", "R14","R15",
	"R16", "R17", "R18", "R19", "R20", "R21", "R22", "R23",
	"R24", "R25", "R26", "R27", "R28", "R29", "R30", "R31",
	"EIPC", "EIPSW", "FEPC", "FEPSW", "ECR", "PSW", "PIR", "TKCW",
	"?", "?", "?", "?", "?", "?", "?", "?",
	"?", "?", "?", "?", "?", "?", "?", "?",
	"CHCW", "ADTRE", "?", "?", "?", "?", "?", "?"
};

#define GET1s(opcode) registerNames[(opcode)&0x1f]
#define GET2s(opcode) registerNames[((opcode)>>5)&0x1f]
#define GETRs(opcode) registerNames[0x20+((opcode)&0x1f)]


uint32_t step( uint32_t pc, uint32_t opc, uint32_t opc2 ){
	uint32_t size = 1;

	switch(opc>>10) {
		case 0x00: {
			printf("MOV %s,%s",GET1s(opc),GET2s(opc));
		} break;

		case 0x01: printf("ADD %s,%s",GET1s(opc),GET2s(opc)); break;
		case 0x02: printf("SUB %s,%s",GET1s(opc),GET2s(opc)); break;
		case 0x03: printf("CMP %s,%s",GET1s(opc),GET2s(opc)); break;
		case 0x04: printf("SHL %s,%s",GET1s(opc),GET2s(opc)); break;
		case 0x05: printf("SHR %s,%s",GET1s(opc),GET2s(opc)); break;
		case 0x06: printf("JMP [%s]",GET1s(opc)); break;
		case 0x07: printf("SAR %s,%s",GET1s(opc),GET2s(opc)); break;
		case 0x08: printf("MUL %s,%s",GET1s(opc),GET2s(opc)); break;
		case 0x09: printf("DIV %s,%s",GET1s(opc),GET2s(opc)); break;
		case 0x0a: printf("MULU %s,%s",GET1s(opc),GET2s(opc)); break;
		case 0x0b: printf("DIVU %s,%s",GET1s(opc),GET2s(opc)); break;
		case 0x0c: printf("OR %s,%s",GET1s(opc),GET2s(opc));    break;
		case 0x0d: printf("AND %s,%s",GET1s(opc),GET2s(opc)); break;
		case 0x0e: printf("XOR %s,%s",GET1s(opc),GET2s(opc)); break;
		case 0x0f: printf("NOT %s,%s",GET1s(opc),GET2s(opc)); break;
		case 0x10: printf("MOV $%X,%s",INTEGER_5BIT(opc),GET2s(opc)); break;
		case 0x11: printf("ADD $%X,%s",INTEGER_5BIT(opc),GET2s(opc)); break;
		case 0x12: printf("SETF $%X,%s",INTEGER_5BIT(opc),GET2s(opc)); break;
		case 0x13: printf("CMP $%X,%s",INTEGER_5BIT(opc),GET2s(opc)); break;
		case 0x14: printf("SHL $%X,%s",UNSIGNED_INTEGER_5BIT(opc),GET2s(opc)); break;
		case 0x15: printf("SHR $%X,%s",UNSIGNED_INTEGER_5BIT(opc),GET2s(opc)); break;
		case 0x16: printf("EI"); break;
		case 0x17: printf("SAR $%X,%s",UNSIGNED_INTEGER_5BIT(opc),GET2s(opc)); break;
		case 0x18: printf("TRAP $%X",INTEGER_5BIT(opc)); break;
		case 0x19: printf("RETI"); break;
		case 0x1a: printf("HALT"); break;
		case 0x1b: printf("? $1B"); break;
		case 0x1c: printf("LDSR %s,%s",GET2s(opc),GETRs(opc));break;
		case 0x1d: printf("STSR %s,%s",GETRs(opc),GET2s(opc));break;
		case 0x1e: printf("DI"); break;

		case 0x1f:
			switch(opc&0x1f) {
				case 0x00:  printf("SCH0BSU"); break;
				case 0x01:  printf("SCH0BSD"); break;
				case 0x02:  printf("SCH1BSU"); break;
				case 0x03:  printf("SCH1BSD"); break;
				case 0x04:  printf("? S $4"); break;
				case 0x05:  printf("? S $5"); break;
				case 0x06:  printf("? S $6"); break;
				case 0x08:  printf("ORBSU"); break;
				case 0x09:  printf("ANDBSU"); break;
				case 0x0a:  printf("XORBSU"); break;
				case 0x0b:  printf("MOVBSU"); break;
				case 0x0c:  printf("ORNBSU"); break;
				case 0x0d:  printf("ANDNBSU"); break;
				case 0x0e:  printf("XORNBSU"); break;
				case 0x0f:  printf("NOTBSU"); break;
				default:        printf("? S $%X",opc&0x1f); break;
			} break;
		case 0x20:
		case 0x21:
		case 0x22:
		case 0x23:
		case 0x24:
		case 0x25:
		case 0x26:
		case 0x27: 
			switch( (opc>>9) &0xf) {
				case 0x0: printf("BV $%X",pc+DISPLACEMENT_9BIT(opc));  break;
				case 0x1: printf("BL $%X",pc+DISPLACEMENT_9BIT(opc));  break;
				case 0x2: printf("BE $%X",pc+DISPLACEMENT_9BIT(opc));  break;
				case 0x3: printf("BNH $%X",pc+DISPLACEMENT_9BIT(opc)); break;
				case 0x4: printf("BN $%X",pc+DISPLACEMENT_9BIT(opc));  break;
				case 0x5: printf("BR $%X",pc+DISPLACEMENT_9BIT(opc));  break;
				case 0x6: printf("BLT $%X",pc+DISPLACEMENT_9BIT(opc)); break;
				case 0x7: printf("BLE $%X",pc+DISPLACEMENT_9BIT(opc)); break;
				case 0x8: printf("BNV $%X",pc+DISPLACEMENT_9BIT(opc)); break;
				case 0x9: printf("BNL $%X",pc+DISPLACEMENT_9BIT(opc)); break;
				case 0xa: printf("BNE $%X",pc+DISPLACEMENT_9BIT(opc)); break;
				case 0xb: printf("BH $%X",pc+DISPLACEMENT_9BIT(opc));  break;
				case 0xc: printf("BP $%X",pc+DISPLACEMENT_9BIT(opc));  break;
				case 0xd: printf("NOP"); break;
				case 0xe: printf("BGE $%X",pc+DISPLACEMENT_9BIT(opc)); break;
				case 0xf: printf("BGT $%X",pc+DISPLACEMENT_9BIT(opc)); break;
			} break;

		case 0x28:  printf("MOVEA $%X, %s, %s",INTEGER_16BIT(opc2),GET1s(opc),GET2s(opc));size=2; break;
		case 0x29:  printf("ADDI $%X, %s, %s",INTEGER_16BIT(opc2),GET1s(opc),GET2s(opc));size=2; break;
		case 0x2a:  printf("JR $%X",pc+DISPLACEMENT_26BIT(opc,opc2));size=2; break;
		case 0x2b:  printf("JAL $%X",pc+DISPLACEMENT_26BIT(opc,opc2));size=2;  break;
		case 0x2c:  printf("ORI $%X, %s, %s",UNSIGNED_INTEGER_16BIT(opc2),GET1s(opc),GET2s(opc));size=2; break;
		case 0x2d:  printf("ANDI $%X, %s, %s",UNSIGNED_INTEGER_16BIT(opc2),GET1s(opc),GET2s(opc));size=2; break;
		case 0x2e:  printf("XORI $%X, %s, %s",UNSIGNED_INTEGER_16BIT(opc2),GET1s(opc),GET2s(opc));size=2; break;
		case 0x2f:  printf("MOVHI $%X, %s, %s",UNSIGNED_INTEGER_16BIT(opc2),GET1s(opc),GET2s(opc));size=2; break;
		case 0x30:  printf("LDB $%X[%s], %s",DISPLACEMENT_16BIT(opc2),GET1s(opc),GET2s(opc));size=2; break;
		case 0x31:  printf("LDH $%X[%s], %s",DISPLACEMENT_16BIT(opc2),GET1s(opc),GET2s(opc));size=2; break;
		case 0x32:  printf("? $32"); break;
		case 0x33:  printf("LDW $%X[%s], %s",DISPLACEMENT_16BIT(opc2),GET1s(opc),GET2s(opc));size=2; break;
		case 0x34:  printf("STB %s, $%X[%s]",GET2s(opc),DISPLACEMENT_16BIT(opc2),GET1s(opc));size=2; break;
		case 0x35:  printf("STH %s, $%X[%s]",GET2s(opc),DISPLACEMENT_16BIT(opc2),GET1s(opc));size=2; break;
		case 0x36:  printf("? $36"); break;
		case 0x37:  printf("STW %s, $%X[%s]",GET2s(opc),DISPLACEMENT_16BIT(opc2),GET1s(opc));size=2; break;
		case 0x38:  printf("INB $%X[%s], %s",DISPLACEMENT_16BIT(opc2),GET1s(opc),GET2s(opc));size=2; break;
		case 0x39:  printf("INH $%X[%s], %s",DISPLACEMENT_16BIT(opc2),GET1s(opc),GET2s(opc));size=2; break;
		case 0x3a:  printf("CAXI $%X[%s], %s",DISPLACEMENT_16BIT(opc2),GET1s(opc),GET2s(opc));size=2; break;
		case 0x3b:  printf("INW $%X[%s], %s",DISPLACEMENT_16BIT(opc2),GET1s(opc),GET2s(opc));size=2; break;

		case 0x3c:  printf("OUTB %s, $%X[%s]",GET2s(opc),DISPLACEMENT_16BIT(opc2),GET1s(opc));size=2; break;
		case 0x3d:  printf("OUTH %s, $%X[%s]",GET2s(opc),DISPLACEMENT_16BIT(opc2),GET1s(opc));size=2; break;
		case 0x3e:
			switch((opc2&0xfc00)>>10) {
				case 0x0: printf("CMPF.S %s, %s",GET1s(opc),GET2s(opc)); break;
				case 0x2: printf("CVT.WS %s, %s",GET1s(opc),GET2s(opc)); break;
				case 0x3: printf("CVT.SW %s, %s",GET1s(opc),GET2s(opc)); break;
				case 0x4: printf("ADDF.S %s, %s",GET1s(opc),GET2s(opc)); break;
				case 0x5: printf("SUBF.S %s, %s",GET1s(opc),GET2s(opc)); break;
				case 0x6: printf("MULF.S %s, %s",GET1s(opc),GET2s(opc)); break;
				case 0x7: printf("DIVF.S %s, %s",GET1s(opc),GET2s(opc)); break;
				case 0xb: printf("TRNC.SW %s, %s",GET1s(opc),GET2s(opc)); break;
				default : printf("? fp  $%X",(opc2&0xfc00)>>10); break;
			}
			size=2;
			break;

		case 0x3f:  printf("OUTW %s, $%X[%s]",GET2s(opc),DISPLACEMENT_16BIT(opc2),GET1s(opc));size=2; break;
	}
	return size;
} 


int main(int argc, char *argv[]) {

	if(argc != 3) {
		printf("\nv810d Nec v810 disassembler (c)Tomasz Slanina\nUsage:\n%s filename address\n (for example %s firmware.bin ffff0000)\n",argv[0],argv[0]);
		return 0;
	}

	FILE * file = fopen(argv[1],"rb");

	uint32_t pc = strtol(argv[2],NULL,16);
	if(file){

		uint16_t opcode1, opcode2;
	
		fread(&opcode1, 1, sizeof(uint16_t), file);
		fread(&opcode2, 1, sizeof(uint16_t), file);

		bool loop = true;
	
		while(loop){
			printf("$%.8X:\t",pc);
			if (step( pc, opcode1, opcode2) == 1 ){
				opcode1 = opcode2;
				loop = fread(&opcode2, 1, sizeof(uint16_t), file) > 0;
				pc+=2;
			} else {
				loop = fread(&opcode1, 1, sizeof(uint16_t), file) > 0;
				loop = loop && (fread(&opcode2, 1, sizeof(uint16_t), file) > 0);
				pc+=4;
			}
			printf("\n");
		}

		fclose(file);
	} else {
		printf("\nFile %s error\n",argv[1]);
		return -1;
	}
	return 0;
}

