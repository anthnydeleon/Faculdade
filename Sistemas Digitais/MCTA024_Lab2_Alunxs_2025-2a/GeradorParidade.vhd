library ieee;
use ieee.std_logic_1164.all;

entity GeradorParidade is
	port(D3, D2, D1, D0: in std_logic;
	     P: out std_logic);
end GeradorParidade;

architecture Comportamental of GeradorParidade is
begin
	P <= D3 xor D2 xor D1 xor D0;
end Comportamental;
