library IEEE;
use IEEE.std_logic_1164.all;
use IEEE.numeric_std.ALL;  -- https://www.xilinx.com/support/answers/45213.html

entity topLevel is
    generic (
		max_freq : integer := 10000000;
		max_counter : integer := 25000000;
		int_size : integer := 4
	 );

    port (
        -- Gloabals
        fpga_clk_50        : in  std_logic;             -- clock.clk
		  
        -- I/Os
        fpga_led_pio       : out std_logic_vector(5 downto 0);
		  enable             : in std_logic := '1';
		  switch             : in std_logic_vector(3 downto 0)
		  
	);
end entity topLevel;

architecture rtl of topLevel is

-- signal
signal blink : std_logic := '0';

begin

  process(fpga_clk_50) 
      variable counter : integer range 0 to max_counter := 0;
		variable limit   : integer range 0 to max_freq := max_freq;
		variable div     : integer := to_integer(unsigned(switch));
		begin
			if (enable = '1') then
			  if (rising_edge(fpga_clk_50)) then
				if (counter < shift_right(to_unsigned(limit, int_size), div)) then
					 counter := counter + 1;
				else
					 blink <= not blink;
					 counter := 0;
				end if;
			  end if;
			else
				blink <= '0';  -- blink off
		  end if;
  end process;

  fpga_led_pio(0) <= blink;
  fpga_led_pio(1) <= blink;
  fpga_led_pio(2) <= blink;
  fpga_led_pio(3) <= blink;
  fpga_led_pio(4) <= blink;
  fpga_led_pio(5) <= blink;
  
end architecture;
