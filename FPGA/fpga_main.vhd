----------------------------------------------------------------------------------
-- Company: 
-- Engineer: 
-- 
-- Create Date:    16:49:22 03/31/2014 
-- Design Name: 
-- Module Name:    fpga_main - Behavioral 
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
use ieee.std_logic_arith.all;

-- Uncomment the following library declaration if using
-- arithmetic functions with Signed or Unsigned values
--use IEEE.NUMERIC_STD.ALL;

-- Uncomment the following library declaration if instantiating
-- any Xilinx primitives in this code.
--library UNISIM;
--use UNISIM.VComponents.all;

entity fpga_main is
generic (DataSize : Integer := 12);
Port (--SW : in STD_LOGIC_VECTOR (7 downto 0);
		LED : out  STD_LOGIC_VECTOR (7 downto 0);
		AN : out STD_LOGIC_VECTOR (3 downto 0);
		Seg : out STD_LOGIC_VECTOR (7 downto 0);
		BTN : in STD_LOGIC_VECTOR (3 downto 0);
		-- motor & encoder --
		HallA : in STD_LOGIC_VECTOR (2 downto 0);
		HallB : in STD_LOGIC_VECTOR (2 downto 0);
		MotorA : out STD_LOGIC_VECTOR (2 downto 0);
		MotorB : out STD_LOGIC_VECTOR (2 downto 0);
		-- SPI --
		SPI_SS, SPI_MClk, SPI_MOSI : in STD_LOGIC;
		SPI_MISO : out STD_LOGIC;
		-- CLK --
		Clk : in STD_LOGIC);
end fpga_main;


architecture Behavioral of fpga_main is
---- Components ----
COMPONENT MCS is
Port(	PWM_duty : in  STD_LOGIC_VECTOR (7 downto 0);
		Motor_Direction_in : in  STD_LOGIC_VECTOR (1 downto 0);
		Motor_Direction_out : out  STD_LOGIC_VECTOR (2 downto 0);
      Position : out  STD_LOGIC_VECTOR (10 downto 0);
		Hall_sensors : in STD_LOGIC_VECTOR (2 downto 0);
		Clk : in STD_LOGIC;
		ResetPosition : in STD_LOGIC);
end component;

COMPONENT DisplayHex is
Port( Segment : out  STD_LOGIC_VECTOR (7 downto 0);
		DisplayThis : in STD_LOGIC_VECTOR (4 downto 0));
end COMPONENT;

COMPONENT DisplaySelect_4_7segDisp is
Port( DisplayData : in  STD_LOGIC_VECTOR (19 downto 0);
		DisplayAN : out  STD_LOGIC_VECTOR (3 downto 0);
		DisplayPart : out  STD_LOGIC_VECTOR (4 downto 0);
		ClkSignal : in STD_LOGIC);
end COMPONENT;

COMPONENT SPI is
	Generic (DataWidth : Integer := 12);
	Port( MClk : in  STD_LOGIC;
			--Clk : in  STD_LOGIC;
			SS : in  STD_LOGIC;
         MISO : out  STD_LOGIC;
         MOSI : in  STD_LOGIC;
			DataOut : out  STD_LOGIC_VECTOR(DataWidth-1 downto 0);
			DataIn : in  STD_LOGIC_VECTOR(DataWidth-1 downto 0);
			SignalPackageEnd : out  STD_LOGIC
			);
end COMPONENT;

---- Signals for components ----
-- signal for 7seg disp --
SIGNAL DisplayThis_DisplayPart_connect : STD_LOGIC_VECTOR (4 downto 0);
SIGNAL Display : STD_LOGIC_VECTOR (19 downto 0);
-- Encoder --
SIGNAL PositionA : STD_LOGIC_VECTOR (10 downto 0);
SIGNAL PositionB : STD_LOGIC_VECTOR (10 downto 0);
-- SPI --
SIGNAL SPI_DO : STD_LOGIC_VECTOR (DataSize-1 downto 0);
SIGNAL SPI_DI : STD_LOGIC_VECTOR (DataSize-1 downto 0);
SIGNAL SPI_SPE : STD_LOGIC;

-- motor control block -- direction (9-8), PWM (7-0) --
SIGNAL MC_A : STD_LOGIC_VECTOR (9 downto 0) := "0000000000";
SIGNAL MC_B : STD_LOGIC_VECTOR (9 downto 0) := "0000000000";

begin
---- instatiate component ----
-- motors --
MCS_A: MCS 
Port map(PWM_duty => MC_A(7 downto 0), 
			Motor_Direction_in => MC_A(9 downto 8), 
			Motor_Direction_out => MotorA,  -- directly connected to port --
			Position => PositionA, 
			Hall_sensors => HallA(2 downto 0),  -- directly connected to port --
			Clk => Clk, 
			ResetPosition => BTN(3));
MCS_B: MCS 
Port map(PWM_duty => MC_B(7 downto 0), 
			Motor_Direction_in => MC_B(9 downto 8),  
			Motor_Direction_out => MotorB,  -- directly connected to port --
			Position => PositionB, 
			Hall_sensors => HallB(2 downto 0),  -- directly connected to port --
			Clk => Clk, 
			ResetPosition => BTN(2));

-- SPI --
SPI_Slave: SPI 
Generic map (DataWidth => DataSize) 
Port map(MClk => SPI_MClk, -- directly connected to port --
			SS => SPI_SS, -- directly connected to port --
			MISO => SPI_MISO, -- directly connected to port --
			MOSI => SPI_MOSI, -- directly connected to port --
			DataOut => SPI_DO, -- connected to signal --
			DataIn => SPI_DI, -- connected to signal --
			SignalPackageEnd => SPI_SPE);  -- connected to signal --

-- display --
DispHex : DisplayHex 
Port Map(Segment => Seg, 
			DisplayThis => DisplayThis_DisplayPart_connect);
DispSelect : DisplaySelect_4_7segDisp 
Port Map(DisplayData => Display, 
			DisplayAN => AN, 
			DisplayPart => DisplayThis_DisplayPart_connect, 
			ClkSignal => Clk);



---- Code ----
LED <= MC_A(7 downto 0);

-- display position on 7 seg disp as HEX --
Display <= "0000000" & PositionA(10 downto 8) & '0' & PositionA(7 downto 4) & '0' & PositionA(3 downto 0) WHEN BTN(2) = '0' ELSE "0000000" & PositionB(10 downto 8) & '0' & PositionB(7 downto 4) & '0' & PositionB(3 downto 0);

-- coordinate SPI (send positionn and recieve and pass-on PWM) --
process (SPI_SPE)
variable dataToSend : integer range 0 to 1 := 0;
variable testInc : integer range 0 to 1080 := 0;
begin
	if rising_edge(SPI_SPE) then
		-- send data -- MotorID (11) & Position (10-0) --
		if dataToSend = 0 then
			-- send position of motorA --
			SPI_DI <= '0' & PositionA;
			--SPI_DI <= '0' & conv_std_logic_vector(testInc,11);
			--testInc := testInc + 1;
			--if testInc >= 1080 then
			--	testInc := 0;
			--end if;
			--testInc := testInc - 1;
			--if testInc > 1080 then
			--	testInc := 1079;
			--end if;
			dataToSend := 1;
		elsif dataToSend = 1 then
			-- send position of motorB --
			SPI_DI <= '1' & PositionB;
			dataToSend := 0;
		else
			dataToSend := 0;
		end if;
		
		-- recieve data -- '0' & MotorID (10) & Direction (9-8) & PWM (7-0) --
		Case SPI_DO(11 downto 10) is
			-- for motor A --
			when "10" => MC_A <= SPI_DO(9 downto 0);
			-- for motor B --
			when "11" => MC_B <= SPI_DO(9 downto 0);
			when others => 
		end case;
	end if;
end process;




end Behavioral;

