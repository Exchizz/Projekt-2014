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

-- Uncomment the following library declaration if using
-- arithmetic functions with Signed or Unsigned values
--use IEEE.NUMERIC_STD.ALL;

-- Uncomment the following library declaration if instantiating
-- any Xilinx primitives in this code.
--library UNISIM;
--use UNISIM.VComponents.all;

entity fpga_main is
Port (SW : in STD_LOGIC_VECTOR (7 downto 0);
		AN : out STD_LOGIC_VECTOR (3 downto 0);
		Seg : out STD_LOGIC_VECTOR (7 downto 0);
		BTN : in STD_LOGIC_VECTOR (3 downto 0);
		HallA : in STD_LOGIC_VECTOR (2 downto 0);
		HallB : in STD_LOGIC_VECTOR (2 downto 0);
		MotorA : out STD_LOGIC_VECTOR (2 downto 0);
		MotorB : out STD_LOGIC_VECTOR (2 downto 0);
		Clk : in STD_LOGIC);
end fpga_main;

architecture Behavioral of fpga_main is
COMPONENT MCS is
    Port ( PWM_duty : in  STD_LOGIC_VECTOR (7 downto 0);
			  Motor_Direction_in : in  STD_LOGIC_VECTOR (1 downto 0);
			  Motor_Direction_out : out  STD_LOGIC_VECTOR (2 downto 0);
           Position : out  STD_LOGIC_VECTOR (10 downto 0);
			  Hall_sensors : in STD_LOGIC_VECTOR (2 downto 0);
			  Clk : in STD_LOGIC;
			  ResetPosition : in STD_LOGIC);
end component;

COMPONENT DisplayHex is
    Port ( Segment : out  STD_LOGIC_VECTOR (7 downto 0);
			  DisplayThis : in STD_LOGIC_VECTOR (4 downto 0));
end COMPONENT;

COMPONENT DisplaySelect_4_7segDisp is
    Port ( DisplayData : in  STD_LOGIC_VECTOR (19 downto 0);
           DisplayAN : out  STD_LOGIC_VECTOR (3 downto 0);
           DisplayPart : out  STD_LOGIC_VECTOR (4 downto 0);
			  ClkSignal : in STD_LOGIC);
end COMPONENT;


-- signal for 7seg disp --
SIGNAL PositionA : STD_LOGIC_VECTOR (10 downto 0);
SIGNAL PositionB : STD_LOGIC_VECTOR (10 downto 0);
SIGNAL DisplayThis_DisplayPart_connect : STD_LOGIC_VECTOR (4 downto 0);
SIGNAL Display : STD_LOGIC_VECTOR (19 downto 0);


begin
-- instatiate component --
MCS_A: MCS Port map(PWM_duty =>SW, Motor_Direction_in => BTN(1 downto 0), Motor_Direction_out => MotorA, Position => PositionA, Hall_sensors => HallA(2 downto 0), Clk => Clk, ResetPosition => BTN(3));
MCS_B: MCS Port map(PWM_duty =>SW, Motor_Direction_in => BTN(1 downto 0), Motor_Direction_out => MotorB, Position => PositionB, Hall_sensors => HallB(2 downto 0), Clk => Clk, ResetPosition => BTN(3));

DispHex : DisplayHex Port Map(Segment => Seg, DisplayThis => DisplayThis_DisplayPart_connect);
DispSelect : DisplaySelect_4_7segDisp Port Map (DisplayData => Display, DisplayAN => AN, DisplayPart => DisplayThis_DisplayPart_connect, ClkSignal => Clk);

-- display on 7 seg --
Display <= "0000000" & PositionA(10 downto 8) & '0' & PositionA(7 downto 4) & '0' & PositionA(3 downto 0) WHEN BTN(2) = '0' ELSE "0000000" & PositionB(10 downto 8) & '0' & PositionB(7 downto 4) & '0' & PositionB(3 downto 0);





end Behavioral;

