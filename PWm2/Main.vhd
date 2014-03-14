----------------------------------------------------------------------------------
-- Company: 
-- Engineer: 
-- 
-- Create Date:    18:20:22 03/12/2014 
-- Design Name: 
-- Module Name:    Main - Behavioral 
-- Project Name: 
-- Target Devices: 
-- Tool versions: 
-- Description: 
--
-- Dependencies: 
--
-- Revision: 
-- Revision 0.01 - File Created
-- Additional Comments: 
--
----------------------------------------------------------------------------------
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;

-- Uncomment the following library declaration if using
-- arithmetic functions with Signed or Unsigned values
--use IEEE.NUMERIC_STD.ALL;

-- Uncomment the following library declaration if instantiating
-- any Xilinx primitives in this code.
--library UNISIM;
--use UNISIM.VComponents.all;

entity Main is
port (
	clk: in std_logic;
	LED: out std_logic_vector (7 downto 0);
	Pin: out std_logic_vector (7 downto 0);
	switch: in std_logic_vector (7 downto 0)
	);
	
	
end Main;

architecture Behavioral of Main is
signal prescaler: 		         integer						   range 0 to 2047;  -- BINÆRT OPTÆlning => Optil 2^n.  Solve (50000000/ Frekvens = "Antal nærmeste 2^n værdi")
--signal counter: 					integer 						   range 1 to 50000000;
shared variable duty: 			   integer   						range 0 to 2047; -- 50 % duty
begin

switch_to_int: process(switch) -- Convert Switch (bin) to (dec)
variable conv_switch: 				integer							range  0 to 255;
begin
	
	conv_switch :=  (conv_integer(switch));
	duty := conv_switch * 8; 
	LED <=  conv_std_logic_vector(conv_switch,8);
end process;

Presccaler: process(clk)
begin
	if rising_edge(clk) then 
		prescaler <= prescaler + 1;	
	end if;
end process;



pwm: process(prescaler)
begin
	if(prescaler > duty) then 
		pin <= "00000000";
	else 
		pin <= "11111111";
	end if;
end process;


end Behavioral;

