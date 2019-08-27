return {
	type = "method",
	description = [[Adds the dialogue to the player's journal, if applicable, at a given index.]],
	arguments = {{
		name = "params",
		type = "table",
		tableParams = {
			{ name = "index", type = "number", default = 0 },
			{ name = "actor", type = "tes3mobileActor|tes3reference|string", default = "tes3.player" },
		}
	}},
	returns = { { type = "boolean" } },
}