-- Sequence Recognizer: VHDL Moore Machine
library ieee;
use ieee.std_logic_1164.all;

entity seq_rec_moore is
   port(
      KEY: in std_logic_vector(1 downto 0); -- KEY(1): reset, KEY(0): clock
      SW: in std_logic_vector(0 downto 0);  -- SW(0): entrada X
      LEDR: out std_logic_vector(4 downto 0); -- Indica estado (A–E)
      HEX0: out std_logic_vector(6 downto 0)  -- Mostra letra do estado
   );
end seq_rec_moore;

architecture moore_arch of seq_rec_moore is
   type state_type is (A, B, C, D, E); -- 'E' é o estado adicional onde Z = 1
   signal state, next_state : state_type;
	signal X: std_logic;
	
begin
	X <= SW(0); -- Atribui chave como entrada aos sinal X
	
	
	
-- Process 1 - state_register: implements positive edge-triggered
state_register: process (KEY)
begin
	if (KEY(1) = '0') then -- Reset (botão pressionado)
		state <= A;
	elsif rising_edge(KEY(0)) then -- Clock (botão pressionado)
		state <= next_state;
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


   -- LEDR: cada bit representa um estado
current_state_ledr: process(state)
begin
	LEDR <= "00000";
	case state is
		when A => LEDR(0) <= '1';
		when B => LEDR(1) <= '1';
		when C => LEDR(2) <= '1';
		when D => LEDR(3) <= '1';
		when E => LEDR(4) <= '1';
	end case;
end process;

-- HEX0: letra do estado atual
-- current_state_display process(state)
-- begin
-- 	case state is
-- 		when A => HEX0 <= "0001000"; -- A
-- 		when B => HEX0 <= "0000011"; -- b
-- 		when C => HEX0 <= "1000110"; -- C
-- 		when D => HEX0 <= "0100001"; -- d
-- 		when E => HEX0 <= "0000110"; -- E
-- 	end case;
-- end process;

   with state select
      HEX0(6 downto 0) <=
         "0001000" when A,
         "0000011" when B,
         "1000110" when C,
         "0100001" when D,
         "0000110" when E;

end;