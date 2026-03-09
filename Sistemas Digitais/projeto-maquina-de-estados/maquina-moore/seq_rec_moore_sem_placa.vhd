-- Sequence Recognizer: VHDL Moore Machine
library ieee;
use ieee.std_logic_1164.all;

entity seq_rec_moore is
   port(
		CLK, RESET, X: in std_logic;
		Z: out std_logic;
		state_A, state_B, state_C, state_D, state_E: out std_logic
	);
end seq_rec_moore;

architecture moore_arch of seq_rec_moore is
   type state_type is (A, B, C, D, E); -- 'E' é o estado adicional onde Z = 1
   signal state, next_state : state_type;
begin

-- Process 1 - state_register: implements positive edge-triggered
-- state storage with asynchronous reset. 
state_register: process (CLK, RESET)
begin
	if (RESET = '1') then
	  state <= A;
	else 
		if (CLK'event and ClK = '1') then
			state <= next_state;
		end if;
  end if;
end process;

-- Process 2: next_state_function: implements next state as function
next_state_func: process (X, state)
begin
   case state is
      when A => 
         if X = '1' then 
            next_state <= B;
         else 
            next_state <= A;
         end if;
			
      when B =>
         if X = '1' then
            next_state <= C;
         else 
            next_state <= A; 
         end if;
			
      when C =>
         if X = '0' then 
            next_state <= D;
         else 
            next_state <= C;
         end if;
			
      when D =>
         if X = '1' then 
            next_state <= E;
         else 
            next_state <= A;
         end if;

      when E =>
         if X = '1' then 
            next_state <= C;
         else 
            next_state <= A;
         end if;
   end case;
end process;

-- Process 3: output_function: implements output as function (Moore)
output_func: process (state)
begin
   case state is
      when E =>
         Z <= '1'; -- Estado de aceitação
      when others =>
         Z <= '0';
   end case;
end process;

-- Process 4: state_indicator: turn on the current state
state_indicator: process(state)
begin
	state_A <= '0';
	state_B <= '0';
	state_C <= '0';
	state_D <= '0';
	state_E <= '0';
	
	case state is
		when A => state_A <= '1';
		when B => state_B <= '1';
		when C => state_C <= '1';
		when D => state_D <= '1';
		when E => state_E <= '1';
	end case;
end process;
end;