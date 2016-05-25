#include "CPU.h"
#include <fstream>
#include <string>
#include <sstream>
#include <iostream>
#include "platform.h"
#include <queue>
#include "logger.h"
#include "BitsAndBytes.h"
#include "intToStr.h"

CPU::CPU(Platform* platform)
{
	platform_ = platform;
	//PC = 64738;
	//NVBDI_ZC_.byte = 0x24;
	setupAddressMap();
	tickCounter = 0;
}


void CPU::mergeAddress()
{
	unsigned short absAddr = 0;
	switch (currentAddressingMode_)
	{
	case am_imp:
		PC--;
		taskQueue_.push(READ_A);
		taskQueue_.push(EXECUTE_OP);
		break;
	case am_imm:
		LDAT_ = ADL_;
		taskQueue_.push(EXECUTE_OP);
		break;
	case am_zp:
		address_ = ADL_;
		taskQueue_.push(CPU_TASKS::READ_D);
		taskQueue_.push(EXECUTE_OP);
		break;
	case am_zpx:
		address_ = ADL_ + X_.byte;
		taskQueue_.push(CPU_TASKS::READ_D);
		taskQueue_.push(EXECUTE_OP);
		break;
	case am_zpy:
		address_ = ADL_ + Y_.byte;
		taskQueue_.push(CPU_TASKS::READ_D);
		taskQueue_.push(EXECUTE_OP);
		break;
	case am_inh:
		ADH_++;
		address_ = ADL_ + (ADH_<<8);
		taskQueue_.push(CPU_TASKS::READ_D);
		taskQueue_.push(EXECUTE_OP);
		break;
	case am_izx:
		address_ = ADL_ + X_.byte;
		if (address_ < ADL_)
		{
			ADL_ = address_ &0x00ff;
			break;
		}
		else
		{
			address_ += (ADH_ << 8);
			taskQueue_.push(CPU_TASKS::READ_D);
			taskQueue_.push(EXECUTE_OP);
		}
		break;
	case am_izy:
		address_ = (ADL_)+(ADH_ << 8);
		if ((address_ >> 8) != ((address_ + Y_.byte) >> 8))
		{
			taskQueue_.push(CPU_TASKS::WASTE);
		}
		address_ += Y_.byte;

		taskQueue_.push(CPU_TASKS::READ_D);
		taskQueue_.push(CPU_TASKS::EXECUTE_OP);


		break;
	case am_abs:
		address_ = ADL_ + (ADH_ << 8);
		taskQueue_.push(CPU_TASKS::READ_D);
		taskQueue_.push(EXECUTE_OP);

		break;
	case am_abx:
		if ((address_ >> 8) != ((address_ + X_.byte) >> 8))
		{
			taskQueue_.push(CPU_TASKS::WASTE);
		}
		address_ = (ADL_)+(ADH_ << 8);
		address_ += X_.byte;
		taskQueue_.push(READ_D);
		taskQueue_.push(EXECUTE_OP);
		break;
	case am_aby:
		absAddr = (ADL_)+(ADH_ << 8);
		if ((absAddr >> 8) != ((absAddr + Y_.byte) >> 8))
		{
			taskQueue_.push(CPU_TASKS::WASTE);
		}
		address_ = absAddr + Y_.byte;
		taskQueue_.push(EXECUTE_OP);
		break;
	case am_ind:
		absAddr = (ADL_)+(ADH_ << 8);
		taskQueue_.push(EXECUTE_OP);
		break;
	case am_rel:
		LDAT_ = ADL_;
		taskQueue_.push(EXECUTE_OP);
		break;
	}


}


void CPU::checkNZ(unsigned char data)
{ 
	BitsAndByte bbyte;
	bbyte.byte = data;
	
	N(bbyte.seven);
	Z(data == 0);

}

void CPU::Reset()
{
	Accumulator_.byte = 0;
	platform_->Write(0x01, 0xff);
	unsigned char low = platform_->Read(0xfffc);
	unsigned char high = platform_->Read(0xfffd);
	X_.byte = 0;
	Y_.byte = 0;
	_(true);
	//	//   resetVector=0xc000;
	 //System.out.printf("CPU: Reset vector=0x%04x\n", resetVector);
	NVBDI_ZC_.byte = 0x24;


	SP = 0;
	SP -= 3;
	SP &= 0xff;
	PC = (low + (high<<8));
	//doIRQ = false;
	//didRTI = false;

	//nbrNMISources = 0;
	//nmiSources.clear();
	//irqSources.clear();


}

unsigned char CPU::Read(unsigned short address, ReadType type)
{
	if (address == 0X02A6)
	{
		bool breakhere = true;
	}
	return platform_->Read(address);

}

unsigned char CPU::ReadA()
{
	LDAT_ = Accumulator_.byte;
	return Accumulator_.byte;
}

void CPU::WriteA(unsigned char data)
{
	Accumulator_.byte = data;
}


void CPU::Write(unsigned short address, unsigned char data)
{
	platform_->Write(address, data);
}

void CPU::ExecuteOp()
{
	switch (OP_)
	{

	

	case 0x07:
	case 0x17:
	case 0x03:
	case 0x13:
	case 0x0f:
	case 0x1f:
	case 0x1b:
		LogStrings::log_op_name = "SLO";
		SLO();
		break;

		//nop
	case 0x04:
	case 0x14:
	case 0xEA:
		LogStrings::log_op_name = "NOP";
		NOP();
		break;

	case  0x02:
		LogStrings::log_op_name = "SLO";
		platform_->dumpRam();
		exit(1);
		break;

		//Logical and aritmetic commands
		//ORA
	case 0x09:
	case 0x05:
	case 0x15:
	case 0x01:
	case 0x11:
	case 0x0D:
	case 0x1D:
	case 0x19:
		LogStrings::log_op_name = "ORA";
		ORA();
		break;

	case 0x16:
		LogStrings::log_op_name = "ASL";
		ASL();
		break;


		//AND

	case 0x29:
	case 0x25:
	case 0x35:
	case 0x21:
	case 0x31:
	case 0x2D:
	case 0x3D:
	case 0x39:
		LogStrings::log_op_name = "AND";
		AND();
		break;

		//EOR

	case 0x49:
	case 0x45:
	case 0x55:
	case 0x41:
	case 0x51:
	case 0x4D:
	case 0x5D:
	case 0x59:
		LogStrings::log_op_name = "EOR";
		EOR();
		break;

		//ADC
	case 0x69:
	case 0x65:
	case 0x75:
	case 0x61:
	case 0x71:
	case 0x6D:
	case 0x7D:
	case 0x79:
		LogStrings::log_op_name = "ADC";
		ADC();
		break;

		//SBC
	case 0xE9:
	case 0xE5:
	case 0xF5:
	case 0xE1:
	case 0xF1:
	case 0xED:
	case 0xFD:
	case 0xF9:
		LogStrings::log_op_name = "SBC";
		SBC();
		break;

		//CMP
	case 0xC9:
	case 0xC5:
	case 0xD5:
	case 0xC1:
	case 0xD1:
	case 0xCD:
	case 0xDD:
	case 0xD9:
		LogStrings::log_op_name = "CMP";
		CMP();
		break;


		//CPX
	case 0xE0:
	case 0xE4:
	case 0xEC:
		LogStrings::log_op_name = "CPX";
		CPX();
		break;

		//CPY
	case 0xC0:
	case 0xC4:
	case 0xCC:
		LogStrings::log_op_name = "CPY";
		CPY();
		break;

		//DEC
	case 0xC6:
	case 0xD6:
	case 0xCE:
	case 0xDE:
		LogStrings::log_op_name = "DEC";
		DEC();
		break;

		//TAX
	case 0xaa:
		TAX();
		break;

		//LDA			 					
	case 0xa9:
	case 0xA5:
	case 0xB5:

	case 0xB1:
	case 0xAD:
	case 0xBD:
	case 0xB9:
		LogStrings::log_op_name = "LDA";
		LDA();
		break;

		//LDX

	case 0xA2:
	case 0xA6:
	case 0xB6:
	case 0xAE:
	case 0xBE:
		LogStrings::log_op_name = "LDX";
		LDX();
		break;


		//LDY
	case 0xA1:
	case 0xA0:
	case 0xA4:
	case 0xB4:
	case 0xAC:
	case 0xBC:
		LogStrings::log_op_name = "LDY";
		LDY();
		break;

		//TYA
	case 0x98:
		TYA();
		break;

	case 0x85:
	case 0x95:
	case 0x81:
	case 0x91:
	case 0x8D:
	case 0x9D:
	case 0x99:
		LogStrings::log_op_name = "STA";
		STA();
		break;

		//JUMP/FLAGS

	case 0x80:
		LogStrings::log_op_name = "BCS";
		BCS();
		break;
	case 0x20:
		LogStrings::log_op_name = "JSR";
		JSR();
		break;

	case 0x18:
		LogStrings::log_op_name = "CLC";
		CLC();
		break;
	case 0x38:
		LogStrings::log_op_name = "SEC";
		SEC();
		break;
	case 0xD8:
		LogStrings::log_op_name = "CLD";
		CLD();
		break;
	case 0xF8:
		LogStrings::log_op_name = "SED";
		SED();
		break;
	case 0x58:
		LogStrings::log_op_name = "CLI";
		CLI();
		break;
	case 0x78:
		LogStrings::log_op_name = "SEI";
		SEI();
		break;
	case 0xB8:
		LogStrings::log_op_name = "CLV";
		CLV();
		break;

	case 0x10:
		LogStrings::log_op_name = "BPL";
		BPL();
		break;
	case 0x30:
		LogStrings::log_op_name = "BMI";
		BMI();
		break;
	case 0x50:
		LogStrings::log_op_name = "BVC";
		BVC();
		break;
	case 0x70:
		LogStrings::log_op_name = "BVS";
		BVS();
		break;
	case 0x90:
		LogStrings::log_op_name = "BCC";
		BCC();
		break;
	case 0xB0:
		LogStrings::log_op_name = "BCS";
		BCS();
		break;
	case 0xD0:
		LogStrings::log_op_name = "BNE";
		BNE();
		break;
	case 0xF0:
		LogStrings::log_op_name = "BEQ";
		BEQ();
		break;

	case 0x2A:
		LogStrings::log_op_name = "ROL";
		ROL();
		break;


		//INC
	case 0xE6:
	case 0xF6:
	case 0xEE:
	case 0xFE:
		LogStrings::log_op_name = "INC";
		INC();
		break;

	case 0xE8:
		LogStrings::log_op_name = "INX";
		INX();
		break;
	case 0xC8:
		LogStrings::log_op_name = "INY";
		INY();
		break;
	case 0xCA:
		LogStrings::log_op_name = "DEX";
		DEX();
		break;
	case 0x88:
		LogStrings::log_op_name = "DEY";
		DEY();
		break;

	case 0x60:
		LogStrings::log_op_name = "RTS";
		RTS();
		break;

	case 0x8A:
		LogStrings::log_op_name = "TXA";
		TXA();
		break;

		//MOVE
	case 0xa8:
		LogStrings::log_op_name = "TAY";
		TAY();
		break;

	case 0x9a:
		LogStrings::log_op_name = "TXS";
		TXS();
		break;

	case 0x8e:
	case 0x86:
		LogStrings::log_op_name = "STX";
		STX();
		break;

		//STY
	case 0x94:
	case 0x8c:
	case 0x84:
		LogStrings::log_op_name = "STY";
		STY();
		break;


		//JMP
	case 0x4C:
	case 0x6C:
		LogStrings::log_op_name = "JMP";
		JMP();
		break;

	case 0x00:
		{
			BRK();
			std::stringstream ss;
			ss << "BRK NOT IMPLEMENTED";
			Log(ss.str());
			platform_->dumpRam();
			exit(1);
			break;
		}
		break;
		//illegal
	case 0xff:
		Log("Illegal OP!!!!");
		ISC();
		break;

	default:
		std::stringstream ss;
		ss << "OP CODE:";
		ss << std::hex << OP_;
		ss << " NOT IMPLEMENTED";
		Log(ss.str());
		platform_->dumpRam();
		exit(1);
		break;
	}

}


void CPU::EvaluateOpAddressMode()
{
	//if()
	
	currentAddressingMode_ = (AddressingMode)opCodeAddressingMode_[OP_];
	//const char* adrStr = MACROSTR(*currentAddressingMode_);
	std::string logstr;

	switch (currentAddressingMode_)
	{
	case am_imp:
		LogStrings::log_address_style = "imp";
		//Log("Addressing Mode: IMP");
		taskQueue_.push(MERGE_ADDRESS);
		break;
	case am_imm:
		LogStrings::log_address_style = "imm";
		//Log("Addressing Mode: IMM");
		taskQueue_.push(MERGE_ADDRESS);
		break;
	case am_zp:
		LogStrings::log_address_style = "zp ";
		//Log("Addressing Mode: ZP");
		taskQueue_.push(MERGE_ADDRESS);
		break;
	case am_zpx:
		LogStrings::log_address_style = "zpx";
		//Log("Addressing Mode: ZPX");
		taskQueue_.push(MERGE_ADDRESS);
		break;
	case am_zpy:
		LogStrings::log_address_style = "zpy";
		//Log("Addressing Mode: ZPY");
		taskQueue_.push(MERGE_ADDRESS);
		break;
	case am_izx:
		LogStrings::log_address_style = "izx";
		//Log("Addressing Mode: IZX");
		taskQueue_.push(CPU_TASKS::READ_LD);
		taskQueue_.push(CPU_TASKS::READ_HD);
		//taskQueue_.push(READ_H);
		taskQueue_.push(MERGE_ADDRESS);
		//currentAddressingMode_ = am_zpx;

		break;
	case am_izy:
		LogStrings::log_address_style = "izy";
		//Log("Addressing Mode: IZY");
		taskQueue_.push(READ_D);
		taskQueue_.push(READ_LD);
		taskQueue_.push(READ_HD);
		taskQueue_.push(MERGE_ADDRESS);
		break;
	case am_abs:
		LogStrings::log_address_style = "abs";
		//Log("Addressing Mode: ABS");
		taskQueue_.push(READ_H);
		taskQueue_.push(MERGE_ADDRESS);
		break;
	case am_abx:
		LogStrings::log_address_style = "abx";
		//Log("Addressing Mode: ABX");
		taskQueue_.push(READ_H);
		taskQueue_.push(MERGE_ADDRESS);
		break;
	case am_aby:
		LogStrings::log_address_style = "aby";
		//Log("Addressing Mode: ABY");
		taskQueue_.push(READ_H);
		taskQueue_.push(MERGE_ADDRESS);
		break;
	case am_ind:
		LogStrings::log_address_style = "ind";
		//Log("Addressing Mode: IND");
		taskQueue_.push(CPU_TASKS::READ_LD);
		taskQueue_.push(CPU_TASKS::READ_HD);
		taskQueue_.push(MERGE_ADDRESS);
		break;
	case am_rel:
		LogStrings::log_address_style = "rel";
		//Log("Addressing Mode: REL");
		//taskQueue_.push(READ_D);
		taskQueue_.push(MERGE_ADDRESS);
		break;
	}
}




void CPU::Tick()
{
	
	if (taskQueue_.size() == 0)
	{
		taskQueue_.push(CPU_TASKS::READ_OP);
	}
	CPU_TASKS currentTask = taskQueue_.front();
	taskQueue_.pop();
	tickCounter++;
	switch (currentTask)
	{
	case READ_OP:
		oldPC = PC;
		
		{
			LogStrings::log_address = "0000";
			std::string addressStr = intToStr(oldPC);
			for (int i = addressStr.length()-1; i > -1;i--)
			{
				LogStrings::log_address[i] = addressStr[i];
			}
	
		}
		{
			LogStrings::log_A = "A:00";
			std::string a = intToStr(Accumulator_.byte);
			if (a.length() == 1)
			{
				a = "0" + a;
			}

			for (int i = a.length() - 1; i > -1; i--)
			{
				LogStrings::log_A[i + 2] = a[i];
			}
		}
		{
			LogStrings::log_X = "X:00";
			std::string x = intToStr(X_.byte);
			if (x.length() == 1)
			{
				x = "0" + x;
			}
			for (int i = x.length() - 1; i > -1; i--)
			{
				LogStrings::log_X[i + 2] = x[i];
			}
		}
		{
			LogStrings::log_Y = "Y:00";
			std::string y = intToStr(Y_.byte);
			if (y.length() == 1)
			{
				y = "0" + y;
			}

			for (int i = y.length() - 1; i > -1; i--)
			{
				LogStrings::log_Y[i + 2] = y[i];
			}
		}

		{
			LogStrings::log_NVBDI_ZC = "P:00";
			std::string p = intToStr(NVBDI_ZC_.byte);
			if (p.length() == 1)
			{
				p = "0" + p;
			}

			for (int i = p.length() - 1; i > -1; i--)
			{
				LogStrings::log_NVBDI_ZC[i + 2] = p[i];
			}
		}

		{
			LogStrings::log_SP = "SP:00";
			std::string sp = intToStr(SP);
			for (int i = sp.length() - 1; i > -1; i--)
			{
				LogStrings::log_SP[i + 3] = sp[i];
			}
		}
		
		ADL_ = 0;
		ADH_ = 0;
		LDAT_ = 0;
		HDAT_ = 0;
		//Log("");
		//LogDate();
		address_ = PC;
		OP_= Read(address_, OP);
		PC++;
		if (oldPC == 0xFD6E)
		{
			bool breakhere = true;
		}
		LogStrings::log_op = "" + intToStr(OP_);
		if (OP_ == 0x99)
		{
			bool breakhere = true;
		}
		//Log("OP: 0x" + intToStr(OP_));
		//Log("PC: " + intToStr(PC));	PC++;
		address_ = PC;
		ADL_ = Read(address_, OP);
		PC++;
		EvaluateOpAddressMode();
		break;

	case READ_H:
		
		address_ = PC;
		ADH_ = Read(address_, HIGH);
		PC++;
		break;
	case READ_LD:

		address_ = LDAT_;
		ADL_ = Read(address_, LOW);
		break;

		
	case READ_HD:
		address_ = HDAT_;
		ADH_ = Read(address_, HIGH);
		break;

	case READ_A:
		LDAT_ = Accumulator_.byte;
		break;
		
	case READ_D:
		LDAT_ = Read(address_, DATA);
		break;
	case WRITE:
		Write(address_, LDAT_);
		break;

	case WRITE_LD:
		Write(address_, LDAT_);
		break;
	case WRITE_HD:
		Write(address_+1, HDAT_);
		break;

	case EXECUTE_OP:
		
	{
		LogStrings::log_op_address = "0000";
		std::string addrStr = intToStr(address_);
		while (addrStr.length() <4 )
		{
			addrStr = "0" + addrStr;
		}
		LogStrings::log_op_address = addrStr;
	}
		
		ExecuteOp();
		tickCounter--;

		
		log();
		//static std::string log_address;
		//log_op = intToStr(OP_);
		//log_op_name;
		//static std::string log_address_style;
		//static std::string log_A =;
		//static std::string log_X;
		//static std::string log_Y;
		//static std::string log_NVBDI_ZC;
		//static std::string log_SP;
		
		/*Log("TickCount: " + intToStr(tickCounter) );
		Log("NVBDI_ZC: "
			+ intToStr(1 == NVBDI_ZC_.N) + " "
			+ intToStr(1 == NVBDI_ZC_.V) + " "
			+ intToStr(1 == NVBDI_ZC_.B) + " "
			+ intToStr(1 == NVBDI_ZC_.D) + " "
			+ intToStr(1 == NVBDI_ZC_.I) + " "
			+ intToStr(1 == 0) + " "
			+ intToStr(1 == NVBDI_ZC_.Z) + " "
			+ intToStr(1 == NVBDI_ZC_.C));
		Log("ADL: 0x" + intToStr(ADL_));
		Log("ADG: 0x" + intToStr(ADH_));
		Log("DAT: 0x" + intToStr(DAT_));
		Log("X: " + intToStr(X_.byte));
		Log("Y: " + intToStr(Y_.byte));
		Log("SP: " + intToStr(SP));
		{
			int stepSize = PC - oldPC;

			Log("Step Size: " + intToStr(stepSize));
		}
		Log("Address: 0x" + intToStr(address_));
		*/
		tickCounter = 0;
		break;
	case MERGE_ADDRESS:
		mergeAddress();
		break;

	}
	if (taskQueue_.size() != 0)
	{
		CPU_TASKS nextTask = taskQueue_.front();
		switch (nextTask)
		{
		case MERGE_ADDRESS:
		case EXECUTE_OP:
			Tick();
			break;
		}
	}



}
//OpImpl
//Logical and arithmetic commands :

void CPU::ORA()
{
	Accumulator_.byte = Accumulator_.byte | LDAT_;
	checkNZ(Accumulator_.byte);
}

void CPU::AND()
{
	Accumulator_.byte = Accumulator_.byte & LDAT_;
	checkNZ(Accumulator_.byte);
}

void CPU::EOR()
{
	Accumulator_.byte = Accumulator_.byte ^ LDAT_;
	checkNZ(Accumulator_.byte);
}

void CPU::ADC()
{
	Accumulator_.byte = Accumulator_.byte + LDAT_;
	checkNZ(Accumulator_.byte);
}

void CPU::SBC()
{
	Accumulator_.byte = Accumulator_.byte - LDAT_;
	checkNZ(Accumulator_.byte);
}

void CPU::CMP()
{
	checkNZ((Accumulator_.byte - LDAT_) & 0xff);
	C(Accumulator_.byte >= LDAT_);

}

void CPU::CPX()
{
	if (Z())
	{
		unsigned char hi = PC >> 8;
		unsigned char lo = PC & 0xff;
		lo += LDAT_;

		PC = lo + (hi << 8);
	}
}

void CPU::CPY()
{
	if (Z())
	{
		unsigned char hi = PC >> 8;
		unsigned char lo = PC & 0xff;
		lo += LDAT_;

		PC = lo + (hi << 8);
	}
}

void CPU::DEC()
{
	if (Z())
	{
		PC += LDAT_;
	}
}

void CPU::DEX()
{
	X_.byte -= 1;
	checkNZ(X_.byte);
}

void CPU::DEY()
{
	Y_.byte -= 1;
	checkNZ(Y_.byte);
}

void CPU::INC()
{
	unsigned char data = Read(address_, DATA);
	if (LDAT_ == 0xa0)
	{
		bool breakhere = true;
	}
	LDAT_ = data + 1;
	taskQueue_.push(CPU_TASKS::WRITE);
	checkNZ(LDAT_);
}

void CPU::INX()
{
	X_.byte += 1;
	checkNZ(X_.byte);
}
void CPU::INY()
{
	Y_.byte += 1;
	checkNZ(Y_.byte);
}

void CPU::ASL()
{
	//int addr = address_;
	//Read(addr, DATA);
	unsigned char v = LDAT_;
	int ov = v;

	int newCarry = v & 0x80;
	v <<= 1;


	if (newCarry != 0)
		C(true);
	v &= 0xff;
	checkNZ(v);

	LDAT_ = v;

	taskQueue_.push(WRITE_LD);

	//Write((addr+ X_.byte) & 0xff, ov);
	//Write((addr + X_.byte) & 0xff, v);
}

void CPU::ROL()
{
	int newCarry = LDAT_ & 0x80;
	LDAT_ <<= 1;
	
	if ((NVBDI_ZC_.byte & 1) != 0)
	{
		LDAT_ |= 1;
	}
		
	NVBDI_ZC_.byte &= ~1;
	if (newCarry != 0)
	{ 
		LDAT_ |= 1;
		LDAT_ &= 0xff;
	}
	checkNZ(LDAT_);
	if (currentAddressingMode_ == am_imp)
	{
		WriteA(LDAT_);
	}
	else
	{
		Write(address_, LDAT_);
	}


}

void CPU::LSR()
{

}

void CPU::ROR()
{

}

//Move Commands

void CPU::LDA()
{
	Accumulator_.byte = LDAT_;
	checkNZ(Accumulator_.byte);
}

void CPU::STA()
{
	Write(address_, Accumulator_.byte);
}

void CPU::LDX()
{
	X_.byte = LDAT_;
	checkNZ(X_.byte);
}

void CPU::STX()
{
	Write(address_, X_.byte);
}

void CPU::LDY()
{
	Y_.byte = LDAT_;
	checkNZ(Y_.byte);
}

void CPU::STY()
{
	LDAT_ = Y_.byte;
	taskQueue_.push(WRITE_LD);
	//Write(address_, Y_.byte);
}

void CPU::TAX()
{
	X_.byte = Accumulator_.byte;
	checkNZ(X_.byte);
}

void CPU::TXA()
{
	Accumulator_.byte = X_.byte;
	checkNZ(Accumulator_.byte);
}

void CPU::TAY()
{
	Y_.byte = Accumulator_.byte;
	checkNZ(Y_.byte);
}

void::CPU::TYA()
{
	Accumulator_.byte = Y_.byte;
	checkNZ(Accumulator_.byte);
}

void::CPU::TSX()
{

}

void CPU::TestAddress()
{
	for (unsigned char hi = 0; hi < 0xff; hi++)
	{
		for (unsigned char lo = 0; lo < 0xff; lo++)
		{
			for (unsigned int mode = am_imp; mode < NR_AM; mode++)
			{
				//mergeAddress()

			}

		}
	}
}

void CPU::TXS()
{
	SP = X_.byte;
}



void CPU::PLA()
{

}

void CPU::PHA()
{

}

void CPU::PLP()
{

}

void CPU::PHP()
{

}


//Jump/Flag Commands

void CPU::BPL()
{
	if (!N())
	{
		unsigned char hi = PC >> 8;
		unsigned char lo = PC & 0xff;
		lo += LDAT_;

		PC = lo + (hi << 8);
	}
}
void CPU::BMI()
{
	if (N())
	{
		unsigned char hi = PC >> 8;
		unsigned char lo = PC & 0xff;
		lo += LDAT_;

		PC = lo + (hi << 8);
	}
}
void CPU::BVC()
{
	if (!V())
	{
		unsigned char hi = PC >> 8;
		unsigned char lo = PC & 0xff;
		lo += LDAT_;

		PC = lo + (hi << 8);
	}
}
void CPU::BVS()
{
	if (V())
	{
		unsigned char hi = PC >> 8;
		unsigned char lo = PC & 0xff;
		lo += LDAT_;

		PC = lo + (hi << 8);
	}
}
void CPU::BCC()
{
	if (!C())
	{
		PC = (PC + LDAT_) - 1;
	}
}
void CPU::BCS()
{
	if (C())
	{
		unsigned char hi = PC >> 8;
		unsigned char lo = PC & 0xff;
		lo += LDAT_;

		PC = lo + (hi << 8);
	}
}
void CPU::BNE()
{
	if (Z() == 0)
	{
		unsigned char hi = PC >> 8;
		unsigned char lo = PC & 0xff;
		lo += LDAT_;

		PC = lo + (hi << 8);

		//PC += DAT_;//ADL_ + (ADH_ << 8);
	}

}
void CPU::BEQ()
{
	if (Z() ==1)
	{
		unsigned char hi = PC >> 8;
		unsigned char lo = PC & 0xff;
		lo += LDAT_;

		PC = lo + (hi << 8);
	}
}


void CPU::BRK()
{
	std::stringstream ss;
	ss << "BRK NOT IMPLEMENTED";
	Log(ss.str());
	platform_->dumpRam();
	exit(1);
}

void CPU::RTI()
{

}

void CPU::JSR()
{
	unsigned char lo = (PC - 1) & 0xff;
	unsigned char hi = (PC - 1) >> 8;
	push(hi);
	push(lo);
	PC = address_;
}

void CPU::RTS()
{
	unsigned short adr = 0;
	adr |= pop();
	adr |= pop() << 8;
	PC = adr + 1;
}

void CPU::JMP()
{
	PC = address_;
}

void CPU::BIT()
{

}

void CPU::CLC()
{
	C(false);
}
void CPU::SEC()
{
	C(true);
}

void CPU::CLD()
{
	D(false);
}

void CPU::SED()
{
	D(true);
}

void CPU::CLI()
{
	I(false);
}

void CPU::SEI()
{
	I(true);
}

void CPU::CLV()
{
	V(false);
}



//current




void CPU::ISC()
{
	PC = PC + 1;
	Accumulator_.byte = Accumulator_.byte - PC;
	checkNZ(Accumulator_.byte);

}

void CPU::SLO()
{
	Log("ILLEGAL OP CODE!!!");
	PC = PC * 2;
	Accumulator_.byte = (Accumulator_.byte | PC);
}






















void CPU::NOP()
{
}







