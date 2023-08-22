#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"

#define OLC_PGEX_QUICKGUI
#include "olcPGEX_QuickGUI.h"

#include "assembler.h"
#include "PEPE8.h"

#include <iostream>
#include <sstream>

class gui : public olc::PixelGameEngine
{
public:
	gui() { sAppName = "PEPE-8 Emulator"; }

	PEPE8 processor;

	bool darkmode = true;

	std::string hex(uint32_t n, uint8_t d)
	{
		std::string s(d, '0');
		for (int i = d - 1; i >= 0; i--, n >>= 4)
			s[i] = "0123456789ABCDEF"[n & 0xF];
		return s;
	};

	std::string bin(int8_t number)
	{
		// Convert the int8_t number to an unsigned char for consistency
		// with std::bitset's constructor
		uint8_t unsignedNumber = static_cast<uint8_t>(number);

		// Convert the unsigned number to a binary representation string
		std::bitset<2> binaryBits(unsignedNumber);

		// Return the binary representation string
		return binaryBits.to_string();
	}

	void DrawRam(int x, int y) //not used anymore, draws ram line by line
	{
		uint16_t nAddr = 0x0000;
		int nRows = 16, nColumns = 16;

		for (int row = 0; row < nRows; row++)
		{
			std::string sOffset = hex(nAddr, 2) + "H" +  ":";
			for (int col = 0; col < nColumns; col++)
			{
				int8_t memory = processor.data[nAddr];
				sOffset += " " + hex(memory, 2);
				nAddr += 1;
			}
			DrawString(x, y, sOffset);
			y += 10;
		}
	}

	void DrawRamCells(int x, int y) //draws ram cell by cell allowing different colors for each cell
	{
		uint16_t nAddr = 0x0000;
		int nRows = 16, nColumns = 16, start = x;

		for (int row = 0; row < nRows; row++)
		{
			DrawString(x, y, (hex(nAddr, 2) + "H" + ":"));
			x += 10;
			for (int col = 0; col < nColumns; col++)
			{
				x += 25;
				int8_t memory = processor.data[nAddr];
				if (memory == 0)
				{
					DrawString(x, y, hex(memory, 2));
				}
				else
				{
					DrawString(x, y, hex(memory, 2), olc::BLUE);
				}
				
				nAddr += 1;
			}
			y += 10;
			x = start;
		}
	}

	void DrawCpu(int x, int y)
	{
		DrawString(x , y , "PEPE-8:", olc::YELLOW);
		//DrawString(x  + 64, y, "N", nes.cpu.status & olc6502::N ? olc::GREEN : olc::RED);
		DrawString(x , y + 15, "PC: " + hex(processor.PC, 4) + " H");
		DrawString(x , y + 30, "A: " +  std::to_string(processor.A) + "d  [" + hex(processor.A, 2) + " H]");
		DrawString(x , y + 45, "SEL_PC: " +  bin(processor.SEL_PC));
		DrawString(x , y + 60, "SEL_ALU: " +  bin(processor.SEL_ALU));
		DrawString(x , y + 75, "ESCR_A: " +  std::to_string(processor.ESCR_A));
		DrawString(x , y + 90, "SEL_A: " +  std::to_string(processor.SEL_A));
		DrawString(x , y + 105, "SEL_B: " +  std::to_string(processor.SEL_B));
		DrawString(x , y + 120, "WR: " +  std::to_string(processor.WR));
	}

	void DrawCode(int x, int y, int nLines)
	{
		int programSize;
		for (int i = 0; i < processor.decoded_instructions.size(); i++)
		{
			if (processor.decoded_instructions[i] == "END")
			{
				programSize = i;
				break;
			}
		}
		
		int j = processor.PC; // Index for drawing lines above and below current instruction

		int nLineY = (nLines >> 1) * 10 + y; //number of lines divided by 2 times 10 (height of each line) plus y (starting point)
		if (j < programSize)
		{
			DrawString(x, nLineY, processor.decoded_instructions[j], olc::CYAN);
			while (nLineY < (nLines * 10) + y)
			{
				nLineY += 10;
				++j;
				if (j < programSize)
				{
					DrawString(x, nLineY, processor.decoded_instructions[j]);
				}
			}
		}

		j = processor.PC; //reset j
		nLineY = (nLines >> 1) * 10 + y; //reset also

		if (j < programSize && j >= 0)
		{
			while (nLineY > y)
			{
				nLineY -= 10;
				--j;
				if (j >= 0)
				{
					DrawString(x, nLineY, processor.decoded_instructions[j]);
				}
			}
		}
	}

	bool OnUserCreate()
	{
		processor.loadProgram();
		//processor.checkProgram();
		return true;
	}

	bool OnUserUpdate(float fElapsedTime)
	{
		if (GetKey(olc::Key::SPACE).bPressed) processor.clock();
		if (GetKey(olc::Key::D).bPressed) darkmode = !darkmode;
		if (GetKey(olc::Key::R).bPressed) processor.reset();

		if (darkmode)
		{
			Clear(olc::VERY_DARK_GREY);
		}
		else
		{
			Clear(olc::DARK_GREY);
		}
		
		DrawCode(448, 150, 12);
		DrawRamCells(5, 5);
		DrawCpu(448, 5);
		DrawString(10, 170, "SPACE = Step Instruction");
		DrawString(10, 180, "R = Reset processor");
		DrawString(10, 190, "D = Dark Mode ON/OFF");
		DrawString(10, 285, "josedsferreira.github.io", olc::GREY);

		if (processor.haltFlag)
		{
			DrawString(10, 200, "Program execution completed!", olc::RED);
		}
		
		return true;
	}
};

int main(int argc, char const *argv[])
{
	//note: to compile:
	//g++ assembler.cpp gui.cpp PEPE8.cpp -o Emulator -lglfw3 -lopengl32 -lgdi32 -luser32 -lkernel32 -lgdiplus -ldwmapi -lshlwapi

	assembler assembler;
    PEPE8 processor;

    gui gui;
	if (gui.Construct(580, 300, 2, 2)) gui.Start();
}