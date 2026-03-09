-- Sequence Recognizer: VHDL Process Description
library ieee;
use ieee.std_logic_1164.all;

entity seq_rec is
   port(
      KEY: in std_logic_vector(1 downto 0); -- KEY(1): reset, KEY(0): clock
      SW: in std_logic_vector(0 downto 0);  -- SW(0): entrada X
      LEDR: out std_logic_vector(3 downto 0); -- Indica estado (A–D)
      HEX0: out std_logic_vector(6 downto 0); -- Mostra letra do estado
      Z: out std_logic  -- Saída da máquina
   );
end seq_rec;

architecture process_3 of seq_rec is
   type state_type is (A, B, C, D);
   signal state, next_state : state_type;
   signal X: std_logic;
	
begin
   X <= SW(0); -- Entrada X controlada pela chave SW(0)

	-- Process 1 - state_register: implements positive edge-triggered
	-- state storage with asynchronous reset. 
	state_register: process(KEY)
	begin
		if (KEY(1) = '0') then -- Reset (botão pressionado)
			state <= A;
		elsif rising_edge(KEY(0)) then -- Clock (botão pressionado)
			state <= next_state;
		end if;
	end process;

	-- Process 2 - next_state_function: implements next state as function
	-- of input X and state. 
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
			if X = '1' then
				next_state <= C;
			else
				next_state <= D;
			end if;

		when D =>
			if X = '1' then
				next_state <= B;
			else
				next_state <= A;
			end if;
		end case;
	end process;
	
	-- Process 3 - output_function: implements output as function of input X and state. 
	output_func: process (X, state)
	begin
		case state is
			when A =>
				Z <= '0';
				
			when B =>
				Z <= '0';
			
			when C =>
				Z <= '0';
			
			when D =>
			if X = '1' then
				Z <= '1';
			else
				Z <= '0';
			end if;
		end case;
	end process;

   -- Process 4: indica visualmente o estado atual
   current_state_ledr: process(state)
   begin
      LEDR <= "0000";
      case state is
         when A => LEDR(0) <= '1';
         when B => LEDR(1) <= '1';
         when C => LEDR(2) <= '1';
         when D => LEDR(3) <= '1';
      end case;
   end process;

   -- HEX0: exibe a letra correspondente ao estado atual
   with state select
      HEX0(6 downto 0) <=
         "0001000" when A,  -- A
         "0000011" when B,  -- b
         "1000110" when C,  -- C
         "0100001" when D;  -- d
end;