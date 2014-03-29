----------------------------------------------------------------------------------
-- Company: 
-- Engineer: 
-- 
-- Create Date:    14:25:51 03/28/2014 
-- Design Name: 
-- Module Name:    MCS - Behavioral 
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

entity MCS is
    Port ( PWM_duty : in  STD_LOGIC_VECTOR (7 downto 0);
			  Motor_Direction_in : in  STD_LOGIC_VECTOR (1 downto 0);
			  Motor_Direction_out : out  STD_LOGIC_VECTOR (2 downto 0);
           Position : out  STD_LOGIC_VECTOR (10 downto 0);
			  Hall_sensors : in STD_LOGIC_VECTOR (2 downto 0);
			  Clk : in STD_LOGIC);
end MCS;

architecture Behavioral of MCS is
-- PWM --
COMPONENT PWM is
port (
	clk: in std_logic;
	--LED: out std_logic_vector (7 downto 0);
	Pin: out std_logic;
	PWM: in std_logic_vector (7 downto 0)
	);
end COMPONENT;

-- ENCODER --
COMPONENT Encoder is
    Port ( Clk : in STD_LOGIC;
			  Hall : in STD_LOGIC_VECTOR (2 downto 0);
			  Position : out STD_LOGIC_VECTOR (10 downto 0));
end COMPONENT;

-- Signals for components --
SIGNAL PWM_SIGNAL : STD_LOGIC;


begin
PWM_part: PWM PORT MAP(clk => Clk, Pin => PWM_SIGNAL, PWM => PWM_Duty);
ENCODER_part: Encoder PORT MAP(Clk => Clk, Hall => Hall_sensors, Position => Position);


---- PWM_part implementation ----
-- Motor_Direction_in, Motor_Direction_out implementation --
WITH Motor_Direction_in SELECT
Motor_Direction_out <=  "000" WHEN "00",
								"10" & PWM_SIGNAL WHEN "01",
								'1' & PWM_SIGNAL & '0' WHEN "10",
								"111" WHEN "11",
								"000" WHEN OTHERS;

----  ----


end Behavioral;

