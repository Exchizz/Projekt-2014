
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;

entity Encoder is
    Port ( Clk : in STD_LOGIC;
			  Hall : in STD_LOGIC_VECTOR (1 downto 0);
			  Position : out STD_LOGIC_VECTOR (10 downto 0);
			  ResetPosition : in STD_LOGIC);
end Encoder;


architecture Behavioral of Encoder is


-- motor --
SHARED VARIABLE Motor_Position : STD_LOGIC_VECTOR (10 downto 0) := "00000000001";
--Signal DisplayThisData : STD_LOGIC_VECTOR (19 downto 0);

begin
-- output ... --
Position <= Motor_Position(10 downto 0);

-- position counter --
PROCESS (Clk,ResetPosition)
	variable last_hall : STD_LOGIC_VECTOR (1 downto 0);
	variable clk_scaler : Integer range 1 to 50000;
BEGIN
	if rising_edge(Clk) then
		clk_scaler := clk_scaler + 1;
		
		if clk_scaler = 50000 then
			clk_scaler := 1;
	
		-- count position --
		if NOT(last_hall = Hall) then
			CASE last_hall IS
			WHEN "00" =>
				if Hall = "10" then
					-- CW --
					if Motor_Position = "10000110111" then
						Motor_Position := "00000000000";
					else
						Motor_Position := Motor_Position + 1;
					end if;
					-- store new hall --
					last_hall := Hall;
				elsif Hall = "01" then 
					-- CCW --
					if Motor_Position = 0 then
						Motor_Position := "10000110111";
					else
						Motor_Position := Motor_Position - 1;
					end if;
					-- store new hall --
					last_hall := Hall;
				end if;
			WHEN "01" =>
				if Hall = "00" then
					-- CW --
					if Motor_Position = "10000110111" then
						Motor_Position := "00000000000";
					else
						Motor_Position := Motor_Position + 1;
					end if;
					-- store new hall --
					last_hall := Hall(1 downto 0);
				elsif Hall = "11" then 
					-- CCW --
					if Motor_Position = 0 then
						Motor_Position := "10000110111";
					else
						Motor_Position := Motor_Position - 1;
					end if;
					-- store new hall --
					last_hall := Hall;
				end if;
			WHEN "10" =>
				if Hall = "11" then
					-- CW --
					if Motor_Position = "10000110111" then
						Motor_Position := "00000000000";
					else
						Motor_Position := Motor_Position + 1;
					end if;
					-- store new hall --
					last_hall := Hall;
				elsif Hall = "00" then 
					-- CCW --
					if Motor_Position = 0 then
						Motor_Position := "10000110111";
					else
						Motor_Position := Motor_Position - 1;
					end if;
					-- store new hall --
					last_hall := Hall;
				end if;
			WHEN "11" =>
				if Hall = "01" then
					-- CW --
					if Motor_Position = "10000110111" then
						Motor_Position := "00000000000";
					else
						Motor_Position := Motor_Position + 1;
					end if;
					-- store new hall --
					last_hall := Hall;
				elsif Hall = "10" then 
					-- CCW --
					if Motor_Position = 0 then
						Motor_Position := "10000110111";
					else
						Motor_Position := Motor_Position - 1;
					end if;
					-- store new hall --
					last_hall := Hall;
				end if;
			WHEN OTHERS =>
			end case;
			
		-- end clk scaler
		end if;

		end if;

	end if;
	-- reset position --
	if ResetPosition = '1' then
		Motor_Position := "00000000000";
	end if;
END PROCESS;

end Behavioral;

