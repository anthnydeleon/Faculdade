-- Copyright (C) 2025  Altera Corporation. All rights reserved.
-- Your use of Altera Corporation's design tools, logic functions 
-- and other software and tools, and any partner logic 
-- functions, and any output files from any of the foregoing 
-- (including device programming or simulation files), and any 
-- associated documentation or information are expressly subject 
-- to the terms and conditions of the Altera Program License 
-- Subscription Agreement, the Altera Quartus Prime License Agreement,
-- the Altera IP License Agreement, or other applicable license
-- agreement, including, without limitation, that your use is for
-- the sole purpose of programming logic devices manufactured by
-- Altera and sold by Altera or its authorized distributors.  Please
-- refer to the Altera Software License Subscription Agreements 
-- on the Quartus Prime software download page.

-- ***************************************************************************
-- This file contains a Vhdl test bench template that is freely editable to   
-- suit user's needs .Comments are provided in each section to help the user  
-- fill out necessary details.                                                
-- ***************************************************************************
-- Generated on "07/23/2025 21:27:42"
                                                            
-- Vhdl Test Bench template for design  :  seq_rec_moore
-- 
-- Simulation tool : Questa Intel FPGA (VHDL)

LIBRARY ieee;                                               
USE ieee.std_logic_1164.all;                                

ENTITY seq_rec_moore_vhd_tst IS
END seq_rec_moore_vhd_tst;

ARCHITECTURE seq_rec_moore_arch OF seq_rec_moore_vhd_tst IS
-- constants                                                 
-- signals                                                   
SIGNAL CLK : STD_LOGIC;
SIGNAL RESET : STD_LOGIC;
SIGNAL X : STD_LOGIC;
SIGNAL Z : STD_LOGIC;
SIGNAL state_A, state_B, state_C, state_D, state_E : STD_LOGIC;

COMPONENT seq_rec_moore
	PORT (
		CLK : IN STD_LOGIC;
		RESET : IN STD_LOGIC;
		X : IN STD_LOGIC;
		Z : BUFFER STD_LOGIC;
		state_A : OUT STD_LOGIC;
		state_B : OUT STD_LOGIC;
		state_C : OUT STD_LOGIC;
		state_D : OUT STD_LOGIC;
		state_E : OUT STD_LOGIC
	);
END COMPONENT;

BEGIN
	i1 : seq_rec_moore
		PORT MAP (-- list connections between master ports and signals
			CLK => CLK,
			RESET => RESET,
			X => X,
			Z => Z,
			state_A => state_A,
         state_B => state_B,
         state_C => state_C,
         state_D => state_D,
         state_E => state_E
	);

init : PROCESS                                               
-- variable declarations                                     
BEGIN                                                        
	for i in 0 to 29 loop   -- 50 ciclos de clock = 1000 ns
		CLK <= '0'; wait for 10 ns;
		CLK <= '1'; wait for 10 ns;
	end loop;                 
WAIT;                                                       
END PROCESS init;  
                                           
always : PROCESS                                              
-- optional sensitivity list                                  
-- (        )                                                 
-- variable declarations                                      
BEGIN                                                         
        -- code executes for every event on sensitivity list  
		  RESET <= '1';
        X <= '0';
        WAIT FOR 40 ns; -- 2 ciclos de clock

        RESET <= '0';
        WAIT FOR 20 ns; -- 1 ciclo de clock

        -- Entrada: 1 1 0 1 -> Deve reconhecer
		  X <= '1'; WAIT FOR 20 ns;
        X <= '1'; WAIT FOR 20 ns;
        X <= '0'; WAIT FOR 20 ns;
        X <= '1'; WAIT FOR 20 ns;
        
        -- Entrada: 1 0 1 0 -> Deve reconhecer com o 1 da última entrada
        X <= '1'; WAIT FOR 20 ns;
        X <= '0'; WAIT FOR 20 ns;
        X <= '1'; WAIT FOR 20 ns;
        X <= '0'; WAIT FOR 20 ns;
		  
		  -- Entrada: 1 0 1 1 -> Não deve reconhecer
        X <= '1'; WAIT FOR 20 ns;
        X <= '0'; WAIT FOR 20 ns;
        X <= '1'; WAIT FOR 20 ns;
        X <= '1'; WAIT FOR 20 ns;
		  
		  -- Entrada: 1 0 1 1 -> Deve reconhecer com o 1 da última entrada
        X <= '1'; WAIT FOR 20 ns;
        X <= '0'; WAIT FOR 20 ns;
        X <= '1'; WAIT FOR 20 ns;
        X <= '1'; WAIT FOR 20 ns;
WAIT;                                                         
END PROCESS always;                                        
END seq_rec_moore_arch;