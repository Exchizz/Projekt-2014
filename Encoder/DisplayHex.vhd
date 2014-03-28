----------------------------------------------------------------------------------
-- Company: 
-- Engineer: 
-- 
-- Create Date:    09:28:34 03/20/2014 
-- Design Name: 
-- Module Name:    DisplayHex - Behavioral 
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


entity DisplayHex is
    Port ( Segment : out  STD_LOGIC_VECTOR (7 downto 0);
			  DisplayThis : in STD_LOGIC_VECTOR (4 downto 0));
end DisplayHex;

architecture Behavioral of DisplayHex is

begin
------------- Show the corresponding number on the display -----------------
Segment(0) <= '1' WHEN ((DisplayThis(3) = '1'  AND DisplayThis(0) = '1') AND NOT(DisplayThis(1) = DisplayThis(2)))
				OR ((DisplayThis(3) = '0' AND DisplayThis(1) = '0') AND NOT(DisplayThis(0) = DisplayThis(2)))
				ELSE '0';
Segment(1) <= '1' WHEN (DisplayThis(3) = '1' AND DisplayThis(2) = '1' AND DisplayThis(1) = '1')
				OR (DisplayThis(3) = '1' AND DisplayThis(0) = DisplayThis(1) AND NOT(DisplayThis(2) = DisplayThis(1))) 
				OR ((DisplayThis(3) = '0' AND DisplayThis(2) = '1') AND NOT(DisplayThis(1) = DisplayThis(0)))
				ELSE '0';
Segment(2) <= '1' WHEN (DisplayThis(0) = '0' AND DisplayThis(1) = '1' AND DisplayThis(3) = DisplayThis(2))
				OR (DisplayThis(3) = '1' AND DisplayThis(2) = '1' AND DisplayThis(0) = DisplayThis(1))
				ELSE '0';
Segment(3) <= '1' WHEN (DisplayThis(0) = '1' AND DisplayThis(1) = '1' AND DisplayThis(2) = '1')
				OR (DisplayThis(3) = '0' AND DisplayThis(1) = '0' AND NOT(DisplayThis(0) = DisplayThis(2)))
				OR (DisplayThis(3) = '1' AND DisplayThis(2) = '0' AND DisplayThis(1) = '1' AND DisplayThis(0) = '0')
				ELSE '0';
Segment(4) <= '1' WHEN (DisplayThis(2) = '0' AND DisplayThis(1) = '0' AND DisplayThis(0) = '1')
				OR (DisplayThis(3) = '0' AND DisplayThis(0) = '1')
				OR (DisplayThis(3) = '0' AND DisplayThis(2) = '1' AND DisplayThis(1) = '0')
				ELSE '0';
Segment(5) <= '1' WHEN (DisplayThis(1) = '0' AND DisplayThis(0) = '1' AND DisplayThis(3) = DisplayThis(2))
				OR (DisplayThis(3) = '0' AND DisplayThis(1) = '1' AND DisplayThis(0) = '1')
				OR (DisplayThis(3) = '0' AND DisplayThis(2) = '0' AND DisplayThis(1) = '1')
				ELSE '0';
Segment(6) <= '1' WHEN (DisplayThis(1) = '0' AND DisplayThis(0) = '0' AND DisplayThis(3) = DisplayThis(2))
				OR (DisplayThis(3) = '0' AND DisplayThis(0) = '1' AND DisplayThis(2) = DisplayThis(1))
				ELSE '0';
Segment(7) <= '1' WHEN DisplayThis(4) = '0' ELSE '0';


end Behavioral;

