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

entity PWM is
port (
	clk: in std_logic;
	--LED: out std_logic_vector (7 downto 0);
	Pin: out std_logic;
	PWM: in std_logic_vector (7 downto 0)
	);
	
	
end PWM;

architecture Behavioral of PWM is
	signal prescaler:			integer range 0 to 2047;
	shared variable duty: 	integer range 0 to 2047;

begin

switch_to_int: process(PWM) -- Convert Switch (bin) to (dec)
	variable conv_switch: integer range  0 to 255;
begin	
	conv_switch :=  (conv_integer(PWM)); 			--8 bit input converted to variable
	duty := conv_switch * 8; 								--Duty threshold set
	--LED <=  conv_std_logic_vector(conv_switch,8);	--Input written to LED's for debugging purposes.
end process;


Presccaler: process(clk)	--Count up the PWM timer
begin
	if rising_edge(clk) then 
		prescaler <= prescaler + 1;	
	end if;
end process;


pwm_duty: process(prescaler)		--If PWM timer is lower than threshhold, output is high, else output is low.
begin
	if(prescaler > duty) then 
		pin <= '0';
	else 
		pin <= '1';
	end if;
end process;

end Behavioral;

