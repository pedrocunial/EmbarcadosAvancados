library IEEE;
use IEEE.std_logic_1164.all;
use IEEE.std_logic_unsigned.all;
use IEEE.numeric_std.all;
use work.all;

entity peripheral_LED is
    generic (
        LEN  : natural := 4;
		  FREQ : integer := 50000000
    );
    port (
        -- Gloabals
        clk             : in  std_logic                     := '0';
        reset           : in  std_logic                     := '0';

        -- I/Os
--		  -- ponte-H
		  LEDs      : out std_logic_vector(LEN-1 downto 0) := (others=>'0');
		  pwm       : out std_logic := '0';


        -- Avalion Memmory Mapped Slave
        avs_address     : in  std_logic_vector(3 downto 0)  := (others => '0'); 
        avs_read        : in  std_logic                     := '0';             
        avs_readdata    : out std_logic_vector(31 downto 0) := (others => '0'); 
        avs_write       : in  std_logic                     := '0';             
        avs_writedata   : in  std_logic_vector(31 downto 0) := (others => '0')
	);
end entity peripheral_LED;

architecture rtl of peripheral_LED is
	signal duty_cycle : integer := 0;
	signal s_leds     : std_logic_vector(LEN-1 downto 0);
	signal timed_out  : std_logic := '0';
	signal timer      : integer := 0;
	signal large_pwm  : std_logic_vector(LEN-1 downto 0);
	signal in_pwm     : std_logic := '0';
 begin

  process(clk)
  begin
    if (reset = '1' ) then
      s_leds <= (others => '0');
    elsif(rising_edge(clk)) then
		if(avs_write = '1') then
			if (avs_address = "0010") then
				s_leds <= avs_writedata(LEN-1 downto 0);
         elsif(avs_address = "0001") then                  -- REG_DATA
            duty_cycle <= to_integer(signed(avs_writedata(9 downto 0)));
         elsif (avs_address = "0100") then
				timer <= to_integer(signed(avs_writedata(31 downto 0)));
			end if;
      end if;
    end if;
  end process;
  

  
  process(clk)
	variable count0 : integer := 0;
	variable count1 : integer := 0;
  begin
	
	if (rising_edge(clk)) then
		if (count0 < 500) then
			count0 := count0 + 1;
		elsif (timed_out = '0') then
			count0 := 0;
			count1 := count1 + 1;
			if (count1 < duty_cycle) then
				in_pwm <= '1';
			elsif (count1 < 1024) then
				in_pwm <= '0';
			else
				count1 := 0;
				in_pwm <= '1';
			end if;
		else
			in_pwm <= '0';
		end if;
	end if;
  end process;

  process(clk)
	variable counter      : integer := 0;
	variable time_counter : integer := 0;
  begin
	  if (rising_edge(clk)) then
			if (counter < FREQ) then
				counter := counter + 1;
			else
				counter := 0;
				if (time_counter > timer) then
					timed_out <= '1';
				else	
					time_counter := time_counter + 1;
					timed_out <= '0';
				end if;
			end if;
		end if;
	end process;

  large_pwm <= (others => in_pwm);
  LEDs <= s_leds and large_pwm;

end rtl;