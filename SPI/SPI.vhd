----------------------------------------------------------------------------------
-- Company: 
-- Engineer: 
-- 
-- Create Date:    09:46:02 04/01/2014 
-- Design Name: 
-- Module Name:    SPI - Behavioral 
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

-- follows the standard of 


-- Uncomment the following library declaration if using
-- arithmetic functions with Signed or Unsigned values
--use IEEE.NUMERIC_STD.ALL;

-- Uncomment the following library declaration if instantiating
-- any Xilinx primitives in this code.
--library UNISIM;
--use UNISIM.VComponents.all;

entity SPI is
	Generic (DataWidth : Integer := 8);
	
	Port( MClk : in  STD_LOGIC;
			--Clk : in  STD_LOGIC;
			SS : in  STD_LOGIC;
         MISO : out  STD_LOGIC;
         MOSI : in  STD_LOGIC;
			DataOut : out  STD_LOGIC_VECTOR(DataWidth-1 downto 0);
			DataIn : in  STD_LOGIC_VECTOR(DataWidth-1 downto 0);
			SignalPackageEnd : out  STD_LOGIC
			);
end SPI;

architecture Behavioral of SPI is
Shared variable DataBufferS : STD_LOGIC_VECTOR(DataWidth-1 downto 0);
Shared variable DataBufferR : STD_LOGIC_VECTOR(DataWidth-1 downto 0);
	
begin

--recieves information through SPI. 
-- follows SPI: SPO = 0, SPH = 0
-- Master: transmits on rising and recieves on falling edge
-- Slave:  sends MSB when SS goes low, thereafter updates the send bit at falling edge. Recieves bit on rising edge.
-- SS is driven high between 2 datatransmissions


SignalPackageEnd <= SS;


Process (MClk,SS)
variable start : STD_LOGIC := '0';
begin
	if SS = '0' then
		if start = '0' then
			DataBufferS := DataIn;
			MISO <= DataBufferS(DataWidth-1);
			DataBufferS := DataBufferS(DataWidth-2 downto 0) & '0';
			start := '1';
		elsif start = '1' then
			if falling_edge(MClk) then
				MISO <= DataBufferS(DataWidth-1);
				DataBufferS := DataBufferS(DataWidth-2 downto 0) & '0';
			end if;
		end if;
		if rising_edge(MClk) then
				DataBufferR := DataBufferR(DataWidth-2 downto 0) & MOSI;
				DataOut <= DataBufferR;
			end if;
	elsif SS = '1' then
		start := '0';
	end if;
end process;


--- vX, seems to have flaws...
--PROCESS (Clk)
--	variable DataBuffer : STD_LOGIC_VECTOR(DataWidth-1 downto 0);
--	Variable pSS : STD_LOGIC := '1';
--	variable pMClk : STD_LOGIC := '1';
--BEGIN 
--	if rising_edge(Clk) then 
--		if SS = '0' AND pSS = '1' then
--			-- SS falling edge --
--			DataBuffer := DataIn;
--			SignalPackageEnd <= '0';
--		elsif SS = '0' AND pSS = '0' then
--			-- SS = 0 --
--			-- test for edges on MClk
--			if MClk = '0' AND pMClk = '1' then
--				-- rising edge --
--				-- recieve data bit --
--				DataBuffer := DataBuffer(DataWidth-2 downto 0) & MOSI;
--			elsif MClk = '1' AND pMClk = '0' then
--				-- falling edge --
--				-- send databit --
--				MISO <= DataBuffer(DataWidth-1);
--			end if;
--		elsif SS = '1' AND pSS = '0' then
--			-- SS rising edge --
--			-- put data out --
--			DataOut <= DataBuffer;
--			-- signal new data in buffer --
--			SignalPackageEnd <= '1';
--		else 
--			-- SS = 1 --
--		end if;
--	end if;
--END PROCESS;

end Behavioral;

