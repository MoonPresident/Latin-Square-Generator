# Latin-Square-Generator

Generate solved square with "generate_latin_square". Returns full latin square.

Get clues for that solution by passing that square into "get_tower_clues". Returns list of clues in clockwise order.

Remove numbers by using "cull_latin_square". Returns latin square with reduced entries.
Will remove non-zero entries only, to the number passed or until all entries are empty.
Culling a partially empty square will remove more values.
i.e. cull_latin_square(latin, 20) will remove as many entries as cull_latin_square(cull_latin_square(latin, 10), 10).
