#pragma once
#include <string>
#include <queue>
#include <map>
#include <fstream>
#include <time.h>
#include <stdio.h>
#include "BitsAndBytes.h"
class Platform;


enum AddressingMode
{
	am_imp,
	am_imm,
	am_zp,
	am_zpx,
	am_zpy,
	am_izx,
	am_izy,
	am_abs,
	am_abx,
	am_aby,
	am_ind,
	am_rel,
	am_inh,
	NR_AM,
};

enum OpCodes
{
	op_ORA,
	op_AND,
	op_EOR,
	op_ADC,
	op_SBC,
	op_CMP,
	op_CPX,
	op_CPY,
	op_DEC,
	op_DEX,
	op_DEY,
	op_INC,
	op_INX,
	op_INY,
	op_ASL,
	op_ROL,
	op_LSR,
	op_ROR,
};


enum ReadType
{
	OP,
	HIGH,
	LOW,
	DATA,
};

enum CPU_TASKS
{
	WASTE,
	READ_OP,
	READ_H,
	READ_D,
	READ_LD,
	READ_HD,
	READ_A,
	WRITE,
	WRITE_LD,
	WRITE_HD,
	WRITE_X,
	WRITE_Y,
	WRITE_A,
	INC_ADH,
	MERGE_ADDRESS,
	EXECUTE_OP,
};


union _NVBDI_ZC_
{
	struct {
		unsigned C : 1;
		unsigned Z : 1;
		unsigned I : 1;
		unsigned D : 1;
		unsigned B : 1;
		unsigned _ : 1;
		unsigned V : 1;
		unsigned N : 1;
	};
	unsigned char byte;
};


class CPU
{
public:
	CPU(Platform* platform);

	void Tick();
	void ExecuteOp();
	void EvaluateOpAddressMode();
	void Reset();
private:


	void LDX_IMM();
	void TXS_IMP();
	void LDY_IMM();
	void SEI_IMP();
	void JSR_ABS();
	void LDA_ABX();
	void CMP_ABX();
	void BNE_REL();
	void CLD_IMP();
	void RTS_IMP();
	void STX_ABS();
	void SEC_IMP();
	void CLC_IMP();
	void LDA_IMM();
	void STA_ABX();
	void DEX_IMP();
	void STA_ZP();
	void RLA_ABS();
	void STA_ABS();
	void LDA_ABS();
	void BEQ_REL();
	void JMP_ABS();
	void AND_ABY();
	void ORA_ABY();
	void TAY_IMP();
	void STA_ABY();
	void INY_IMP();
	void STX_ZP();
	void STY_ZP();
	void INC_ZP();
	void LDA_IZY();
	void TAX_IMM();
	void EOR_ZPX();
	void CMP_IZX();
	void STA_IZY();
	void CMP_IZY();
	void ROL_IMM();
	void TXA_IMM();
	void TYA_IMM();
	void LDY_ZP();
	void STY_ABS();
	void LDA_ABY();
	void BCS_REL();
	void DEY_IMM();
	void BPL_REL();
	void STY_ZPX();
	void checkV(int val);
	void ADC_IMM();
	void BCC_REL();
	void INX_REL();
	void CPX_IMM();
	void STA_ZPX();
	void LDA_ZPX();
	void CLI_IMP();
	void ORA_ABS();
	void LDA_ZP();
	void LDX_ZP();
	void LDY_ZPX();
	void BMI_REL();
	void AND_IMM();
	void ORA_IMM();
	void JMP_IND();
	void LDX_ABS();
	void LDY_ABS();
	void CPY_ZP();
	void CMP_ZP();
	void ADC_ZP();
	void SBC_IMM();
	void CMP_IMM();
	void PHP_IMP();

	void PHA_IMP();

	void PLA_IMP();

	void PLP_IMP();

	unsigned int totalOps;
	void setupAddressMap()
	{

		opCodeAddressingMode_[0x00] = am_imp;
		opCodeAddressingMode_[0x01] = am_izx;
		opCodeAddressingMode_[0x02] = am_imp;
		opCodeAddressingMode_[0x03] = am_izx;
		opCodeAddressingMode_[0x04] = am_zp;
		opCodeAddressingMode_[0x05] = am_zp;
		opCodeAddressingMode_[0x06] = am_zp;
		opCodeAddressingMode_[0x07] = am_zp;
		opCodeAddressingMode_[0x08] = am_imp;
		opCodeAddressingMode_[0x09] = am_imm;
		opCodeAddressingMode_[0x0a] = am_imp;
		opCodeAddressingMode_[0x0b] = am_imm;
		opCodeAddressingMode_[0x0c] = am_abs;
		opCodeAddressingMode_[0x0d] = am_abs;
		opCodeAddressingMode_[0x0e] = am_abs;
		opCodeAddressingMode_[0x0f] = am_abs;

		opCodeAddressingMode_[0x10] = am_rel;
		opCodeAddressingMode_[0x11] = am_izy;
		opCodeAddressingMode_[0x12] = am_imp;
		opCodeAddressingMode_[0x13] = am_izy;
		opCodeAddressingMode_[0x14] = am_zpx;
		opCodeAddressingMode_[0x15] = am_zpx;
		opCodeAddressingMode_[0x16] = am_zpx;
		opCodeAddressingMode_[0x17] = am_zpx;
		opCodeAddressingMode_[0x18] = am_imp;
		opCodeAddressingMode_[0x19] = am_aby;
		opCodeAddressingMode_[0x1a] = am_imp;
		opCodeAddressingMode_[0x1b] = am_aby;
		opCodeAddressingMode_[0x1c] = am_abx;
		opCodeAddressingMode_[0x1d] = am_abx;
		opCodeAddressingMode_[0x1e] = am_abx;
		opCodeAddressingMode_[0x1f] = am_abx;

		opCodeAddressingMode_[0x20] = am_abs;
		opCodeAddressingMode_[0x21] = am_izx;
		opCodeAddressingMode_[0x22] = am_imp;
		opCodeAddressingMode_[0x23] = am_izx;
		opCodeAddressingMode_[0x24] = am_zp;
		opCodeAddressingMode_[0x25] = am_zp;
		opCodeAddressingMode_[0x26] = am_zp;
		opCodeAddressingMode_[0x27] = am_zp;
		opCodeAddressingMode_[0x28] = am_imp;
		opCodeAddressingMode_[0x29] = am_imm;
		opCodeAddressingMode_[0x2a] = am_imp;
		opCodeAddressingMode_[0x2b] = am_aby;
		opCodeAddressingMode_[0x2c] = am_abx;
		opCodeAddressingMode_[0x2d] = am_abx;
		opCodeAddressingMode_[0x2e] = am_abx;
		opCodeAddressingMode_[0x2f] = am_abx;

		opCodeAddressingMode_[0x30] = am_rel;
		opCodeAddressingMode_[0x31] = am_izy;
		opCodeAddressingMode_[0x32] = am_imp;
		opCodeAddressingMode_[0x33] = am_izy;
		opCodeAddressingMode_[0x34] = am_zpx;
		opCodeAddressingMode_[0x35] = am_zpx;
		opCodeAddressingMode_[0x36] = am_zpx;
		opCodeAddressingMode_[0x37] = am_zpx;
		opCodeAddressingMode_[0x38] = am_imp;
		opCodeAddressingMode_[0x39] = am_aby;
		opCodeAddressingMode_[0x3a] = am_imp;
		opCodeAddressingMode_[0x3b] = am_aby;
		opCodeAddressingMode_[0x3c] = am_abx;
		opCodeAddressingMode_[0x3d] = am_abx;
		opCodeAddressingMode_[0x3e] = am_abx;
		opCodeAddressingMode_[0x3f] = am_abx;

		opCodeAddressingMode_[0x40] = am_imp;
		opCodeAddressingMode_[0x41] = am_izx;
		opCodeAddressingMode_[0x42] = am_imp;
		opCodeAddressingMode_[0x43] = am_izx;
		opCodeAddressingMode_[0x44] = am_imp;
		opCodeAddressingMode_[0x45] = am_zp;
		opCodeAddressingMode_[0x46] = am_zp;
		opCodeAddressingMode_[0x47] = am_zp;
		opCodeAddressingMode_[0x48] = am_imp;
		opCodeAddressingMode_[0x49] = am_imm;
		opCodeAddressingMode_[0x4a] = am_imp;
		opCodeAddressingMode_[0x4b] = am_imm;
		opCodeAddressingMode_[0x4c] = am_abs;
		opCodeAddressingMode_[0x4d] = am_abs;
		opCodeAddressingMode_[0x4e] = am_abs;
		opCodeAddressingMode_[0x4f] = am_abs;

		opCodeAddressingMode_[0x50] = am_rel;
		opCodeAddressingMode_[0x51] = am_izy;
		opCodeAddressingMode_[0x52] = am_imp;
		opCodeAddressingMode_[0x53] = am_izy;
		opCodeAddressingMode_[0x54] = am_zpx;
		opCodeAddressingMode_[0x55] = am_zpx;
		opCodeAddressingMode_[0x56] = am_zpx;
		opCodeAddressingMode_[0x57] = am_zpx;
		opCodeAddressingMode_[0x58] = am_imp;
		opCodeAddressingMode_[0x59] = am_aby;
		opCodeAddressingMode_[0x5a] = am_imp;
		opCodeAddressingMode_[0x5b] = am_aby;
		opCodeAddressingMode_[0x5c] = am_abx;
		opCodeAddressingMode_[0x5d] = am_abx;
		opCodeAddressingMode_[0x5e] = am_abx;
		opCodeAddressingMode_[0x5f] = am_abx;

		opCodeAddressingMode_[0x60] = am_imp;
		opCodeAddressingMode_[0x61] = am_izx;
		opCodeAddressingMode_[0x62] = am_imp;
		opCodeAddressingMode_[0x63] = am_izx;
		opCodeAddressingMode_[0x64] = am_zp;
		opCodeAddressingMode_[0x65] = am_zp;
		opCodeAddressingMode_[0x66] = am_zp;
		opCodeAddressingMode_[0x67] = am_zp;
		opCodeAddressingMode_[0x68] = am_imp;
		opCodeAddressingMode_[0x69] = am_imm;
		opCodeAddressingMode_[0x6a] = am_imp;
		opCodeAddressingMode_[0x6b] = am_imm;
		opCodeAddressingMode_[0x6c] = am_ind;
		opCodeAddressingMode_[0x6d] = am_abs;
		opCodeAddressingMode_[0x6e] = am_abs;
		opCodeAddressingMode_[0x6f] = am_abs;

		opCodeAddressingMode_[0x70] = am_rel;
		opCodeAddressingMode_[0x71] = am_izy;
		opCodeAddressingMode_[0x72] = am_imp;
		opCodeAddressingMode_[0x73] = am_izy;
		opCodeAddressingMode_[0x74] = am_zpx;
		opCodeAddressingMode_[0x75] = am_zpx;
		opCodeAddressingMode_[0x76] = am_zpx;
		opCodeAddressingMode_[0x77] = am_zpx;
		opCodeAddressingMode_[0x78] = am_imp;
		opCodeAddressingMode_[0x79] = am_aby;
		opCodeAddressingMode_[0x7a] = am_imp;
		opCodeAddressingMode_[0x7b] = am_aby;
		opCodeAddressingMode_[0x7c] = am_abx;
		opCodeAddressingMode_[0x7d] = am_abx;
		opCodeAddressingMode_[0x7e] = am_abx;
		opCodeAddressingMode_[0x7f] = am_abx;

		opCodeAddressingMode_[0x80] = am_imm;
		opCodeAddressingMode_[0x81] = am_izx;
		opCodeAddressingMode_[0x82] = am_imm;
		opCodeAddressingMode_[0x83] = am_izx;
		opCodeAddressingMode_[0x84] = am_zp;
		opCodeAddressingMode_[0x85] = am_zp;
		opCodeAddressingMode_[0x86] = am_zp;
		opCodeAddressingMode_[0x87] = am_zp;
		opCodeAddressingMode_[0x88] = am_imp;
		opCodeAddressingMode_[0x89] = am_imm;
		opCodeAddressingMode_[0x8a] = am_imp;
		opCodeAddressingMode_[0x8b] = am_imm;
		opCodeAddressingMode_[0x8c] = am_abs;
		opCodeAddressingMode_[0x8d] = am_abs;
		opCodeAddressingMode_[0x8e] = am_abs;
		opCodeAddressingMode_[0x8f] = am_abs;

		opCodeAddressingMode_[0x90] = am_rel;
		opCodeAddressingMode_[0x91] = am_izy;
		opCodeAddressingMode_[0x92] = am_imp;
		opCodeAddressingMode_[0x93] = am_izy;
		opCodeAddressingMode_[0x94] = am_zpx;
		opCodeAddressingMode_[0x95] = am_zpx;
		opCodeAddressingMode_[0x96] = am_zpy;
		opCodeAddressingMode_[0x97] = am_zpy;
		opCodeAddressingMode_[0x98] = am_imp;
		opCodeAddressingMode_[0x99] = am_aby;
		opCodeAddressingMode_[0x9a] = am_imp;
		opCodeAddressingMode_[0x9b] = am_aby;
		opCodeAddressingMode_[0x9c] = am_abx;
		opCodeAddressingMode_[0x9d] = am_abx;
		opCodeAddressingMode_[0x9e] = am_aby;
		opCodeAddressingMode_[0x9f] = am_aby;

		opCodeAddressingMode_[0xa0] = am_imm;
		opCodeAddressingMode_[0xa1] = am_izx;
		opCodeAddressingMode_[0xa2] = am_imm;
		opCodeAddressingMode_[0xa3] = am_izx;
		opCodeAddressingMode_[0xa4] = am_zp;
		opCodeAddressingMode_[0xa5] = am_zp;
		opCodeAddressingMode_[0xa6] = am_zp;
		opCodeAddressingMode_[0xa7] = am_zp;
		opCodeAddressingMode_[0xa8] = am_imp;
		opCodeAddressingMode_[0xa9] = am_imm;
		opCodeAddressingMode_[0xaa] = am_imp;
		opCodeAddressingMode_[0xab] = am_imm;
		opCodeAddressingMode_[0xac] = am_abs;
		opCodeAddressingMode_[0xad] = am_abs;
		opCodeAddressingMode_[0xae] = am_abs;
		opCodeAddressingMode_[0xaf] = am_abs;

		opCodeAddressingMode_[0xb0] = am_rel;
		opCodeAddressingMode_[0xb1] = am_izy;
		opCodeAddressingMode_[0xb2] = am_imp;
		opCodeAddressingMode_[0xb3] = am_izy;
		opCodeAddressingMode_[0xb4] = am_zpx;
		opCodeAddressingMode_[0xb5] = am_zpx;
		opCodeAddressingMode_[0xb6] = am_zpy;
		opCodeAddressingMode_[0xb7] = am_zpy;
		opCodeAddressingMode_[0xb8] = am_imp;
		opCodeAddressingMode_[0xb9] = am_aby;
		opCodeAddressingMode_[0xba] = am_imp;
		opCodeAddressingMode_[0xbb] = am_aby;
		opCodeAddressingMode_[0xbc] = am_abx;
		opCodeAddressingMode_[0xbd] = am_abx;
		opCodeAddressingMode_[0xbe] = am_abx;
		opCodeAddressingMode_[0xbf] = am_abx;

		opCodeAddressingMode_[0xc0] = am_imm;
		opCodeAddressingMode_[0xc1] = am_izx;
		opCodeAddressingMode_[0xc2] = am_imm;
		opCodeAddressingMode_[0xc3] = am_izx;
		opCodeAddressingMode_[0xc4] = am_zp;
		opCodeAddressingMode_[0xc5] = am_zp;
		opCodeAddressingMode_[0xc6] = am_zp;
		opCodeAddressingMode_[0xc7] = am_zp;
		opCodeAddressingMode_[0xc8] = am_imp;
		opCodeAddressingMode_[0xc9] = am_imm;
		opCodeAddressingMode_[0xca] = am_imp;
		opCodeAddressingMode_[0xcb] = am_imm;
		opCodeAddressingMode_[0xcc] = am_abs;
		opCodeAddressingMode_[0xcd] = am_abs;
		opCodeAddressingMode_[0xce] = am_abs;
		opCodeAddressingMode_[0xcf] = am_abs;

		opCodeAddressingMode_[0xd0] = am_rel;
		opCodeAddressingMode_[0xd1] = am_izy;
		opCodeAddressingMode_[0xd2] = am_imp;
		opCodeAddressingMode_[0xd3] = am_izy;
		opCodeAddressingMode_[0xd4] = am_zpx;
		opCodeAddressingMode_[0xd5] = am_zpx;
		opCodeAddressingMode_[0xd6] = am_zpx;
		opCodeAddressingMode_[0xd7] = am_zpx;
		opCodeAddressingMode_[0xd8] = am_imp;
		opCodeAddressingMode_[0xd9] = am_aby;
		opCodeAddressingMode_[0xda] = am_imp;
		opCodeAddressingMode_[0xdb] = am_aby;
		opCodeAddressingMode_[0xdc] = am_abx;
		opCodeAddressingMode_[0xdd] = am_abx;
		opCodeAddressingMode_[0xde] = am_abx;
		opCodeAddressingMode_[0xdf] = am_abx;

		opCodeAddressingMode_[0xe0] = am_imm;
		opCodeAddressingMode_[0xe1] = am_izx;
		opCodeAddressingMode_[0xe2] = am_imm;
		opCodeAddressingMode_[0xe3] = am_izx;
		opCodeAddressingMode_[0xe4] = am_zp;
		opCodeAddressingMode_[0xe5] = am_zp;
		opCodeAddressingMode_[0xe6] = am_zp;
		opCodeAddressingMode_[0xe7] = am_zp;
		opCodeAddressingMode_[0xe8] = am_imp;
		opCodeAddressingMode_[0xe9] = am_imm;
		opCodeAddressingMode_[0xea] = am_imp;
		opCodeAddressingMode_[0xeb] = am_imm;
		opCodeAddressingMode_[0xec] = am_abs;
		opCodeAddressingMode_[0xed] = am_abs;
		opCodeAddressingMode_[0xee] = am_abs;
		opCodeAddressingMode_[0xef] = am_abs;

		opCodeAddressingMode_[0xf0] = am_rel;
		opCodeAddressingMode_[0xf1] = am_izy;
		opCodeAddressingMode_[0xf2] = am_imp;
		opCodeAddressingMode_[0xf3] = am_izy;
		opCodeAddressingMode_[0xf4] = am_zpx;
		opCodeAddressingMode_[0xf5] = am_zpx;
		opCodeAddressingMode_[0xf6] = am_zpx;
		opCodeAddressingMode_[0xf7] = am_zpx;
		opCodeAddressingMode_[0xf8] = am_imp;
		opCodeAddressingMode_[0xf9] = am_aby;
		opCodeAddressingMode_[0xfa] = am_imp;
		opCodeAddressingMode_[0xfb] = am_aby;
		opCodeAddressingMode_[0xfc] = am_abx;
		opCodeAddressingMode_[0xfd] = am_abx;
		opCodeAddressingMode_[0xfe] = am_abx;
		opCodeAddressingMode_[0xff] = am_abx;

	}

	unsigned char SP;

	int tickCounter;

	void mergeAddress();
	AddressingMode currentAddressingMode_;
	int opCodeAddressingMode_[256];
	unsigned short address_;

	std::queue<CPU_TASKS> taskQueue_;
	Platform* platform_;

	unsigned char Read(unsigned short address, ReadType type);

	unsigned char ReadA();

	void WriteA(unsigned char data);
	
	void Write(unsigned short address, unsigned char data);

	


	BitsAndByte Accumulator() { return Accumulator_; }
	BitsAndByte X() { return X_; }
	BitsAndByte Y() { return Y_; }

	void IZX();
	void ISC();
	
	void checkNZ(unsigned char data);

	void SLO();

	void ORA();
	void AND();
	void EOR();
	void ADC();
	void SBC();
	void CMP();
	
	void CPX();
	void CPY();
	void DEC();
	void DEX();
	void DEY();
	void INC();
	void INX();
	void INY();
	void ASL();
	void ROL();
	void LSR();
	void ROR();

	void NOP();

	void LDA();
	void STA();

	void CLC();
	void SEC();
	void CLD();
	void SED();
	void CLI();
	void SEI();
	void CLV();

	void BPL();
	void BMI();
	void BVC();
	void BVS();
	void BCC();
	void BCS();
	void BNE();
	void BEQ();
	void BRK();
	void RTI();
	void TAY();

	unsigned char pop()
	{
		SP++;
		SP &= 0xff;
		return Read(0x100 + SP,DATA);
	}

	void push(unsigned char value)
	{
		Write(0x100 + SP, value & 0xff);
		SP--;
		SP &= 0xff;
	}



	void LDY();
	void LDX();

	void JMP();

	void BIT();
	
	void JSR();

	void TXS();
	void PLA();
	void PHA();
	void PLP();
	void PHP();
	void STX();
	void STY();
	void TAX();
	void TXA();
	void RTS();

	

	void N(bool val)
	{
		NVBDI_ZC_.N = val;
	}
	void V(bool val)
	{
		NVBDI_ZC_.V = val;

	}
	void B(bool val)
	{
		NVBDI_ZC_.B = val;

	}
	void D(bool val)
	{
		NVBDI_ZC_.D = val;
	}
	void _(bool val)
	{
		NVBDI_ZC_.D = val;
	}
	void I(bool val)
	{
		NVBDI_ZC_.I = val;

	}
	void Z(bool val)
	{
		NVBDI_ZC_.Z = val;

	}
	void C(bool val)
	{
		NVBDI_ZC_.C = val;
	}




	bool N()
	{
		return NVBDI_ZC_.N != 0;
	}
	bool V()
	{
		return NVBDI_ZC_.V != 0;

	}
	bool B()
	{
		return NVBDI_ZC_.B != 0;

	}
	bool D()
	{
		return NVBDI_ZC_.D != 0;
	}
	bool I()
	{
		return NVBDI_ZC_.I != 0;

	}
	bool Z()
	{
		return NVBDI_ZC_.Z != 0;

	}
	bool C()
	{
		return NVBDI_ZC_.C != 0;
	}


	unsigned char ADL_;
	unsigned char ADH_;
	unsigned char LDAT_;
	unsigned char HDAT_;
	unsigned char OP_;

	BitsAndByte X_;
	BitsAndByte Y_;
	std::string Stack;
	unsigned short PC;
	unsigned short oldPC;
	int Time;
	BitsAndByte Accumulator_;
	BitsAndByte P;
	_NVBDI_ZC_	NVBDI_ZC_;

	void TYA();

	void TSX();


	void TestAddress();
};
	
