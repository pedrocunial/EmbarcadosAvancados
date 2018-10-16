	component niosHello is
		port (
			butaos_export : in  std_logic_vector(4 downto 0) := (others => 'X'); -- export
			clk_clk       : in  std_logic                    := 'X';             -- clk
			leds_1_name   : out std_logic_vector(3 downto 0);                    -- name
			reset_reset_n : in  std_logic                    := 'X'              -- reset_n
		);
	end component niosHello;

	u0 : component niosHello
		port map (
			butaos_export => CONNECTED_TO_butaos_export, -- butaos.export
			clk_clk       => CONNECTED_TO_clk_clk,       --    clk.clk
			leds_1_name   => CONNECTED_TO_leds_1_name,   -- leds_1.name
			reset_reset_n => CONNECTED_TO_reset_reset_n  --  reset.reset_n
		);

