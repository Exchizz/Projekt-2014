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
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;

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
			  Clk : in STD_LOGIC;
			  ResetPosition : in STD_LOGIC);
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
			  Hall : in STD_LOGIC_VECTOR (1 downto 0);
			  Position : out STD_LOGIC_VECTOR (10 downto 0);
			  ResetPosition : in STD_LOGIC);
end COMPONENT;


-- signals for reset --
--SIGNAL PWM_reset : STD_LOGIC_VECTOR (7 downto 0);

-- Signals for components --
SIGNAL PWM_PIN : STD_LOGIC := '0';
SIGNAL ResetNow : STD_LOGIC := '0';
SIGNAL MotorState : STD_LOGIC_VECTOR(1 downto 0) := "00"; -- 00 = run, 01 = init, 10 = find, 11 = done
SIGNAL PWM_signal : STD_LOGIC_VECTOR (7 downto 0);
SIGNAL Position_signal : STD_LOGIC_VECTOR (10 downto 0);
SIGNAL Motor_Dir_out_run : STD_LOGIC_VECTOR (2 downto 0);
SIGNAL Motor_Dir_out_reset : STD_LOGIC_VECTOR (1 downto 0);

begin
PWM_part: PWM PORT MAP(clk => Clk, Pin => PWM_PIN, PWM => PWM_signal);
ENCODER_part: Encoder PORT MAP(Clk => Clk, Hall => Hall_sensors(1 downto 0), Position => Position_Signal, ResetPosition => ResetNow);

position <= Position_Signal;

---- PWM_part implementation ----
-- Motor_Direction_in, Motor_Direction_out implementation --
WITH Motor_Direction_in SELECT
Motor_Dir_out_run <=  "000" WHEN "00",
								"10" & PWM_PIN WHEN "01",
								'1' & PWM_PIN & '0' WHEN "10",
								"111" WHEN "11",
								"000" WHEN OTHERS;

Motor_Direction_out <= Motor_Dir_out_run WHEN MotorState = "00" ELSE '1' & Motor_Dir_out_reset;

-- PWM control -- when in state "run" use input, else control otherwise
PWM_signal <= "01000000" WHEN MotorState = "10" ELSE PWM_duty;


---- ResetPosition implementation ----

PROCESS (Clk)
Variable direction : STD_LOGIC := '1';
variable clk_scaler : INTEGER range 1 to 5000000 := 1;
variable last_position : INTEGER range 0 to 1080;
variable position_change : INTEGER range 0 to 1080;
variable wait_signal : INTEGER range 0 to 1000 := 0;

BEGIN
	if rising_edge(Clk) then
		-- btn pressed --
		if ResetPosition = '1' then
			MotorState <= "01";
		end if;
		-- state 11, retun to run --
		if MotorState = "11" then
			wait_signal := wait_signal +1;
			if wait_signal = 1000 then
				ResetNow <= '0';
				Motor_Dir_out_reset <= "00";
				MotorState <= "00";
				wait_signal := 0;
			end if;
		end if;
		-- state 01, init --
		if MotorState = "01" then
			MotorState <= "10";
			clk_scaler := 1;
			last_position := conv_integer(Position_Signal);
			--PWM_reset <= "01000000";
		end if;
		
		-- state 01, find zero --
		if MotorState = "10" then
			clk_scaler := clk_scaler + 1;
			if direction = '1' then 
				Motor_Dir_out_reset <= '0' & PWM_PIN;
			else
				Motor_Dir_out_reset <= PWM_PIN & '0';
			end if;
			-- if it reaches zero --
			if Hall_sensors(2) = '0' then
				ResetNow <= '1';
				Motor_Dir_out_reset <= "11";
				MotorState <= "11";
			end if;
			-- detect collision --
			if clk_scaler = 5000000 then
				clk_scaler := 1;
				
				-- compare --
				if direction = '1' then
					position_change := conv_integer(Position_Signal) - last_position;
				else 
					position_change := last_position - conv_integer(Position_Signal);
				end if;
				-- decide if position needs to be changed --
				if position_change < 3 then
					direction := Not(direction);
				end if;
				-- safe the position for next time --
				last_position := conv_integer(Position_Signal);
			end if;
			
		end if;
		
	end if;
END PROCESS;



end Behavioral;

