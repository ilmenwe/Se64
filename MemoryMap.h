#pragma once



class IN;
class OUT;
class IO;

class IRQ;
class Refresh_Counter;
class Raster_Counter;
class Interupt_Logic;
class Video_Matrix_Counter___Row_Counter;
class Video_Matrix___Color_Line;
class Memory_Interface;
class Address_and_Data;
class BA;
class AEC;
class MC0_7;
class Clock_Generator;
class RAS;
class CAS;
class oO;
class oIN;
class Sprite_Data_Buffers;
class Sprite_Data_Sequencer;
class MUX;
class Border_Unit;
class Sync_Generation;
class Color_Generation;
class oColor;
class Video_Output;
class Graphic_Data_Sequencer;


class IRQ
{
	Interupt_Logic* IN_interupt_logic_;
};
class Refresh_Counter
{
	Address_Generator* OUT_address_generator_;
};

class Raster_Counter
{
	Clock_Generator* IN_clock_generator_;
	Interupt_Logic* OUT_interupt_logic_;
	Video_Matrix_Counter___Row_Counter* IN_video_matrix_counter___raster_counter_;
	Video_Matrix_Counter___Row_Counter* OUT_video_matrix_counter___raster_counter_;
};

class Interupt_Logic
{
	IRQ* OUT_irq_;
	MUX* OUT_mux_;
	Raster_Counter* OUT_raster_counter_;
};



class Video_Matrix_Counter___Row_Counter
{
	Raster_Counter* IN_raster_counter_;
	Raster_Counter* OUT_raster_counter_;
	Address_Generator* OUT_address_generator_;
};

class Video_Matrix___Color_Line
{
	Address_Generator* IN_address_generator_;
	Graphic_Data_Sequencer* OUT_graphic_data_sequencer_;
};

class Memory_Interface
{
	Address_Generator* OUT_address_generator_;
	Address_Generator* IN_address_generator_;
	BA* OUT_BA;
	AEC* OUT_AEC;
};
class Address_Generator 
{
	Memory_Interface* IN_memory_interface_;
	Memory_Interface* OUT_memory_interface_;
	Video_Matrix_Counter___Row_Counter* IN_video_matrix_counter___row_counter_;
	MC0_7* IN_MC0_7;
	Video_Matrix___Color_Line* OUT_video_matrix___color_line_;
	Refresh_Counter* IN_refresh_counter_;
};
class Address_and_Data
{
	Memory_Interface* IN_memory_interface_;
	Memory_Interface* OUT_memory_interface_;
};

class BA
{
	Memory_Interface * IN_memory_interface_;
};
class AEC
{
	Memory_Interface * IN_memory_interface_;
};
class MC0_7
{
	Raster_Counter* IN_raster_counter_;
	Address_Generator* OUT_address_Generator_;
};

class Clock_Generator
{
	oIN* IN_oIN_;
	RAS* OUT_RAS_;
	CAS* OUT_CAS_;
	oO* OUT_oO_;
};

class RAS
{
	Clock_Generator * IN_clock_generator_;
};

class CAS {
	Clock_Generator * IN_clock_generator_;
};

class oO 
{
	Clock_Generator * IN_clock_generator_;
};

class oIN 
{
	Clock_Generator * IN_clock_generator_;
};

class Sprite_Data_Buffers
{
	Address_Generator* IN_address_manager_;
	Sprite_Data_Sequencer* OUT_sprite_data_sequencer_;
};
class Sprite_Data_Sequencer
{
	Sprite_Data_Buffers* IN_sprite_data_buffers_;
	MUX* OUT_MUX_;


};
class MUX
{
	Sprite_Data_Sequencer* IN_sprite_data_sequencer_;
	Graphic_Data_Sequencer* IN_graphic_data_sequencer_;

};
class Border_Unit
{
	MUX* IN_MUX_;
	Color_Generation* OUT_color_generation_;
	Raster_Counter* IN_raster_counter_;
};
class Sync_Generation
{

};
class Color_Generation
{
	oColor* IN_oColor_;
	Border_Unit* IN_border_unit;
	Video_Output* OUT_video_output_;
};
class oColor
{
	Color_Generation* OUT_color_generation_;
};
class Video_Output
{
	Sync_Generation* IN_sync_generation_;
	Color_Generation* IN_color_generation_;
};

class Graphic_Data_Sequencer
{
	Video_Matrix___Color_Line* IN_video_matrix___color_line_;
	MUX* OUT_mux_;
};



class VIC
{
private:
	IRQ irq_;
	Refresh_Counter refresh_counter_;
	Raster_Counter raster_counter_;
	Interupt_Logic interupt_logic_;
	Video_Matrix_Counter___Row_Counter video_matrix_counter___row_counter_;
	Video_Matrix___Color_Line video_matrix___color_line_;
	Memory_Interface memory_interface_;
	Address_and_Data address_and_data_;
	BA ba_;
	AEC aec_;
	MC0_7 mc0_7_;
	Clock_Generator clock_generator_;
	RAS ras_;
	CAS cas_;
	oO oO_;
	oIN oIN_;
	Sprite_Data_Buffers sprite_data_buffers_;
	Sprite_Data_Sequencer sprite_data_sequencer_;
	MUX mux_;
	Border_Unit border_unit_;
	Sync_Generation sync_generation_;
	Color_Generation color_generation;
	oColor oColor_;
	Video_Output video_output_;
	Graphic_Data_Sequencer graphic_data_sequencer_;
public:

};