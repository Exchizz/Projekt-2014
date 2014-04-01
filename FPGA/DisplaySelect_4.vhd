----------------------------------------------------------------------------------
-- Company: 
-- Engineer: 
-- 
-- Create Date:    10:51:34 03/20/2014 
-- Design Name: 
-- Module Name:    DisplaySelect_4-7segDisp - Behavioral 
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


entity DisplaySelect_4_7segDisp is
    Port ( DisplayData : in  STD_LOGIC_VECTOR (19 downto 0);
           DisplayAN : out  STD_LOGIC_VECTOR (3 downto 0);
           DisplayPart : out  STD_LOGIC_VECTOR (4 downto 0);
			  ClkSignal : in STD_LOGIC);
end DisplaySelect_4_7segDisp;

architecture Behavioral of DisplaySelect_4_7segDisp is
Signal Display_State : STD_LOGIC_VECTOR (1 downto 0) := "00";

BEGIN
-- state changer --
PROCESS (ClkSignal)
	variable time_count: integer range 0 to 1000 := 0;

BEGIN
	if rising_edge(ClkSignal) then
		time_count := time_count + 1;
		
		if (time_count = 0) then
			Display_State <= Display_State +1;	
		end if;
	end if;
end process; 

-- bestem AN --
WITH Display_State SELECT
DisplayAN <= "1110" WHEN "00",
				 "1101" WHEN "01",
				 "1011" WHEN "10",
				 "0111" WHEN "11",
				 "1111" WHEN OTHERS;

-- bestem output --
WITH Display_State SELECT
DisplayPart <= DisplayData(4 downto 0) WHEN "00",
					DisplayData(9 downto 5) WHEN "01",
					DisplayData(14 downto 10) WHEN "10",
					DisplayData(19 downto 15) WHEN "11",
					"00000" WHEN OTHERS;
end Behavioral;

